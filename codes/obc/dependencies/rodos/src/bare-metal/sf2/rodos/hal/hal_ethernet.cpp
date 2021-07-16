#include "hal/hal_ethernet.h"
#include "rodos.h"
#include "rodos-lwip.h"
#include "lwip/pbuf.h"
#include "drivers/mss_ethernet_mac.h"
#include "drivers/mss_sys_services.h"
#include "drivers/mss_gpio.h"

namespace RODOS {

constexpr uint16_t BUFFER_SIZE = 1518;

static void mac_rx_callback(uint8_t* p_rx_packet, uint32_t pckt_length,
                            void* caller_info);

static void packet_tx_complete_handler(void* caller_info);

uint8_t __attribute__((section(".ddr"))) rxData[BUFFER_SIZE], txData[BUFFER_SIZE];

typedef struct ranctx {
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
} ranctx;

static ranctx ctx;
#define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))
static uint32_t ranval(ranctx* x) {
    uint32_t e = x->a - rot(x->b, 27);
    x->a       = x->b ^ rot(x->c, 17);
    x->b       = x->c + x->d;
    x->c       = x->d + e;
    x->d       = e + x->a;
    return x->d;
}

static void raninit(ranctx* x, uint32_t seed) {
    uint32_t i;
    x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
    for(i = 0; i < 20; ++i) {
        (void)ranval(x);
    }
}

extern "C" {
uint32_t lwip_rand() {
    static bool initialized = false;
    if(!initialized) {
        raninit(&ctx, static_cast<uint32_t>(NOW() & 0xFFFFFFFF));
        initialized = true;
    }
    return ranval(&ctx);
}
}

class HW_HAL_ETH {
    volatile uint16_t dataLen = 0;

  public:
    HAL_ETH* hal;
    pbuf*    buf;
    /* MSS Ethernet MAC configuration record */
    mss_mac_cfg_t mac_config;
    volatile bool hasNewData = false, txComplete = true;

    HW_HAL_ETH() {
        buf = 0;
        hal = 0;
    }

    bool isLinkActive() {
        uint8_t         link_up;
        mss_mac_speed_t speed;
        uint8_t         full_duplex;
        link_up = MSS_MAC_get_link_status(&speed, &full_duplex);
        return link_up == MSS_MAC_LINK_UP;
    }

    void initialize_mss_mac(void) {
        NVIC_SetPriority(EthernetMAC_IRQn, 5u);
        NVIC_SetPriority(UART0_IRQn, 4u); //TODO CHECK if that is correct?!
        MSS_MAC_init(&mac_config);
        MSS_MAC_set_tx_callback(packet_tx_complete_handler);
        MSS_MAC_set_rx_callback(mac_rx_callback);
        MSS_MAC_receive_pkt(rxData, 0);
        NVIC_EnableIRQ(EthernetMAC_IRQn);
    }

    void ethernet_init(void) {
        uint8_t serialNum[16] = { 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                                  0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };
        // TXDIS
        MSS_GPIO_config(MSS_GPIO_11, MSS_GPIO_OUTPUT_MODE);
        MSS_GPIO_set_output(MSS_GPIO_11, 1);
        // RESET the Ethernet Chip
        MSS_GPIO_config(MSS_GPIO_10, MSS_GPIO_OUTPUT_MODE);
        MSS_GPIO_set_output(MSS_GPIO_10, 1);

        int64_t end = NOW() + 100 * MICROSECONDS;
        BUSY_WAITING_UNTIL(end);
        MSS_GPIO_set_output(MSS_GPIO_10, 0);
        end = NOW() + 100 * MICROSECONDS;
        BUSY_WAITING_UNTIL(end);
        MSS_GPIO_set_output(MSS_GPIO_10, 1);

        end = NOW() + 100 * MILLISECONDS;
        AT(end); // SM: no bussy waiting for 100 ms!

        MSS_SYS_get_serial_number(serialNum);
        MSS_MAC_cfg_struct_def_init(&mac_config);
        for(int i = 0; i < 6; i++) {
            uint8_t digit = serialNum[i * 2];
            if(i == 0) {
                digit |= 0x02;  // Mark as locally administered MAC address
                digit = static_cast<uint8_t>(digit & ~0x01); // Mark as unicast
            }
            mac_config.mac_addr[i] = digit;
        }
        mac_config.speed_duplex_select = MSS_MAC_ANEG_ALL_SPEEDS;
        mac_config.phy_addr            = 0x00;
        initialize_mss_mac();
    }

    ETH_ERR read() {
        if(!hasNewData)
            return ETH_ERR_NODATA;
        if(!isLinkActive())
            return ETH_ERR_LINKDOWN;
        buf          = pbuf_alloc(PBUF_RAW, BUFFER_SIZE, PBUF_POOL);
        uint16_t len = min(dataLen, BUFFER_SIZE);
        pbuf_take(buf, rxData, len); // BUFFER_SIZE PBUF_POOL_SIZE
        pbuf_realloc(buf, len);
        hasNewData = false;
        MSS_MAC_receive_pkt(rxData, 0); //re-activate RX
        return ETH_ERR_OK;
    }

    ETH_ERR write(pbuf* writeBuffer) {
        uint8_t* packetWritePtr = txData;
        uint8_t* packetLimit    = txData + 1518;
        pbuf*    buffer         = writeBuffer;
        uint32_t len;

        if(!isLinkActive()) {
            return ETH_ERR_LINKDOWN;
        }

        if(!writeBuffer) {
            return ETH_ERR_NODATA;
        }

        if(!txComplete) {
            return ETH_ERR_BUFFERFULL;
        }
        while(1) {
            if(packetWritePtr >= packetLimit)
                return ETH_ERR_BUFFERFULL;

            memcpy(packetWritePtr, buffer->payload, buffer->len);
            packetWritePtr += buffer->len;

            // Why not buffer->next != NULL? See http://www.nongnu.org/lwip/pbuf_8c.html
            if(buffer->tot_len == buffer->len)
                break;

            buffer = buffer->next;
        }

        len        = static_cast<uint32_t>(packetWritePtr - txData);
        txComplete = false;
        MSS_MAC_send_pkt(txData, len, 0);
        return ETH_ERR_OK;
    }

    void rxCallBack([[gnu::unused]] uint8_t* p_rx_packet, uint32_t pckt_length,
                    [[gnu::unused]] void* caller_info) {
        hasNewData = true;
        dataLen    = static_cast<uint16_t>(pckt_length);
        hal->upCallDataReady();
    }

    void txCallBack([[gnu::unused]] void* caller_info) {
        txComplete = true;
        hal->upCallWriteFinished();
    }
};

HW_HAL_ETH theOneAndOnlyEthContext;

HAL_ETH::HAL_ETH([[gnu::unused]] ETH_IDX idx) {
    context = &theOneAndOnlyEthContext;
}

static void mac_rx_callback(uint8_t* p_rx_packet, uint32_t pckt_length,
                            void* caller_info) {
    theOneAndOnlyEthContext.rxCallBack(p_rx_packet, pckt_length, caller_info);
}

static void packet_tx_complete_handler(void* caller_info) {
    theOneAndOnlyEthContext.txCallBack(caller_info);
}

int32_t HAL_ETH::init() {
    context->hal = this;
    context->ethernet_init();
    return 0;
}

bool HAL_ETH::checkLinkStatus() {
    return context->isLinkActive();
}

bool HAL_ETH::isWriteFinished() {
    return context->txComplete;
}

bool HAL_ETH::isDataReady() {
    return context->hasNewData;
}

ETH_ERR HAL_ETH::write(pbuf* writeBuffer) {
    return context->write(writeBuffer);
}

ETH_ERR HAL_ETH::read(pbuf** readBuffer) {
    if(!context->isLinkActive()) {
        return ETH_ERR_LINKDOWN;
    }

    ETH_ERR result = context->read();
    *readBuffer    = context->buf;
    return result;
}

const uint8_t* HAL_ETH::getMACAddr() {
    return context->mac_config.mac_addr;
}

} /* namespace RODOS */

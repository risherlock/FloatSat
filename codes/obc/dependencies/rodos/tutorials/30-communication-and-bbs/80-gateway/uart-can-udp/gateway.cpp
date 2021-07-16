
#include "rodos.h"
#include "gateway.h"

#define GATEWAY_UDP


#ifdef GATEWAY_CAN
    RODOS::HAL_CAN canPort(CAN_IDX0);
    LinkinterfaceCAN linkinterface(&canPort);
    
    class InitCanForGateway : public Initiator {
        void init() { canPort.init(1000000); }
    } initCanForGateway;
#endif


#ifdef GATEWAY_UART
    #warning "you will need differnet uart interface: one for sender, one for receiver"
    # for uart id see rodos-core/api/hal/hal_uart.h : 40 
    #    UART_IDX0         "/dev/ttyUSB0" 
    #   UART_IDX1         "/dev/ttyUSB1"
    #   UART_IDX2         "/dev/rfcomm0"
    #   UART_IDX3         "/dev/rfcomm1"
    #   UART_IDX4         "/dev/ttyS0"  
    #   UART_IDX5         "/dev/ttyS1"  


    RODOS::HAL_UART uart0(UART_IDX0);
    LinkinterfaceUART linkinterface(&uart0);
    class InitUartorGateway : public Initiator {
        void init() { uart0.init(115200); }
    } initCanForGateway;
#endif


#ifdef GATEWAY_UDP
    static UDPInOut udp(-50000);
    static LinkinterfaceUDP linkinterface(&udp);
#endif


static Gateway gateway1(&linkinterface, true);



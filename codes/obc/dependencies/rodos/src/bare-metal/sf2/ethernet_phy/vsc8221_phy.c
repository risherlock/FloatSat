/*
 * vsc8221_phy.c
 *
 *  Created on: Oct 12, 2016
 *      Author: karstenbecker
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "drivers/phy.h"
#include "drivers/mss_ethernet_mac.h"
#include "vsc8221_phy.h"

static uint8_t g_phy_addr = 0;

static void write_reg(uint8_t reg, uint16_t value) {
    MSS_MAC_write_phy_reg(g_phy_addr, reg, value);
}

static uint16_t read_reg(uint8_t reg) {
    return MSS_MAC_read_phy_reg(g_phy_addr, reg);
}

void MSS_MAC_phy_init(uint8_t phy_addr) {
    g_phy_addr = phy_addr;
    write_reg(31, 0x2A30);
    write_reg(8, 0x0212);
    write_reg(31, 0x52B5);
    write_reg(2, 0x000F);
    write_reg(1, 0x472A);
    write_reg(0, 0x8FA4);
    write_reg(31, 0x2A30);
    write_reg(8, 0x0012);
    write_reg(31, 0x0000);
#ifdef VSC8221_DISABLE_AUTONEGOTIATION
    write_reg(30, 0x4000); // Disable Clause 37 ANEG
#endif
}

static void enableMDIX() {
    write_reg(31, 0x52b5);
    write_reg(18, 0x0012);
    write_reg(17, 0x2803);
    write_reg(16, 0x87fa);
    write_reg(31, 0x0000);
}

#ifdef VSC8221_DEBUG

#    define INT_ENABLE_STATUS (1u << 15)
#    define LINK_STATE_CHANGE (1u << 13)
#    define ANEG_ERROR (1u << 11)
#    define ANEG_INTERLOCK_DONE (1u << 10)
#    define POWERED_DEV_DETECT (1u << 9)
#    define SYMBOL_ERR (1u << 8)
#    define DESCRAMBLER_ERR (1u << 7)
#    define TX_FIFO (1u << 6)
#    define RX_FIFO (1u << 5)
#    define FALSE_CARRIER (1u << 3)
#    define CABLE_IMPAIRMENT (1u << 2)
#    define MASTER_SLAVE_RES_ERR (1u << 1)
#    define RXER (1u << 0)

static void printReg(uint16_t reg, uint8_t bit, char* ifStr, char* elseStr) {
    if(reg & (1u << bit))
        printf(" %s", ifStr);
    else if(elseStr != NULL)
        printf(" %s", elseStr);
}

void phy_decodeRegisters() {
    uint16_t reg = read_reg(0);
    printf("Reg 0:");
    printReg(reg, 15, "RESET", NULL);
    printReg(reg, 14, "NEAR_END_LOOPBACK", NULL);
    uint8_t tmp = (((reg >> 6) & 1) << 1) | ((reg >> 13) & 1);
    switch(tmp) {
        case 0b00:
            printf(" 10M");
            break;
        case 0b01:
            printf(" 100M");
            break;
        case 0b10:
            printf(" 1000M");
            break;
        case 0b11:
            printf(" RESERVED_SPD");
            break;
    }
    printReg(reg, 12, "ANEG_ENABLED", NULL);
    printReg(reg, 11, "POWER_DOWN", NULL);
    printReg(reg, 10, "ISOLATE", NULL);
    printReg(reg, 9, "RESTART_MII", NULL);
    printReg(reg, 8, "FULL_DUPLEX", "HALF_DUPLEX");
    printf("\n");

    reg = read_reg(1);
    printf("Reg: 1");
    printReg(reg, 6, "MF_PREAMBLE_SURPRESSION", NULL);
    printReg(reg, 5, "ANG_COMPLETE", NULL);
    printReg(reg, 4, "REMOTE_FAULT", NULL);
    printReg(reg, 2, "LINK_UP", "LINK_DOWN");
    printReg(reg, 1, "JABBER_DETECTED", NULL);
    printf("\n");

    reg = read_reg(4);
    printf("Reg: 4");
    printReg(reg, 15, "NEXT_PAGE_REQUEST", NULL);
    printReg(reg, 13, "TX_REMOTE_FAULT", NULL);
    printReg(reg, 11, "ADV_ASYM_PAUSE_CAP", NULL);
    printReg(reg, 10, "ADV_SYM_PAUSE_CAP", NULL);
    printReg(reg, 9, "ADV_100T4_CAP", NULL);
    printReg(reg, 8, "ADV_100TX_FD_CAP", NULL);
    printReg(reg, 7, "ADV_100TX_HD_CAP", NULL);
    printReg(reg, 6, "ADV_10TX_FD_CAP", NULL);
    printReg(reg, 5, "ADV_10TX_HD_CAP", NULL);
    printf("\n");

    reg = read_reg(5);
    printf("Reg: 5");
    printReg(reg, 15, "LP_NEXT_PAGE_REQUEST", NULL);
    printReg(reg, 14, "LP_ACK", NULL);
    printReg(reg, 13, "LP_TX_REMOTE_FAULT", NULL);
    printReg(reg, 11, "LP_ADV_ASYM_PAUSE_CAP", NULL);
    printReg(reg, 10, "LP_ADV_SYM_PAUSE_CAP", NULL);
    printReg(reg, 9, "LP_ADV_100T4_CAP", NULL);
    printReg(reg, 8, "LP_ADV_100TX_FD_CAP", NULL);
    printReg(reg, 7, "LP_ADV_100TX_HD_CAP", NULL);
    printReg(reg, 6, "LP_ADV_10TX_FD_CAP", NULL);
    printReg(reg, 5, "LP_ADV_10TX_HD_CAP", NULL);
    printf("\n");

    reg = read_reg(9);
    printf("Reg: 9");
    printReg(reg, 12, "MANUAL_MASTER_ENABLE", NULL);
    printReg(reg, 11, "MANUAL_MASTER", "MANUAL_SLAVE");
    printReg(reg, 10, "MULI_PORT", "SINGLE_PORT");
    printReg(reg, 9, "ADV_1000TX_FD_CAP", NULL);
    printReg(reg, 8, "ADV_1000TX_HD_CAP", NULL);
    printf("\n");

    reg = read_reg(10);
    printf("Reg: 10");
    printReg(reg, 15, "MASTER_SLAVE_ERR", NULL);
    printReg(reg, 14, "IS_MASTER", "IS_SLAVE");
    printReg(reg, 13, "RCVR_STATUS_OK", "RCVR_STATUS_FAULT");
    printReg(reg, 12, "REM_RCVR_STATUS_OK", "REM_RCVR_STATUS_FAULT");
    printReg(reg, 11, "LP_1000TX_CAP", NULL);
    printReg(reg, 10, "LP_1000TX_CAP", NULL);
    printf("\n");

    reg = read_reg(23);
    printf("Reg: 23");
    uint8_t newVal = (((reg >> 12) & 0xF) << 2) | ((reg >> 1) & 3);
    switch(newVal) {
        case 0b111100:
            printf(" 802.3z SerDes CAT5 Clause 37 disabled");
            break;
        case 0b111001:
            printf(" 802.3z SerDes CAT5 Clause 37 enabled");
            break;
        case 0b111010:
            printf(" 802.3z SerDes CAT5 Clause 37 enabled Media Converter");
            break;
        case 0b111000:
            printf(" 802.3z SerDes CAT5 Clause 37 Auto Detection");
            break;
        case 0b101001:
            printf(" SGMII CAT5 625MHz SCLK Disabled");
            break;
        case 0b100001:
            printf(" SGMII CAT5 625MHz SCLK Enabled");
            break;
        case 0b100100:
            printf(" SGMII CAT5 625MHz SCLK Disabled Clause 37 Auto-neg disabled");
            break;
        case 0b101101:
            printf(" SGMII CAT5 625MHz SCLK Enabled Clause 37 Auto-neg disabled");
            break;
        default:
            printf("Unknown/incorrect mode %02X %04X", newVal, reg);
            break;
    }
    printReg(reg, 3, "FAR_END_LOOP", NULL);
    printReg(reg, 0, "EEPROM_DETECTED", NULL);
    printf("\n");

    reg = read_reg(28);
    printf("Reg: 28");
    printReg(reg, 15, "ANEG_COMPLETE", NULL);
    printReg(reg, 14, "ANEG_DISABLED", "ANEG_ENABLED");
    printReg(reg, 13, "MDI_CROSSOVER", NULL);
    printReg(reg, 12, "CD_SWAPPED", NULL);
    printReg(reg, 11, "A_POL_SWAP", NULL);
    printReg(reg, 10, "B_POL_SWAP", NULL);
    printReg(reg, 9, "C_POL_SWAP", NULL);
    printReg(reg, 8, "D_POL_SWAP", NULL);
    printReg(reg, 6, "ACTI_PHY", NULL);
    printReg(reg, 5, "FULL_DUPLEX", "HALF_DUPLEX");
    tmp = (reg >> 3) & 3;
    switch(tmp) {
        case 0b00:
            printf(" 10M");
            break;
        case 0b01:
            printf(" 100M");
            break;
        case 0b10:
            printf(" 1000M");
            break;
        case 0b11:
            printf(" RESERVED_SPD");
            break;
    }
    printf("\n");
}

void phy_checkInterrupts() {
    write_reg(25,
              INT_ENABLE_STATUS | LINK_STATE_CHANGE | ANEG_ERROR | ANEG_INTERLOCK_DONE | POWERED_DEV_DETECT |
                SYMBOL_ERR | DESCRAMBLER_ERR | TX_FIFO | RX_FIFO | FALSE_CARRIER | CABLE_IMPAIRMENT | MASTER_SLAVE_RES_ERR | RXER);
    uint16_t reg = read_reg(26);
    if(reg & INT_ENABLE_STATUS) {
        printf("Interrupts:");
        if(reg & LINK_STATE_CHANGE)
            printf(" LINK_STATE_CHANGE");
        if(reg & ANEG_ERROR)
            printf(" ANEG_ERROR");
        if(reg & ANEG_INTERLOCK_DONE)
            printf(" ANEG_INTERLOCK_DONE");
        if(reg & POWERED_DEV_DETECT)
            printf(" POWERED_DEV_DETECT");
        if(reg & SYMBOL_ERR)
            printf(" SYMBOL_ERR");
        if(reg & DESCRAMBLER_ERR)
            printf(" DESCRAMBLER_ERR");
        if(reg & TX_FIFO)
            printf(" TX_FIFO");
        if(reg & RX_FIFO)
            printf(" RX_FIFO");
        if(reg & FALSE_CARRIER)
            printf(" FALSE_CARRIER");
        if(reg & CABLE_IMPAIRMENT)
            printf(" CABLE_IMPAIRMENT");
        if(reg & MASTER_SLAVE_RES_ERR)
            printf(" MASTER_SLAVE_RES_ERR");
        if(reg & RXER)
            printf(" RXER");
        printf("\n");
    } else {
        printf("No interrupts\n");
    }
}

#endif

/**************************************************************************/ /**
 *
 */

#define BMSR_AUTO_NEGOTIATION_COMPLETE 0x20

#define FD (1u << 8)
#define SPD_10M ()
#define SPD_100M (1u << 13)
#define SPD_1000M (1u << 6)

void phy_setLinkSpeedManual(uint32_t speed_duplex_select) {
    enableMDIX();
    uint16_t reg = read_reg(0);
    reg &= ~((1u << 12) | (1u << 6) | (1u << 13) | (1u << 8));
    switch(speed_duplex_select) {
        case MSS_MAC_ANEG_10M_FD:
            reg |= FD;
            write_reg(0, reg);
            break;
        case MSS_MAC_ANEG_10M_HD:
            write_reg(0, reg);
            //reg 6&13 is 0
            enableMDIX();
            break;
        case MSS_MAC_ANEG_100M_FD:
            reg |= SPD_100M | FD;
            write_reg(0, reg);
            enableMDIX();
            break;
        case MSS_MAC_ANEG_100M_HD:
            reg |= SPD_100M;
            write_reg(0, reg);
            enableMDIX();
            break;
        case MSS_MAC_ANEG_1000M_FD:
            reg |= SPD_1000M | FD;
            write_reg(0, reg);
            break;
        case MSS_MAC_ANEG_1000M_HD:
            reg |= SPD_1000M;
            write_reg(0, reg);
            break;
    }
    //Update Register 9.9 and 9.8
}

void MSS_MAC_phy_set_link_speed(uint32_t speed_duplex_select) {
    uint16_t       phy_reg;
    uint32_t       inc;
    uint32_t       speed_select;
    const uint16_t mii_advertise_bits[4] = { ADVERTISE_10FULL, ADVERTISE_10HALF, ADVERTISE_100FULL, ADVERTISE_100HALF };

    /* Set auto-negotiation advertisement. */

    /* Set 10Mbps and 100Mbps advertisement. */
    phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, MII_ADVERTISE);
    phy_reg &= ~(ADVERTISE_10HALF | ADVERTISE_10FULL |
                 ADVERTISE_100HALF | ADVERTISE_100FULL);

    speed_select = speed_duplex_select;
    for(inc = 0u; inc < 4u; ++inc) {
        uint32_t advertise;
        advertise = speed_select & 0x00000001u;
        if(advertise != 0u) {
            phy_reg |= mii_advertise_bits[inc];
        }
        speed_select = speed_select >> 1u;
    }

    MSS_MAC_write_phy_reg(g_phy_addr, MII_ADVERTISE, phy_reg);

    /* Set 1000Mbps advertisement. */
    phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, MII_CTRL1000);
    phy_reg &= ~(ADVERTISE_1000FULL | ADVERTISE_1000HALF);

    if((speed_duplex_select & MSS_MAC_ANEG_1000M_FD) != 0u) {
        phy_reg |= ADVERTISE_1000FULL;
    }

    if((speed_duplex_select & MSS_MAC_ANEG_1000M_HD) != 0u) {
        phy_reg |= ADVERTISE_1000HALF;
    }

    MSS_MAC_write_phy_reg(g_phy_addr, MII_CTRL1000, phy_reg);
}

void MSS_MAC_phy_autonegotiate_start(void) {
    uint16_t phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, MII_BMCR);
    write_reg(MII_BMCR, phy_reg | 0x100);
}

bool MSS_MAC_phy_autonegotiate_complete(void) {
    uint16_t phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, MII_BMSR);
    if(phy_reg == 0xFFFF)
        return false;
    return (phy_reg & BMSR_AUTO_NEGOTIATION_COMPLETE) != 0;
}

void MSS_MAC_phy_autonegotiate(void) {
    bool              autoneg_complete;
    volatile uint32_t copper_aneg_timeout = 700000u;
    MSS_MAC_phy_autonegotiate_start();
    do {
        autoneg_complete = MSS_MAC_phy_autonegotiate_complete();
        --copper_aneg_timeout;
    } while(!autoneg_complete && (copper_aneg_timeout != 0u));
}

uint8_t MSS_MAC_phy_get_link_status(mss_mac_speed_t* speed,
                                    uint8_t*         fullduplex) {
    uint16_t phy_reg;
    uint16_t link_up;
    uint8_t  link_status;

    phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, MII_BMSR);
    link_up = phy_reg & BMSR_LSTATUS;

    if(link_up != MSS_MAC_LINK_DOWN) {
        uint16_t op_mode;

        link_status = MSS_MAC_LINK_UP;

        phy_reg = MSS_MAC_read_phy_reg(g_phy_addr, 0x1C);
        op_mode = (phy_reg >> 3) & 0x0007u;
        switch(op_mode) {
            case 0b000:
                *speed      = MSS_MAC_10MBPS;
                *fullduplex = MSS_MAC_HALF_DUPLEX;
                break;
            case 0b100:
                *speed      = MSS_MAC_10MBPS;
                *fullduplex = MSS_MAC_FULL_DUPLEX;
                break;
            case 0b001:
                *speed      = MSS_MAC_100MBPS;
                *fullduplex = MSS_MAC_HALF_DUPLEX;
                break;
            case 0b101:
                *speed      = MSS_MAC_100MBPS;
                *fullduplex = MSS_MAC_FULL_DUPLEX;
                break;
            case 0b010:
                *speed      = MSS_MAC_1000MBPS;
                *fullduplex = MSS_MAC_HALF_DUPLEX;
                break;
            case 0b110:
                *speed      = MSS_MAC_1000MBPS;
                *fullduplex = MSS_MAC_FULL_DUPLEX;
                break;
            default:
                link_status = MSS_MAC_LINK_DOWN;
                break;
        }
    } else {
        link_status = MSS_MAC_LINK_DOWN;
    }

    return link_status;
}

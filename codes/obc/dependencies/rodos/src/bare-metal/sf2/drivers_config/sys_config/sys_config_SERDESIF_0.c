/*=============================================================*/
/* Created by Microsemi SmartDesign Fri Mar 03 11:10:38 2017   */
/*                                                             */
/* Warning: Do not modify this file, it may lead to unexpected */
/*          functional failures in your design.                */
/*                                                             */
/*=============================================================*/

/*-------------------------------------------------------------*/
/* SERDESIF_0 Initialization                                   */
/*-------------------------------------------------------------*/

#include <stdint.h>
#include "../../CMSIS/sys_init_cfg_types.h"

const cfg_addr_value_pair_t g_m2s_serdes_0_config[] = {
    { (uint32_t*)(0x40028000 + 0x2028), 0x10F } /* SYSTEM_CONFIG_PHY_MODE_1 */,
    { (uint32_t*)(0x40028000 + 0x1198), 0x30 } /* LANE0_PHY_RESET_OVERRIDE */,
    { (uint32_t*)(0x40028000 + 0x1000), 0x80 } /* LANE0_CR0 */,
    { (uint32_t*)(0x40028000 + 0x1004), 0x20 } /* LANE0_ERRCNT_DEC */,
    { (uint32_t*)(0x40028000 + 0x1008), 0xF8 } /* LANE0_RXIDLE_MAX_ERRCNT_THR */,
    { (uint32_t*)(0x40028000 + 0x100c), 0x80 } /* LANE0_IMPED_RATIO */,
    { (uint32_t*)(0x40028000 + 0x1014), 0x29 } /* LANE0_PLL_M_N */,
    { (uint32_t*)(0x40028000 + 0x1018), 0x20 } /* LANE0_CNT250NS_MAX */,
    { (uint32_t*)(0x40028000 + 0x1024), 0x80 } /* LANE0_TX_AMP_RATIO */,
    { (uint32_t*)(0x40028000 + 0x1028), 0x15 } /* LANE0_TX_PST_RATIO */,
    { (uint32_t*)(0x40028000 + 0x1030), 0x10 } /* LANE0_ENDCALIB_MAX */,
    { (uint32_t*)(0x40028000 + 0x1034), 0x38 } /* LANE0_CALIB_STABILITY_COUNT */,
    { (uint32_t*)(0x40028000 + 0x103c), 0x70 } /* LANE0_RX_OFFSET_COUNT */,
    { (uint32_t*)(0x40028000 + 0x11d4), 0x2 } /* LANE0_GEN1_TX_PLL_CCP */,
    { (uint32_t*)(0x40028000 + 0x11d8), 0x22 } /* LANE0_GEN1_RX_PLL_CCP */,
    { (uint32_t*)(0x40028000 + 0x1198), 0x0 } /* LANE0_PHY_RESET_OVERRIDE */,
    { (uint32_t*)(0x40028000 + 0x1200), 0x1 } /* LANE0_UPDATE_SETTINGS */,
    { (uint32_t*)(0x40028000 + 0x2028), 0xF0F } /* SYSTEM_CONFIG_PHY_MODE_1 */
};

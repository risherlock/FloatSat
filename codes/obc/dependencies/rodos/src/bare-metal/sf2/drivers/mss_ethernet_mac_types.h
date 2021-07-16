/*******************************************************************************
 * (c) Copyright 2011-2015 Microsemi SoC Products Group.  All rights reserved.
 * 
 * This file contains type definitions used throughout SmartFusion2 Ethernet MAC
 * and PHY device drivers. User need not include this file in application source
 * code.
 * Inclusion of mss_ethernet_mac.h inherits these types.
 * 
 * SVN $Revision: 7657 $
 * SVN $Date: 2015-08-13 17:49:00 +0530 (Thu, 13 Aug 2015) $
 */
#ifndef MSS_ETHERNET_MAC_TYPES_H_
#define MSS_ETHERNET_MAC_TYPES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************/
/* Public type definitions                                                     */
/*******************************************************************************/

/*******************************************************************************
 * MAC interface speed
 * This type definition provides various interface speeds supported by MAC hardware.
 */
typedef enum {
    MSS_MAC_10MBPS   = 0x00,
    MSS_MAC_100MBPS  = 0x01,
    MSS_MAC_1000MBPS = 0x02,
    INVALID_SPEED    = 0x03
} mss_mac_speed_t;

/*******************************************************************************
  SmartFusion2 Ethernet MAC Configuration Structure.
  The mss_mac_cfg_t type provides the prototype for the configuration values of the
  SmartFusion2 Ethernet MAC.You need to create a record of this type to hold the 
  configuration of the MAC. The MSS_MAC_cfg_struct_def_init() API should be used
  to initialize the configuration record to default values. Later, the 
  configuration elements in the record can be changed to desired values.
  
  speed_duplex_select
    The speed_duplex_select configuration parameter specifies the allowed link
    speeds. It is a bit-mask of the various link speed and duplex modes. The
    speed_duplex_select configuration can be set to a bitmask of the following
    defines to specify the allowed link speed and duplex mode:
        • MSS_MAC_ANEG_10M_FD
        • MSS_MAC_ANEG_10M_HD
        • MSS_MAC_ANEG_100M_FD
        • MSS_MAC_ANEG_100M_HD
        • MSS_MAC_ANEG_1000M_FD
        • MSS_MAC_ANEG_1000M_HD
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_ANEG_ALL_SPEEDS indicating that a link will be setup for best
    available speed and duplex combination.

  mac_addr
    The mac_addr configuration parameter is a 6-byte array containing the local
    MAC address of the Ethernet MAC.

  phy_address
    The phy_address parameter specifies the address of the PHY device, set in
    hardware by the address pins of the PHY device.

  tx_edc_enable 
    The tx_edc_enable parameter specifies enable or disable error detection and
    correction for tx FIFOs. The allowed values for the tx_edc_enable
    configuration parameter are:
        • MSS_MAC_ERR_DET_CORR_ENABLE 
        • MSS_MAC_ERR_DET_CORR_DISABLE 
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_ERR_DET_CORR_ENABLE.

  rx_edc_enable
    The rx_edc_enable parameter specifies enable or disable error detection and
    correction for rx FIFOs. The allowed values for the rx_edc_enable
    configuration parameter are:
        • MSS_MAC_ERR_DET_CORR_ENABLE 
        • MSS_MAC_ERR_DET_CORR_DISABLE 
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_ERR_DET_CORR_ENABLE.

  preamble_length
    The preamble_length parameter specifies the length of the preamble field of
    the packet in bytes. The allowed values for the preamble_length
    configuration parameter are: 
        • MSS_MAC_PREAMLEN_DEFVAL
        • MSS_MAC_PREAMLEN_MAXVAL
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_PREAMLEN_DEFVAL.

  hugeframe_enable
    The hugeframe_enable parameter specifies enable or disable huge frame support.
    When enabled, it allows frames longer than the maximum frame length to be
    transmitted and received. When disabled, the MAC limits the length of frames
    at the maximum frame length. The allowed values for the hugeframe_enable
    configuration parameter are: 
        • MSS_MAC_HUGE_FRAME_ENABLE
        • MSS_MAC_HUGE_FRAME_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_HUGE_FRAME_DISABLE.

  length_field_check
    The length_field_check parameter specifies enable or disable length filed
    check. When enabled, the MAC checks the frame’s length field to ensure it
    matches the actual data field length. The allowed values for the
    length_field_check configuration parameter are: 
        • MSS_MAC_LENGTH_FILED_CHECK_ENABLE 
        • MSS_MAC_LENGTH_FILED_CHECK_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_LENGTH_FILED_CHECK_ENABLE

  pad_n_CRC
    The pad_n_CRC parameter specifies enable or disable padding and appending CRC.
    When enabled, the MAC pads all the short frames and appends a CRC to every
    frame whether or not padding is required. When disabled, frames presented
    to the MAC, have a valid length and contain a CRC. The allowed values for
    the pad_n_CRC configuration parameter are: 
        • MSS_MAC_PAD_N_CRC_ENABLE 
        • MSS_MAC_PAD_N_CRC_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_PAD_N_CRC_ENABLE 

  append_CRC
    The append_CRC parameter specifies enable or disable appending CRC. When
    enabled, the MAC appends a CRC to all the frames. When disabled, frames
    presented to the MAC, have a valid length and contain a valid CRC. The
    allowed values for the append_CRC configuration parameter are: 
        • MSS_MAC_CRC_ENABLE 
        • MSS_MAC_CRC_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_CRC_ENABLE.

  fullduplex 
    The fullduplex parameter specifies enable or disable full duplex. When
    enabled, the MAC operates in full duplex mode. When disabled, the MAC
    operates in half duplex mode. The allowed values for the fullduplex
    configuration parameter are: 
        • MSS_MAC_FULLDUPLEX_ENABLE
        • MSS_MAC_FULLDUPLEX_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_FULLDUPLEX_ENABLE.

  loopback
    The loopback parameter specifies enable or disable loop back mode. When
    enabled, the MAC transmit outputs to be looped back to its receiving inputs.
    The allowed values for the loopback configuration parameter are: 
        • MSS_MAC_LOOPBACK_ENABLE 
        • MSS_MAC_LOOPBACK_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_LOOPBACK_DISABLE.

  rx_flow_ctrl
    The rx_flow_ctrl parameter specifies enable or disable receiver flow control.
    When enabled, the MAC detects and acts on PAUSE flow control frames. When
    disabled, it ignores PAUSE flow control frames. The allowed values for the
    rx_flow_ctrl configuration parameter are: 
        • MSS_MAC_RX_FLOW_CTRL_ENABLE 
        • MSS_MAC_RX_FLOW_CTRL_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_RX_FLOW_CTRL_ENABLE.

  tx_flow_ctrl
    The tx_flow_ctrl parameter specifies enable or disable transmitter flow
    control. When enabled, the transmitter sends PAUSE flow control frames when
    requested by the system. When disabled, prevents the transmitter from
    sending flow control frames. The allowed values for the tx_flow_ctrl
    configuration parameter are: 
        • MSS_MAC_TX_FLOW_CTRL_ENABLE 
        • MSS_MAC_TX_FLOW_CTRL_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_TX_FLOW_CTRL_ENABLE.

  min_IFG
    The min_IFG parameter specifies minimum size of management gap (IFG) to
    enforce between frames (expressed in bit times). The allowed values for
    the min_IFG configuration parameter are: 
        • MSS_MAC_MINIFG_DEFVAL 
        • MSS_MAC_MINIFG_MAXVAL
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_MINIFG_DEFVAL.

  btb_IFG
    The btb_IFG parameter specifies the Inter frame gap between back-to-back
    packets (expressed in bit times), used exclusively in full-duplex mode when
    two transmit packets are sent back-to-back. The allowed values for the
    btb_IFG configuration parameter are: 
        • MSS_MAC_BTBIFG_DEFVAL 
        • MSS_MAC_BTBIFG_MAXVAL
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_BTBIFG_DEFVAL.

  max_retx_tries
    The max_retx_tries parameter specifies the number of retransmission attempts
    following a collision before aborting the packet due to excessive collisions.
    The allowed values for the max_retx_tries configuration parameter are: 
        • MSS_MAC_MAXRETX_DEFVAL 
        • MSS_MAC_MAXRETX_MAXVAL
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_MAXRETX_DEFVAL.

  excessive_defer
    The excessive_defer parameter specifies enable or disable transmission of
    packet that exceeded max collisions. When enabled, the transmitter allows
    the transmission of a packet that has been excessively deferred. When
    disabled, the transmitter aborts the transmission of a packet that has
    been excessively deferred. The allowed values for the excessive_defer 
    configuration parameter are: 
        • MSS_MAC_EXSS_DEFER_ENABLE
        • MSS_MAC_EXSS_DEFER_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_EXSS_DEFER_DISABLE.

  nobackoff
    The nobackoff parameter specifies enable or disable back off. When enabled,
    the transmitter immediately re-transmits following a collision. When disabled,
    the transmitter follows the binary exponential backoff rule. The allowed
    values for the nobackoff configuration parameter are: 
        • MSS_MAC_NO_BACKOFF_ENABLE 
        • MSS_MAC_NO_BACKOFF_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_NO_BACKOFF_DISABLE.

  backpres_nobackoff
    The backpres_nobackoff parameter specifies enable or disable back off in back
    pressure mode. When enabled, the transmitter immediately re-transmits
    following a collision during back pressure operation. When disabled, the
    transmitter follows the binary exponential backoff rule. The allowed values
    for the backpres_nobackoff configuration parameter are: 
        • MSS_MAC_BACKPRESS_NO_BACKOFF_ENABLE 
        • MSS_MAC_BACKPRESS_NO_BACKOFF_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_BACKPRESS_NO_BACKOFF_DISABLE.

  ABEB_enable
    The ABEB_enable parameter specifies enable or disable arbitrary binary
    exponential back-off. When enabled, it configures the transmitter to use the
    ABEB_truncvalue value instead of the 802.3 standard tenth collisions. When
    disabled, it causes the transmitter to follow the 802.3 standard binary
    exponential backoff rule. The allowed values for the ABEB_enable
    configuration parameter are:
        • MSS_MAC_ABEB_ENABLE 
        • MSS_MAC_ABEB_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_ABEB_DISABLE.

  ABEB_truncvalue
    The ABEB_truncvalue parameter specifies alternative binary exponential
    back-off value. This value is used when the ABEB_enable parameter is enabled.
    The allowed values for the ABEB_truncvalue configuration parameter are: 
        • MSS_MAC_ABEBTRUNC_DEFVAL 
        • MSS_MAC_ABEBTRUNC_MAXVAL
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_ABEBTRUNC_DEFVAL.

  phyclk
    The phyclk parameter specifies MII management clock divider value. PCLK is
    the source clock. The allowed values for the phyclk configuration parameter
    are: 
        • MSS_MAC_DEF_PHY_CLK 
        • MSS_MAC_BY4_PHY_CLK 
        • MSS_MAC_BY6_PHY_CLK 
        • MSS_MAC_BY8_PHY_CLK 
        • MSS_MAC_BY10_PHY_CLK 
        • MSS_MAC_BY14_PHY_CLK 
        • MSS_MAC_BY20_PHY_CLK
        • MSS_MAC_BY28_PHY_CLK
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_DEF_PHY_CLK.

  supress_preamble
    The supress_preamble parameter specifies enable or disable preamble suppression
    at PHY. When enabled, MII Management suppresses preamble generation and
    reduces the Management cycle from 64 clocks to 32clocks. When disabled, MII
    Management performs Management read/write cycles with the 64 clocks of 
    preamble. The allowed values for the supress_preamble configuration parameter
    are: 
    • MSS_MAC_SUPPRESS_PREAMBLE_ENABLE 
    • MSS_MAC_SUPPRESS_PREAMBLE_DISABLE
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_SUPPRESS_PREAMBLE_DISABLE.

  max_frame_length
    The max_frame_length parameter specifies the maximum frame size in both the
    transmit and receive directions. The allowed values for the max_frame_length
    configuration parameter are: 
        • MSS_MAC_MAXFRAMELEN_DEFVAL 
        • MSS_MAC_MAXFRAMELEN_MAXVAL
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_MAXFRAMELEN_DEFVAL.

  non_btb_IFG
    The non_btb_IFG parameter specifies non back to back inter-frame gap value.
    The allowed values for the non_btb_IFG configuration parameter are: 
        • MSS_MAC_NONBTBIFG_DEFVAL 
        • MSS_MAC_NONBTBIFG_MAXVAL
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_NONBTBIFG_DEFVAL.

  slottime
    The slottime parameter specifies the slot time or collision window during
    which collisions might occur in a properly configured network. The allowed 
    values for the slottime configuration parameter are: 
        • MSS_MAC_SLOTTIME_DEFVAL 
        • MSS_MAC_SLOTTIME_MAXVAL
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_SLOTTIME_DEFVAL.

  framedrop_mask
    The framedrop_mask parameter specifies to drop frames based on receive
    statistics. The allowed values for the framedrop_mask configuration parameter
    are: 
        • MSS_MAC_FRAME_DROP_MASK_DEFVAL
    The MSS_MAC_cfg_struct_def_init() function sets this configuration parameter
    to MSS_MAC_FRAME_DROP_MASK_DEFVAL.
 */
typedef struct
{
    uint32_t speed_duplex_select; /* Link speed and duplex mode allowed to setup a link. */
    uint8_t  mac_addr[6];         /* Station's MAC address */
    uint8_t  phy_addr;            /* Address of Ethernet PHY on MII management interface. */
    uint8_t  tx_edc_enable;       /* Enable / disable error detection and correction for tx FIFOs */
    uint8_t  rx_edc_enable;       /* Enable / disable error detection and correction for rx FIFOs */
    uint8_t  preamble_length;     /* 4-bit Length of preamble field: default value is 0x7 */
    uint8_t  hugeframe_enable;    /* Enable / disable huge frame support: default is disable 0 */
    uint8_t  length_field_check;  /* Enable / disable length field checking */
    uint8_t  pad_n_CRC;           /* Enable / disable padding and appending CRC */
    uint8_t  append_CRC;          /* Enable / disable appending CRC */
    uint8_t  fullduplex;          /* Enable / disable full duplex: default is disable 0 */
    uint8_t  loopback;            /* Enable / disable loopback mode: default is disable 0 */
    uint8_t  rx_flow_ctrl;        /* Enable / disable receiver flow control: default is disable 0 */
    uint8_t  tx_flow_ctrl;        /* Enable / disable transmitter flow control: default is disable 0 */
    uint8_t  min_IFG;             /* 8-bit minimum inter-frame gap value */
    uint8_t  btb_IFG;             /* 7-bit back to back inter-frame gap value */
    uint8_t  max_retx_tries;      /* 5-bit maximum retransmission tries value: default is 0xF */
    uint8_t  excessive_defer;     /* Enable / disable transmission of packet that exceeded max collisions: default is disable 0 */
    uint8_t  nobackoff;           /* Enable / disable back off. default is disable 0 */
    uint8_t  backpres_nobackoff;  /* Enable / disable back off in back pressure mode: default is disable 0 */
    uint8_t  ABEB_enable;         /* Enable / disable arbitrary binary exponential back-off: default is disable 0 */
    uint8_t  ABEB_truncvalue;     /* 4-bit alternative binary exponential back-off value: default is 0xA */
    uint8_t  phyclk;              /* 3-bit MGMT clock divider value */
    uint8_t  supress_preamble;    /* Enable / disable preamble suppression at PHY: default is disable 0 */
    uint16_t max_frame_length;    /* Maximum frame length: default value is 0x0600(1536d) */
    uint16_t non_btb_IFG;         /* 14-bit non back to back inter-frame gap value */
    uint16_t slottime;            /* 10-bit collision window value : default is 0x37 */
    uint32_t framedrop_mask;      /* 18-bit mask to drop frames based on receive statistics */
} mss_mac_cfg_t;

/*******************************************************************************
 * Transmit and Receive statistics.
 * Statistic of below type, which is desired to be read is to be passed to the
 * function MSS_MAC_read_stat(). The width of returned statistic value is 
 * mentioned in the comment against the statistic.
 */
typedef enum {
    MSS_MAC_FRAME_CNT_64,             /* 18-bit */
    MSS_MAC_FRAME_CNT_127,            /* 18-bit */
    MSS_MAC_FRAME_CNT_255,            /* 18-bit */
    MSS_MAC_FRAME_CNT_511,            /* 18-bit */
    MSS_MAC_FRAME_CNT_1K,             /* 18-bit */
    MSS_MAC_FRAME_CNT_MAX,            /* 18-bit */
    MSS_MAC_FRAME_CNT_VLAN,           /* 18-bit */
    MSS_MAC_RX_BYTE_CNT,              /* 24-bit */
    MSS_MAC_RX_PKT_CNT,               /* 18-bit */
    MSS_MAC_RX_FCS_ERR_CNT,           /* 12-bit */
    MSS_MAC_RX_MULTICAST_PKT_CNT,     /* 18-bit */
    MSS_MAC_RX_BROADCAST_PKT_CNT,     /* 22-bit */
    MSS_MAC_RX_CTRL_PKT_CNT,          /* 18-bit */
    MSS_MAC_RX_PAUSE_PKT_CNT,         /* 12-bit */
    MSS_MAC_RX_UNKNOWN_OPCODE_CNT,    /* 12-bit */
    MSS_MAC_RX_ALIGN_ERR_CNT,         /* 12-bit */
    MSS_MAC_RX_FRAMELENGTH_ERR_CNT,   /* 16-bit */
    MSS_MAC_RX_CODE_ERR_CNT,          /* 12-bit */
    MSS_MAC_RX_CS_ERR_CNT,            /* 12-bit */
    MSS_MAC_RX_UNDERSIZE_PKT_CNT,     /* 12-bit */
    MSS_MAC_RX_OVERSIZE_PKT_CNT,      /* 12-bit */
    MSS_MAC_RX_FRAGMENT_CNT,          /* 12-bit */
    MSS_MAC_RX_JABBER_CNT,            /* 12-bit */
    MSS_MAC_RX_DROP_CNT,              /* 12-bit */
    MSS_MAC_TX_BYTE_CNT,              /* 24-bit */
    MSS_MAC_TX_PKT_CNT,               /* 18-bit */
    MSS_MAC_TX_MULTICAST_PKT_CNT,     /* 18-bit */
    MSS_MAC_TX_BROADCAST_PKT_CNT,     /* 18-bit */
    MSS_MAC_TX_PAUSE_PKT_CNT,         /* 12-bit */
    MSS_MAC_TX_DEFFERAL_PKT_CNT,      /* 12-bit */
    MSS_MAC_TX_EXCS_DEFFERAL_PKT_CNT, /* 12-bit */
    MSS_MAC_TX_SINGLE_COLL_PKT_CNT,   /* 12-bit */
    MSS_MAC_TX_MULTI_COLL_PKT_CNT,    /* 12-bit */
    MSS_MAC_TX_LATE_COLL_PKT_CNT,     /* 12-bit */
    MSS_MAC_TX_EXCSS_COLL_PKT_CNT,    /* 12-bit */
    MSS_MAC_TX_TOTAL_COLL_PKT_CNT,    /* 13-bit */
    MSS_MAC_TX_PAUSE_HONORED_CNT,     /* 12-bit */
    MSS_MAC_TX_DROP_CNT,              /* 12-bit */
    MSS_MAC_TX_JABBER_CNT,            /* 12-bit */
    MSS_MAC_TX_FCS_ERR_CNT,           /* 12-bit */
    MSS_MAC_TX_CNTRL_PKT_CNT,         /* 12-bit */
    MSS_MAC_TX_OVERSIZE_PKT_CNT,      /* 12-bit */
    MSS_MAC_TX_UNDERSIZE_PKT_CNT,     /* 12-bit */
    MSS_MAC_TX_FRAGMENT_CNT,          /* 12-bit */
    MSS_MAC_LAST_STAT
} mss_mac_stat_t;

/*******************************************************************************
 * 
 */
typedef void (*mss_mac_transmit_callback_t)(void* p_user_data);

/*******************************************************************************
 * 
 */
typedef void (*mss_mac_receive_callback_t)(uint8_t* p_rx_packet,
                                           uint32_t pckt_length,
                                           void*    p_user_data);


#ifdef __cplusplus
}
#endif

#endif /* MSS_ETHERNET_MAC_TYPES_H_ */

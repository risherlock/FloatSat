#ifndef _LSM9DS0_SETTINGS_H_
#define _LSM9DS0_SETTINGS_H_

#include <math.h>

// LSM9DS0 register maps
#define LSM9DS0_ADDR_G 0x6B
#define LSM9DS0_ADDR_XM 0x1D
#define LSM9DS0_WHO_AM_I_XM 0x0F
#define LSM9DS0_WHO_AM_I_G 0x0F

#define LSM9DS0_CTRL_REG1_G 0x20
#define LSM9DS0_CTRL_REG2_G 0x21
#define LSM9DS0_CTRL_REG3_G 0x22
#define LSM9DS0_CTRL_REG4_G 0x23
#define LSM9DS0_CTRL_REG5_G 0x24

#define LSM9DS0_OUT_X_L_G 0x28
#define LSM9DS0_OUT_X_H_G 0x29
#define LSM9DS0_OUT_Y_L_G 0x2A
#define LSM9DS0_OUT_Y_H_G 0x2B
#define LSM9DS0_OUT_Z_L_G 0x2C
#define LSM9DS0_OUT_Z_H_G 0x2D

#define LSM9DS0_CTRL_REG0_XM 0x1F
#define LSM9DS0_CTRL_REG1_XM 0x20
#define LSM9DS0_CTRL_REG2_XM 0x21
#define LSM9DS0_CTRL_REG3_XM 0x22
#define LSM9DS0_CTRL_REG4_XM 0x23
#define LSM9DS0_CTRL_REG5_XM 0x24
#define LSM9DS0_CTRL_REG6_XM 0x25
#define LSM9DS0_CTRL_REG7_XM 0x26

#define LSM9DS0_OUT_X_L_A 0x28
#define LSM9DS0_OUT_X_H_A 0x29
#define LSM9DS0_OUT_Y_L_A 0x2A
#define LSM9DS0_OUT_Y_H_A 0x2B
#define LSM9DS0_OUT_Z_L_A 0x2C
#define LSM9DS0_OUT_Z_H_A 0x2D

#define LSM9DS0_OUT_X_L_M 0x08
#define LSM9DS0_OUT_X_H_M 0x09
#define LSM9DS0_OUT_Y_L_M 0x0A
#define LSM9DS0_OUT_Y_H_M 0x0B
#define LSM9DS0_OUT_Z_L_M 0x0C
#define LSM9DS0_OUT_Z_H_M 0x0D

#define LSM9DS0_OFFSET_X_REG_L_M 0x16
#define LSM9DS0_OFFSET_X_REG_H_M 0x17
#define LSM9DS0_OFFSET_Y_REG_L_M 0x18
#define LSM9DS0_OFFSET_Y_REG_H_M 0x19
#define LSM9DS0_OFFSET_Z_REG_L_M 0x1A
#define LSM9DS0_OFFSET_Z_REG_H_M 0x1B

// Linear Acceleration: milli g's per LSB
#define LSM9DS0_ACCEL_mg_LSB_2G 0.061F  // +-2g
#define LSM9DS0_ACCEL_mg_LSB_4G 0.122F  // +-4g
#define LSM9DS0_ACCEL_mg_LSB_6G 0.183F  // +-6g
#define LSM9DS0_ACCEL_mg_LSB_8G 0.244F  // +-8g
#define LSM9DS0_ACCEL_mg_LSB_16G 0.732F // +-16g
#define LSM9DS0_GRAVITY_EARTH 9.80665F  // m/s^2

// Gyroscope omega: dps per LSB
#define LSM9DS0_GYRO_DPS_DIGIT_245DPS 0.00875F  // +-245dps
#define LSM9DS0_GYRO_DPS_DIGIT_500DPS 0.01750F  // +-500dps
#define LSM9DS0_GYRO_DPS_DIGIT_2000DPS 0.07000F // +-2000dps

// Magnetic field strength: milli Gauss per LSB
#define LSM9DS0_MAG_mG_2GAUSS 0.08F  // +-2 gauss
#define LSM9DS0_MAG_mG_4GAUSS 0.16F  // +-4 gauss
#define LSM9DS0_MAG_mG_8GAUSS 0.32F  // +-8 gauss
#define LSM9DS0_MAG_mG_12GAUSS 0.48F // +-12 gauss

// Temperature: LSB per degree celsius
#define LSM9DS0_TEMP_LSB_DEGREE_CELSIUS 8

// I2C->writeRead() requires the pointer to the address
const uint8_t lsm8ds1_who_am_i[1] = {LSM9DS0_WHO_AM_I_G};
const uint8_t lsm8ds1_out_x_l_g[1] = {0x80 | LSM9DS0_OUT_X_L_G};
const uint8_t lsm8ds1_out_x_l_xl[1] = {0x80 | LSM9DS0_OUT_X_L_A};
const uint8_t lsm8ds1_out_x_l_m[1] = {0x80 | LSM9DS0_OUT_X_L_M};

/*
                    Configure IMU registers
    Please refer datasheet to  
*/

// Change the factors based on fullscale
#define ACCEL_g_PER_LSB     LSM9DS0_ACCEL_mg_LSB_2G / 1000.0F
#define GYRO_RPS_PER_LSB    LSM9DS0_GYRO_DPS_DIGIT_2000DPS *(M_PI / 180.0F)
#define MAG_uT_PER_LSB      LSM9DS0_MAG_mG_2GAUSS / 10.0

/*  ~ CTRL_REG1_G ~
   DR[1,0] - Output data rate: 190Hz
   BW[1,0] - Bandwidth: 12.5Hz
   PD - Power-down mode: Normal mode
   Zen - Z-axis enable: Enabled
   Yen - Y-axis enable: Enabled
   Xen - X-axis enable: Enabled

*/
static uint8_t ctrl_reg1_g_setting[2] = {LSM9DS0_CTRL_REG1_G, 0b01001111};

/*  ~ CTRL_REG2_G ~
    HPM[1,0] - High-pass filter mode: Normal mode
    HPC[3,2,1,0] - High-pass filter cutoff frequency: 13.5Hz (for 190Hz ODR)
*/
static uint8_t ctrl_reg2_g_setting[2] = {LSM9DS0_CTRL_REG2_G, 0b00000000};

/*  ~ CTRL_REG3_G ~
    I1_Int1 - Interrupt enable on INT_G pin: Disable
    I1_Boot - Boot status available on INT_G: Disable
    H_Lactive - Interrupt active configuration on INT_G: High
    PP_OD - Push-pull / Open drain: Push pull
    I2_DRDY - Date-ready on DRDY_G: Disable
    I2_WTM - FIFO watermark interrupt on DRDY_G: Disable
    I2_ORun - FIFO overrun interrupt on DRDY_G: Disable
    I2_Empty - IFO empty interrupt on DRDY_G: Disable
*/
static uint8_t ctrl_reg3_g_setting[2] = {LSM9DS0_CTRL_REG3_G, 0b00000000};

/*  ~ CTRL_REG4_G ~
    BDU - Block data update: Default
    BLE - Big/little endian data selection: LSB at lower address
    FS[1,0] - Full-scale: 2000dps (degrees per second)
    ST[1,0] - Self-test: Disabled
    SIM - SPI serial interface mode: 4-wire interface
*/
static uint8_t ctrl_reg4_g_setting[2] = {LSM9DS0_CTRL_REG4_G, 0b00100000};

/*  ~ CTRL_REG5_G ~
    BOOT - Reboot memory content: Default
    FIFO_EN - FIFO enable: Default
    HPen - High-pass filter: Default
    INT1_Sel[1,0] - INT1 selection: Default
    Out_Sel[1,0] - Out selection: Default
*/
static uint8_t ctrl_reg5_g_setting[2] = {LSM9DS0_CTRL_REG5_G, 0b00000000};

/*  ~ CTRL_REG0_XM ~
    BOOT - Reboot memory content: Normal mode
    FIFO_EN - FIFO enable: FIFO disable
    WTM_EN - FIFO programmable watermark enable: Disabled
    HP_Click - High-pass filter enabled for Click function: Bypassed
    HPIS1 - High-pass filter enabled for interrupt generator 1: Bypassed
    HPIS2 - High-pass filter enabled for interrupt generator 2: Bypassed
*/
static const uint8_t ctrl_reg0_xm_setting[2] = {LSM9DS0_CTRL_REG0_XM, 0b00000000};

/*  ~ CTRL_REG1_XM ~
    AODR[3,2,1,0] - Acceleration data rate: 200Hz
    BDU - Block data update for accel and mag data: Continuous update
    AZEN - Accel Z-axis enable: Enabled
    AYEN - Accel Y-axis enable: Enabled
    AXEN - Accel X-axis enable: Enabled
*/
static const uint8_t ctrl_reg1_xm_setting[2] = {LSM9DS0_CTRL_REG1_XM, 0b01110111};

/*  ~ CTRL_REG2_XM ~
    ABW[1,0] - Accel anti-alias filter bandwidth: 50Hz
    AFS[2,1,0] - Accel full-scale: +-2g
    AST[1,0] - Accel self-test: Disabled
    SIM - SPI Serial Interface Mode: 4-wire interface
*/
static const uint8_t ctrl_reg2_xm_setting[2] = {LSM9DS0_CTRL_REG2_XM, 0b11000000};

/* ~ CTRL_REG3_XM ~
    P1_BOOT - Boot on INT1_XM pin enable: Default
    P1_TAP - Tap generator interrupt on INT1_XM pin: Default
    P1_INT1 - Inertial interrupt generator 1 on INT1_XM pi: Default
    P1_INT2 - Inertial interrupt generator 2 on INT1_XM pin: Default
    P1_INTM - Magnetic interrupt generator on INT1_XM pin: Default
    P1_DRDYA - Accelerometer data-ready signal on INT1_XM pin: Default
    P1_DRDYM - Magnetometer data-ready signal on INT1_XM pin: Default
    P1_EMPTY - FIFO empty indication on INT1_XM pin: Default
*/
static const uint8_t ctrl_reg3_xm_setting[2] = {LSM9DS0_CTRL_REG3_XM, 0b00000000};

/* ~ CTRL_REG4_XM ~
    P2_TAP - Tap generator interrupt on INT2_XM pin: Default
    P2_INT1 - Inertial interrupt generator 1 on INT2_XM pin: Default
    P2_INT2 - Inertial interrupt generator 2 on INT2_XM pin: Default
    P2_INTM - Magnetic interrupt generator on INT2_XM pin: Default
    P2_DRDYA - Accelerometer data-ready signal on INT2_XM pin: Default
    P2_DRDYM - Magnetometer data-ready signal on INT2_XM pin: Default
    P2_Overrun - FIFO overrun interrupt on INT2_XM pin: Default
    P2_WTM - FIFO watermark interrupt on INT2_XM pin: Default
*/
static const uint8_t ctrl_reg4_xm_setting[2] = {LSM9DS0_CTRL_REG4_XM, 0b00000000};

/*  ~ CTRL_REG5_XM ~
    TEMP_EN - Temperature sensor enable: Enabled
    M_RES[1,0] - Magnetic resolution selection: Low resolution
    M_ODR[2,1,0] - Magnetic data rate selection: 100Hz
    LIR2 - Latch interrupt request on INT2_SRC register: 0
    LIR1 - Latch interrupt request on INT21SRC register: 0
*/
static const uint8_t ctrl_reg5_xm_setting[2] = {LSM9DS0_CTRL_REG5_XM, 0b10010100};

/*  ~ CTRL_REG6_XM ~
    MFS[1,0] - Magnetic full-scale selection: +-2gauss
*/
static const uint8_t ctrl_reg6_xm_setting[2] = {LSM9DS0_CTRL_REG6_XM, 0b00000000};

/*  ~ CTRL_REG7_XM ~
    AHPM[1,0] - High-pass filter mode selection for accel data: Normal mode
    AFDS - Filtered accel data selection: Internal filter bypassed
    MLP - Magnetic data low-power mode: Default
    MD[1,0] - Magnetic sensor mode selection: Continuous-conversion mode
*/
static const uint8_t ctrl_reg7_xm_setting[2] = {LSM9DS0_CTRL_REG7_XM, 0b00000000};

#endif // lsM9ds0_settings.h
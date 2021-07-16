#ifndef _LSM9DS0_H_
#define _LSM9DS0_H_

#include "rodos.h"
#include "hal.h"

extern volatile struct SensorVector
{
  float X = 0.0;
  float Y = 0.0;
  float Z = 0.0;
} acc, gyro, mag;

extern HAL_GPIO pin_cs_ag;
extern HAL_GPIO pin_cs_m;
extern HAL_GPIO pin_en_imu;

extern bool initIMU();
extern bool checkComm();

extern bool initGyro();
extern bool initAccelMag();

extern bool readGyro(); // rad/s
extern bool readMag();  // uT
extern bool readAcc();  // g's
extern float getMagNorm();

extern void setMagCalParams(SensorVector offset, float matrix[3][3]);
extern void setGyroCalParams(SensorVector offset);

#endif // lsm9ds1.h
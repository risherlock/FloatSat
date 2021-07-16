// TODO
// 1. Tilt compensation heading
// 2. Motor and encoder

#include "lsm9ds0.h"
#include "lsm9ds0_settings.h"

HAL_I2C IMU(I2C_IDX2);

volatile SensorVector acc, gyro, mag;

// Vectors and matrix for calibrations
static SensorVector gyro_offset, mag_offset;
static float mag_M[3][3] = {{1.0F, 0, 0}, {0, 1.0F, 0}, {0, 0, 1.0F}};

HAL_GPIO pin_en_imu(GPIO_055); // PD7
HAL_GPIO pin_cs_ag(GPIO_018);  // PB2
HAL_GPIO pin_cs_m(GPIO_032);   // PC0

bool initIMU()
{
	pin_en_imu.init(true, 1, 1);
	pin_cs_ag.init(true, 1, 1);
	pin_cs_m.init(true, 1, 1);

	IMU.init(400000);
	bool status_comm = checkComm();
	bool status_gyro = initGyro();
	bool status_accel_mag = initAccelMag();

	return (status_comm & status_accel_mag & status_gyro);
}

bool checkComm()
{
	uint8_t rx_gyro_id[1] = {0};
	uint8_t rx_accel_mag_id[1] = {0};

	int status_gyro = IMU.writeRead(LSM9DS0_ADDR_G, lsm8ds1_who_am_i, 1, rx_gyro_id, 1);
	int status_mag = IMU.writeRead(LSM9DS0_ADDR_XM, lsm8ds1_who_am_i, 1, rx_accel_mag_id, 1);

	if ((rx_gyro_id[0] != 0xD4) || (rx_accel_mag_id[0] != 0x49))
	{
		PRINTF("Gyro_id = %x with I2C_status = %d (Expected = 0xD4, 1)\r\n", rx_gyro_id[0], status_gyro);		   // 11010100 -> 0xD4
		PRINTF("Accel_mag_id = %x with I2C_status = %d (Expected = 0x49, 1)\r\n", rx_accel_mag_id[0], status_mag); // 01001001 -> 0x49
	}

	return !(status_gyro < 0) && !(status_mag < 0) &&
		   (rx_gyro_id[0] == 0xD4) && (rx_accel_mag_id[0] == 0x49);
}

bool initGyro()
{
	int status_1 = IMU.write(LSM9DS0_ADDR_G, ctrl_reg1_g_setting, 2);
	int status_2 = IMU.write(LSM9DS0_ADDR_G, ctrl_reg2_g_setting, 2);
	int status_3 = IMU.write(LSM9DS0_ADDR_G, ctrl_reg3_g_setting, 2);
	int status_4 = IMU.write(LSM9DS0_ADDR_G, ctrl_reg4_g_setting, 2);
	int status_5 = IMU.write(LSM9DS0_ADDR_G, ctrl_reg5_g_setting, 2);

	return !(status_1 < 0) && !(status_2 < 0) && !(status_3 < 0) &&
		   !(status_4 < 0) && !(status_5 < 0);
}

bool initAccelMag()
{
	int status_0 = IMU.write(LSM9DS0_ADDR_XM, ctrl_reg0_xm_setting, 2);
	int status_1 = IMU.write(LSM9DS0_ADDR_XM, ctrl_reg1_xm_setting, 2);
	int status_2 = IMU.write(LSM9DS0_ADDR_XM, ctrl_reg2_xm_setting, 2);
	int status_3 = IMU.write(LSM9DS0_ADDR_XM, ctrl_reg3_xm_setting, 2);
	int status_4 = IMU.write(LSM9DS0_ADDR_XM, ctrl_reg4_xm_setting, 2);
	int status_5 = IMU.write(LSM9DS0_ADDR_XM, ctrl_reg5_xm_setting, 2);
	int status_6 = IMU.write(LSM9DS0_ADDR_XM, ctrl_reg6_xm_setting, 2);
	int status_7 = IMU.write(LSM9DS0_ADDR_XM, ctrl_reg7_xm_setting, 2);

	return !(status_0 < 0) && !(status_1 < 0) && !(status_2 < 0) &&
		   !(status_3 < 0) && !(status_4 < 0) && !(status_5 < 0) &&
		   !(status_6 < 0) && !(status_7 < 0);
}

bool readAcc()
{
	uint8_t data[6];
	int status = IMU.writeRead(LSM9DS0_ADDR_XM, lsm8ds1_out_x_l_xl, 1, data, 6);

	// Read from registers
	acc.X = (int16_t)(data[1] << 8) | data[0];
	acc.Y = (int16_t)(data[3] << 8) | data[2];
	acc.Z = (int16_t)(data[5] << 8) | data[4];

	// Acceleration in g's
	acc.X *= ACCEL_g_PER_LSB;
	acc.Y *= ACCEL_g_PER_LSB;
	acc.Z *= ACCEL_g_PER_LSB;

	return !(status < 0);
}

bool readGyro()
{
	uint8_t data[6] = {0, 0, 0, 0, 0, 0};
	int status = IMU.writeRead(LSM9DS0_ADDR_G, lsm8ds1_out_x_l_g, 1, data, 6);

	// Read from registers
	gyro.X = (int16_t)(data[1] << 8) | data[0];
	gyro.Y = (int16_t)(data[3] << 8) | data[2];
	gyro.Z = (int16_t)(data[5] << 8) | data[4];

	// Change to radian per seconds
	gyro.X *= GYRO_RPS_PER_LSB;
	gyro.Y *= GYRO_RPS_PER_LSB;
	gyro.Z *= GYRO_RPS_PER_LSB;

	return !(status < 0);
}

bool readMag()
{
	uint8_t data[6];
	int status = IMU.writeRead(LSM9DS0_ADDR_XM, lsm8ds1_out_x_l_m, 1, data, 6);

	// Read from registers
	mag.X = (int16_t)(data[1] << 8) | data[0];
	mag.Y = (int16_t)(data[3] << 8) | data[2];
	mag.Z = (int16_t)(data[5] << 8) | data[4];

	// Change to mini Gauss
	mag.X *= MAG_uT_PER_LSB;
	mag.Y *= MAG_uT_PER_LSB;
	mag.Z *= MAG_uT_PER_LSB;

	// Hard iron: mag_measured - mag_offset
	float offset_corr[3];
	offset_corr[0] = mag.X - mag_offset.X;
	offset_corr[1] = mag.Y - mag_offset.Y;
	offset_corr[2] = mag.Z - mag_offset.Z;

	// Soft iron: mag_M * (mag_measured - mag_offset)
	float cal[3];
	for (uint8_t r = 0; r < 3; r++)
	{
		float elem = 0.0;
		for (size_t c = 0; c < 3; c++)
		{
			elem += mag_M[r][c] * offset_corr[c];
		}
		cal[r] = elem;
	}

	mag.X = cal[0];
	mag.Y = cal[1];
	mag.Z = cal[2];

	return !(status < 0);
}

float getMagNorm()
{
	return sqrt(mag.X * mag.X + mag.Y * mag.Y + mag.Z * mag.Z);
}

void setMagCalParams(SensorVector offset, float matrix[3][3])
{
	mag_offset.X = offset.X;
	mag_offset.Y = offset.Y;
	mag_offset.Z = offset.Z;

	for (uint8_t r = 0; r < 3; r++)
	{
		for (uint8_t c = 0; c < 3; c++)
		{
			mag_M[r][c] = matrix[r][c];
		}
	}
}

void setGyroCalParams(SensorVector offset)
{
	gyro_offset.X = offset.X;
	gyro_offset.Y = offset.Y;
	gyro_offset.Z = offset.Z;
}
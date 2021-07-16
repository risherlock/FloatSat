#include "lsm9ds0.h"
#include "rodos.h"

HAL_GPIO pin_dc_dc(GPIO_066); // PE2

class ImuThread : public StaticThread<>
{
public:
	void setup();
	void run()
	{
		setup();
		while (1)
		{
			suspendCallerUntil(NOW() + 100 * MILLISECONDS);

			bool status_acc = readAcc();
			bool status_gyr = readGyro();
			bool status_mag = readMag();
			float mag_norm = getMagNorm();

			if (status_acc && status_gyr && status_mag)
			{
				PRINTF("aX:%f,\taY:%f,\taZ:%f,\tgX:%f,\tgY:%f,\tgZ:%f,\tmX:%f,\tmY:%f,\tmZ:%f,\tmNorm:%f\r\n",
					   acc.X, acc.Y, acc.Z, gyro.X, gyro.Y, gyro.Z, mag.X, mag.Y, mag.Z, mag_norm);
			}
			else
			{
				PRINTF("IMU error !! \r\n");
			}
		}
	}
} imu_thread;

void ImuThread::setup()
{
	pin_dc_dc.init(true, 1, 1);

	float mag_Mat[3][3] = {{1.0F, 0, 0}, {0, 1.0F, 0}, {0, 0, 1.0F}};

	SensorVector mag_offset, gyro_offset;
	mag_offset.X = 0.0;
	mag_offset.Y = 0.0;
	mag_offset.Z = 0.0;

	gyro_offset.X = 0.0;
	gyro_offset.Y = 0.0;
	gyro_offset.Z = 0.0;

	setGyroCalParams(gyro_offset);
	setMagCalParams(mag_offset, mag_Mat);

	if (initIMU())
	{
		PRINTF("Successful IMU initialization !!\n");
	}
	else
	{
		PRINTF("IMU initialization error !!\n");
		while (1)
		{
		};
	}
}
#ifndef CHR_SENSORDATA_H
#define CHR_SENSORDATA_H 1

#include<string.h>

typedef unsigned short mask_16;

/* Enumerators for enable/disable channels */
enum Channels{az = 1, ay, ax, gz, gy, gx, mz, my, mx,
	      roll_rate, pitch_rate, yaw_rate, roll, pitch, yaw};

/* Enumerator for enable/disable a sensor set */
enum ChannelGroups{Accelerometer, RateGyro, Magnetometer,
	           EulerRate, EulerAngle};

/* Packet class */
class CHR_SensorData {
public:
	CHR_SensorData();

	/* Functions for enable/disable channels */
	mask_16 enableChannel(Channels);
	mask_16 disableChannel(Channels);
	mask_16 enableChannel(ChannelGroups);
	mask_16 disableChannel(ChannelGroups);
	
	/* Functions for testing the data */
	bool containedChannel(Channels);
	bool containedChannel(ChannelGroups);

	/* Get the channel mask */
	mask_16 getChannelMask();

	/* Data Field (Raw data, not calibrated yet) */
	mask_16 accelerometer[3];
	mask_16 rategyro[3];
	mask_16 magnetometer[3];
	mask_16 eulerrate[3];
	mask_16 eulerangle[3];

private:
	/* Channel setting */
	mask_16 channel_mask;
};

#endif

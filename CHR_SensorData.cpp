#include"CHR_SensorData.h"

CHR_SensorData::CHR_SensorData()
{
	memset(accelerometer, 0, 6);
	memset(rategyro, 0, 6);
	memset(magnetometer, 0, 6);
	memset(eulerrate, 0, 6);
	memset(eulerangle, 0, 6);

	/* Default Channel : Euler Angle only */
	channel_mask = 0xE000;
}

mask_16 CHR_SensorData::enableChannel(Channels channel)
{
	channel_mask |= (1 << channel);
	return channel_mask;
}

mask_16 CHR_SensorData::disableChannel(Channels channel)
{
	channel_mask &= ~(1 << channel);
	return channel_mask;
}

mask_16 CHR_SensorData::enableChannel(ChannelGroups group)
{
	channel_mask |= (7 << (1 + group * 3));
	return channel_mask;
}
mask_16 CHR_SensorData::disableChannel(ChannelGroups group)
{
	channel_mask &= ~(7 << (1 + group * 3));
	return channel_mask;
}

bool CHR_SensorData::containedChannel(Channels channel)
{
	return (channel_mask & (1 << channel));
}

bool CHR_SensorData::containedChannel(ChannelGroups group)
{
	mask_16 mask = 7 << (1 + group * 3);
	return ((channel_mask & mask) == mask);
}

mask_16 CHR_SensorData::getChannelMask()
{
	return channel_mask;
}


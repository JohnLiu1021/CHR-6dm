#include "CHR_6dm.h"

CHR-6dm::CHR-6dm(const char *path) : SerialPort(path)
{
	_shared.mutex = PTHREAD_MUTEX_INITIALIZER;
	memset(_shared.TXData, 0, 100);
	memset(_shared.RXData, 0, 100);
	_shared.updated = false;
	_shared.oneshot = true;
	_data_packet = NULL;
}


CHR-6dm::~CHR-6dm();

RValue CHR-6dm::open(int Timeout)
{
	_timeout_tv.tv_sec = Timeout / 1000;
	_timeout_tv.tv_usec = (Timeout - (_timeout.tv_sec * 1000)) * 1000;
	OpenPort();
	SetAttr(115200, 1, 100);
	_shared.fd = GetFD();
	return gotoConfigMode();
}

/*
   This method will resets all AHRS settings to factory default
   value.
*/
RValue CHR-6dm::resetToFactory();

/*
   This method simply set the device into SLIENT_MODE, not that
   there really exist a config mode in the device. However, if
   user wants to change any settings of the device, they have 
   to make sure the device is in a so called "config mode". 
   I think this is a better way to prevent accidentally option
   changing.
*/
RValue CHR-6dm::gotoConfigMode()
{



/*
   Set active channels in the device. the class CHR_SensorData
   is used to preserve the settings, thus it's passed by 
   pointer.
*/
RValue CHR-6dm::setActiveChannels(CHR_SensorData *);

/*
   This is how you can get the data in SLIENT_MODE.
*/
RValue CHR-6dm::getData(CHR_SensorData *);

/*
   Set the device into BROADCAST_MODE, which transmits the data
   automatically. The second argument is the samping rate.
*/
RValue CHR-6dm::gotoMeasurementMode(CHR_SensorData *, int);

/*
   This method will block until the data received by the driver
   from the device is updated.
*/
RValue CHR-6dm::isUpdated(UpdateMode mode)
{
	if (!_measurement_mode)
		return CHR_ErrorCommand;

	gettimeofday(&_time_tv, NULL);
	_timeout_tv.tv_usec += _time_tv.tv_usec;
	if (_timeout_tv.tv_usec >= 1000000) {
		_timeout_tv.tv_usec -= 1000000;
		_timeout_tv.tv_sec += 1;
	}
	_timeout_tv.tv_sec += _time_tv.tv_sec;

	do {
		gettimeofday(&_curr_time, NULL);
		if (_time_tv.tv_usec >= _timeout_tv.tv_usec 
		    && _time_tv.tv_sec >= _timeout_tv.tv_sec) {
			break;
		}

		pthread_mutex_lock(&_shared.mutex);
		if (_shared.updated) {
			pthread_mutex_unlock(&_shared.mutex);
			return CHR_OK;
		}
		pthread_mutex_unlock(&_shared.mutex);

	} while (mode == BLOCKING);

	return CHR_Timeout;
}

static void *CHR-6dm::_communicator(void *ptr)
{
	struct SharedData *s = (struct SharedData *)ptr;
	byte buffer[100];
	int rd, wr;

	if (!s->oneshot) {
		pthread_cleanup_push(_cleanup_function, ptr);
	}

	do {
		if (s->oneshot) // Transmition is needed
			wr = write(s->fd, s->TXData, s->TXNum);

		rd = read(s->fd, buffer, s->RXNum);
		if (CHR::verifyPacket(buffer)) {
			pthread_mutex_lock(&s->mutex);
			s->updated = true;
			memcpy(s->RXData, buffer, s->RXNum);
			pthread_mutex_unlock(&s->mutex);
		}
	} while(!s->oneshot);

	if (!s->oneshot) {
		pthread_cleanup_pop(0);
	}

}

static void CHR-6dm::_cleanup_function(void *ptr)
{
	struct SharedData *s = (struct SharedData *)ptr;
	pthread_mutex_unlock(&s->mutex);
}
	
		



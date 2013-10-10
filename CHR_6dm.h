#ifndef CHR_6DM_H
#define CHR_6DM_H 1

#include "SerialPort_cpp.h"
#include "CHR_SensorData.h"

enum RValue{
	CHR_OK,
	CHR_Timeout,
	CHR_ErrorCommand
};

typedef unsigned char byte;

struct DataOptions {
	pthread_mutex_t mutex;
	byte TXData[100];
	byte RXData[100];
	bool updated;
	bool validation;
}

class CHR-6dm : public SerialPort {
public:
	CHR-6dm();
	CHR-6dm(const char *);
	CHR-6dm(string);
	~CHR-6dm();

	/* 
	   When invoking the open method for the first time, this driver
	   will send a SELF_TEST command to the device to ensure the
	   device is correctly connected. After receiving the response
	   data, it will also set the device into SILENT_MODE. Thus
	   users must invoke the gotoMeasurementMode method in order to 
	   make the device sending sensor package automatically.
	*/
	RValue open(int);
	RValue open(const char *, int);
	RValue open(string, int);

	/*
	   This method will resets all AHRS settings to factory default
	   value.
	*/
	RValue resetToFactory();

	/*
	   This method simply set the device into SLIENT_MODE, not that
	   there really exist a config mode in the device. However, if
	   user wants to change any settings of the device, they have 
	   to make sure the device is in a so called "config mode". 
	   I think this is a better way to prevent accidentally option
	   changing.
	*/
	RValue gotoConfigMode();

	/*
	   Set active channels in the device. the class CHR_SensorData
	   is used to preserve the settings, thus it's passed by 
	   pointer.
	*/
	RValue setActiveChannels(CHR_SensorData *);

	/*
	   This is how you can get the data in SLIENT_MODE.
	*/
	RValue getData(CHR_SensorData *);

	/*
	   Set the device into BROADCAST_MODE, which transmits the data
	   automatically. The second argument is the samping rate.
	*/
	RValue gotoMeasurementMode(CHR_SensorData *, int);

	/*
	   This method will block until the data received by the driver
	   from the device is updated.
	*/
	RValue isUpdated(CHR_SensorData *);

private:
	/*
	   Unique thread for transmitting and receiving data from serial
	   port.
	*/
	static void *communication(void *ptr);
	struct DataOptions options;
	




	









	




#endif

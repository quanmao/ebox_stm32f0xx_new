// 
//    FILE: dht11.h
// VERSION: 0.4.1
// PURPOSE: DHT11 Temperature & Humidity Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
//
//     URL: http://playground.arduino.cc/Main/DHT11Lib
//
// HISTORY:
// George Hadjikyriacou - Original version
// see dht.cpp file
// 
// 2017/10/6  ��ֲ��ebox

#ifndef dht11_h
#define dht11_h

#include "ebox_gpio.h"

#define DHT11LIB_VERSION "0.4.1"

#define DHTLIB_OK				0
#define DHTLIB_ERROR_CHECKSUM	-1
#define DHTLIB_ERROR_TIMEOUT	-2

class dht11
{
public:
	dht11(E_PinID id){
		_p = new E_GPIO(id);
	}
	int read(void);
	int humidity;
	int temperature;
private:
	E_GPIO *_p;
};
#endif
//
// END OF FILE
//

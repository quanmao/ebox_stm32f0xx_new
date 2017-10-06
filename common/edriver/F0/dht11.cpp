//
// FILE: dht11.cpp
// VERSION: 0.4.1
// PURPOSE: DHT11 Temperature & Humidity Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
//
// HISTORY:
// George Hadjikyriacou - Original version (??)
// Mod by SimKard - Version 0.2 (24/11/2010)
// Mod by Rob Tillaart - Version 0.3 (28/03/2011)
// + added comments
// + removed all non DHT11 specific code
// + added references
// Mod by Rob Tillaart - Version 0.4 (17/03/2012)
// + added 1.0 support
// Mod by Rob Tillaart - Version 0.4.1 (19/05/2012)
// + added error codes
//

#include "dht11.h"
#include "ebox_core.h"
#include "ebox_debug.h"

#define LOW 	0
#define HIGH 	1

// Return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
int dht11::read(void)
{
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	_p->mode(OUTPUT_OD);
	no_interrupts();	//关中断，防止干扰总线通讯
	// 复位
	_p->reset();
	delay_ms(18);
	_p->set();
	delay_us(40);

	// 80us LOW Acknowledge or TIMEOUT 
	unsigned int loopCnt = 10000;
	while (_p->read() == LOW)
		if (loopCnt-- == 0)	return DHTLIB_ERROR_TIMEOUT;

	// 80us HIGH or TIMEOUT
	loopCnt = 10000;
	while (_p->read() == HIGH)
		if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		// 50us LOW lecel
		loopCnt = 10000;
		while (_p->read() == LOW)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;
		// HIGH level
		loopCnt = 0;
		while (_p->read() == HIGH  && loopCnt++ < 10000){
			delay_us(1);	// 实际延时约1.5us
		}
		if (loopCnt>=10000){
			DBG("idx =  %d,i = %d\r\n",idx,i);
			return DHTLIB_ERROR_TIMEOUT;
		}
		// HIGH level > 30us(20*1.5) is 1, else 0
		if (loopCnt > 20) bits[idx] |= (1 << cnt);

		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}
	// 读取结束，恢复中断
	interrupts();
	// WRITE TO RIGHT VARS
	// as bits[1] and bits[3] are allways zero they are omitted in formulas.
	humidity    = bits[0];
	temperature = bits[2];

	uint8_t sum = bits[0] + bits[2];
	// 校验
	if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
	return DHTLIB_OK;
}

//
// END OF FILE
//

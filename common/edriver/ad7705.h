#ifndef __AD7705_H_
#define __AD7705_H_

#include "ebox.h"

class AD7705{
	public:
		
	AD7705(E_GPIO *cs,E_GPIO *rst,E_GPIO *drdy,E_SPI *spi);
	void init();
	uint16_t readAdc(uint8_t ch);
	private:
	void syncSPI();
	void reset();
	void waitDRDY();
	void write(uint8_t data);
	uint8_t read();
	
	E_GPIO *_cs;
	E_GPIO *_rst;
	E_GPIO *_drdy;
	
	E_SPI *_spi;
	
	uint8_t _initOK;
	
};

#endif

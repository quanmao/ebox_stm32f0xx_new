 /*
 * Dht11.cpp
 *
 * Version 0.6
 *
 * DHT11 Temperature and humidity sensor library for eBOX.
 *
 * See http://playground.arduino.cc/main/DHT11Lib
 *
 * License:   GPL v3 (http://www.gnu.org/licenses/gpl.html)
 * Datasheet: http://www.micro4you.com/files/sensor/DHT11.pdf
 *
 * Modification History:
 *   - George Hadjikyriacou                     - Version 0.1 (??)
 *   - SimKard                                  - Version 0.2 (24/11/2010)
 *   - Rob Tillaart                             - Version 0.3 (28/03/2011)
 *       * Added comments
 *       * Removed all non-DHT11 specific code
 *       * Added References
 *   - Rob Tillaart                             - Version 0.4 (17/03/2012)
 *       * Added Arduino 1.0 support
 *   - Rob Tillaart                             - Version 0.4.1 (19/05/2012)
 *       * Added error codes
 *   - Andy Dalton                              - Version 0.5 (13/09/2013)
 *       * Replaced magic numbers with named constants
 *       * Factored out repeated code into a helper method.
 *       * Made pin a parameter to the constructor instead of a parameter
 *         to read()
 *       * Changed the error codes to an enumeration
 *       * Update the temp/humidity only if the checksum is correct
 *       * Added more comments
 *    - Link                            			  - Version 0.6 (2016/01/03)
 *       * port from ardunio to eBox
 */
#include "Dht11.h"
#include "ebox_debug.h"

#define BITS_IN(object) (8 * sizeof((object)))

// The version of this code
const char *const Dht11::VERSION = "0.6";

// Various named constants.
enum
{
    /*
     * Time required to signal the DHT11 to switch from low power mode to
     * running mode.  18 ms is the minimal, add a few extra ms to be safe.
     */
    START_SIGNAL_WAIT = 18,

    /*
     * Once the start signal has been sent, we wait for a response.  The doc
     * says this should take 20-40 us, we wait 5 ms to be safe.
     */
    RESPONSE_WAIT =  40,

    /*
     * The time threshold between a 0 bit and a 1 bit in the response.  Times
     * greater than this (in ms) will be considered a 1; otherwise they'll be
     * considered a 0.
     */
    ONE_THRESHOLD = 15,

    /*
     * The number of bytes we expect from the sensor.  This consists of one
     * byte for the integral part of the humidity, one byte for the fractional
     * part of the humidity, one byte for the integral part of the temperature,
     * one byte for the fractional part of the temperature, and one byte for
     * a checksum.  The DHT11 doesn't capture the fractional parts of the
     * temperature and humidity, but it doesn't transmit data during those
     * times.
     */
    RESPONSE_SIZE =  5,

    /*
     * The number of bits in a bytes.
     */
    BITS_PER_BYTE =  8,

    /*
     * The 0-base most significant bit in a byte.
     */
    BYTE_MS_BIT =  7,

    /*
     * The index in the response where the humidity reading is stored.
     */
    HUMIDITY_INDEX =  0,

    /*
     * The index in the response where the temperature is stored.
     */
    TEMPERATURE_INDEX =  2,

    /*
     * The index in the response where the checksum is stored.
     */
    CHECKSUM_INDEX =  4,
};

Dht11::ReadStatus Dht11::read()
{
    uint8_t    buffer[RESPONSE_SIZE] = { 0 };
    uint8_t    bitIndex              = BYTE_MS_BIT;
    ReadStatus status                = OK;
		__IO uint32_t	highStart,j,p;

    // Request sample
    pin->mode(OUTPUT_PP_PU);
    pin->reset();
    delay_ms(START_SIGNAL_WAIT);

    // Wait for response
    pin->set();
    delay_us(RESPONSE_WAIT);
    pin->mode(INPUT_PU);

    // Acknowledge or timeout
    // Response signal should first be low for 80us...
		// 等待从低变高

    if ((status = this->waitForPinChange(LOW)) != OK)
    {
				DBG("first low 80us NG \r\n");
        goto done;
    }
    // ... then be high for 80us ...
    if ((status = this->waitForPinChange(HIGH)) != OK)
    {
				DBG("first high 80us NG \r\n");
        goto done;
    }
		/*
     * ... then provide 5 bytes of data that include the integral part of the
     * humidity, the fractional part of the humidity, the integral part of the
     * temperature, the fractional part of the temperature, and a checksum
     * that is the sum of the integral parts of humidity and temperature.
     */
    for (size_t i = 0; i < BITS_IN(buffer); i++)
    {
				// low to high
//        if ((status = this->waitForPinChange(LOW)) != OK)
//        {
//						DBG("50us NG \r\n");
//            goto done;
//        }
				while(!pin->read());
				j = 0;
				while( pin->read() && j++ < 10000){
					 delay_us(1);					
				}
				if(j>10000)
				{
						status = ERROR_TIMEOUT;
						DBG("");
						goto done;
				}
//				if ((status = this->waitForPinChange(1)) != OK)
//        {
//						DBG("timeout %d us, index = %d \r\n",(micros() - highStart),bitIndex);
//            goto done;
//        }
				//DBG("j is %d \r\n",j);
        // 26-28 us = 0, 50 us = 1.  40 us is a good threshold between 0 and 1
				//j = (micros() - highStart);
        if ( j > ONE_THRESHOLD)
        {
            buffer[i / BITS_PER_BYTE] |= (1 << bitIndex);
        }
        // Decrement or reset bitIndex
				//DBG("%d \r\n",bitIndex);
        bitIndex = (bitIndex > 0) ? bitIndex - 1 : BYTE_MS_BIT;
    }

    // Check the checksum.  Only if it's good, record the new values.
//    if (buffer[CHECKSUM_INDEX] == (uint8_t)(  buffer[HUMIDITY_INDEX]
//                                     + buffer[TEMPERATURE_INDEX]))
//    {			
        // buffer[1] and buffer[3] should be the fractional parts of the
        // humidity and temperature, respectively, but the DHT11 doesn't
        // provide those values, so we omit them here.
        this->humidity    = buffer[HUMIDITY_INDEX];
        this->temperature = buffer[TEMPERATURE_INDEX];
//    }
//    else
//    {
//				DBG("r sum = %d, hum = %d, tem = %d \r\n",buffer[CHECKSUM_INDEX], buffer[HUMIDITY_INDEX],buffer[TEMPERATURE_INDEX]);
//        status = ERROR_CHECKSUM;
//    }

done:
    return status;
}

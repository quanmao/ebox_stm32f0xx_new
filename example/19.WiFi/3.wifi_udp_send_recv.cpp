/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ESP8266.h"
#include "esp8266_udp.h"

E_UART usart(USART1,PA_9,PA_10);
E_UART usart2(USART2,PA_2,PA_3);
E_GPIO	PA4(PA_4);

WIFI_UDP udp(&wifi);


char        remote_ip[] = "time.windows.com";
uint16_t    remote_port = 123;
uint16_t    local_port = 4321;


char recv_buf[1024] = {0};
uint8_t send_buf[] = "0b 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 d6 6d D9 00 00 00 00 00";
uint16_t len = 0;
uint32_t count = 0;

void setup()
{
    ebox_init();
    usart.begin(115200);

    wifi.begin(&PA4, &usart2, 115200);

    usart.printf("setup begin\r\n");
    
    usart.printf("FW Version: ");
		wifi.get_version(recv_buf);
    usart.printf("%s\r\n", recv_buf);
   
//    if (wifi.set_opr_to_softAP()) {
//        usart.printf("to station + softap ok\r\n");
//    } else {
//        usart.printf("to station + softap err\r\n");
//    }

    if (wifi.join_ap()) {
        usart.printf("Join AP success\r\n");
        usart.printf("IP: ");   
				wifi.query_sta_ip(recv_buf);
        usart.printf("%s\r\n", recv_buf);
    } else {
        usart.printf("Join AP failure\r\n");
    }
    
//    if (wifi.disable_MUX()) {
//        usart.printf("single ok\r\n");
//    } else {
//        usart.printf("single err\r\n");
//    }
    
    usart.printf("setup end\r\n");

}

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
int i;


void loop(void)
{
// send an NTP request to the time server at the given address
//unsigned long sendNTPpacket()
  // set all bytes in the buffer to 0
    uint8_t packetBuffer[NTP_PACKET_SIZE] = {0};
  // Initialize values needed to form NTP request
 // packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[0] = 0xe3;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
    if (udp.connect(remote_ip,remote_port,4001)) {
        usart.printf("udp connect ok\r\n");
    } else {
        usart.printf("udp connect fail\r\n");
    }
		delay_ms(10);
	while(1){
	packetBuffer[0] = 0xe3;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
		
    udp.send((uint8_t*)packetBuffer, NTP_PACKET_SIZE);       
		uint32_t len = udp.read((uint8_t*)packetBuffer);
    if (len > 0) {
    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:		
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = (highWord << 16 | lowWord)+6;
    //usart.printf("Seconds since Jan 1 1900 = %d \r\n",secsSince1900);
    // now convert NTP time into everyday time:

    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears + 8 * 3600;
    // print Unix time:
		usart.printf("Unix time = %d  ",epoch);
    // print the hour, minute and second:
			usart.printf("The Beijing time is %d:",(epoch  % 86400L) / 3600);       // UTC is the time at Greenwich Meridian (GMT) // print the hour (86400 equals secs per day)
//    if ( ((epoch % 3600) / 60) < 10 ) {
//      // In the first 10 minutes of each hour, we'll want a leading '0'
//      //Serial.print('0');
//    }

		usart.printf("%02d:",(epoch  % 3600) / 60);// print the minute (3600 equals secs per minute)
//    if ( (epoch % 60) < 10 ) {
//      // In the first 10 seconds of each minute, we'll want a leading '0'
//    }
		usart.printf("%02d \r\n",epoch % 60);// print the second
    }
    else {
        usart.printf("received failure \r\n");
    }

    delay_ms(5000);
	}
}

int main(void)
{
    bool ret;
    setup();

    while(1)
    {
			loop();
    }

}









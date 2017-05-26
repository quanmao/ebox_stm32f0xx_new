/******************************************************************************
程 序 名： 　　
编 写 人： LQM　
编写时间： 2014年8月21日
硬件支持： STM32F030F4-20Pin　　
接口说明： 4位共阳数码，其中数据端口和公共端口均为低电平有效.型号 SM410564
修改日志：　　
　　NO.1-  2017年2月28日 移植到ebox平台
*******************************************************************************
说明：
******************************************************************************/
#include "ebox.h"

#ifndef	_DRIVER_LED_H_
#define _DRIVER_LED_H_

#define	DP_ON	0x7f	//显示小数点
#define DP_OFF	0xff	//不显示小数点

#define	Display_ON	1	//显示开
#define Display_OFF 0	//显示关


class NUMLED{
public:
    NUMLED(E_PORT *digital,E_GPIO * A0,E_GPIO * A1,E_GPIO * A2,E_GPIO * A3);
    NUMLED(E_PORT *digital,E_PinID A0,E_PinID A1,E_PinID A2,E_PinID A3);

    /**
    		函数名称：   Led_Display()
    		入口参数:    Ctr,要显示的位，1-4,对应数码管从左到右
    		出口参数：   无
    		备    注：   采用动态扫描的方式，扫描频率有外界控制
       */
    void display(unsigned char Ctr);
    /** @brief 显示扫描，定时调用此程序。将缓冲区数据送到数码管显示
       * @param none
       */    
    void scan(void);
    void on(void);
    void off(void);
    /** @brief 更新显示缓存
       * @param BufferBit 要更新的数据位 1-4
       * @param Number 要更新的数据 0-f
       * @param DisFlag 显示标志 0 不显示，1显示
       * @param DPFlag  小数点标志 DP_OFF DP_ON显示
       */
    void bufferUpdate(unsigned char BufferBit,unsigned char Number,unsigned char DisBit,unsigned char DPBit);
private:
    E_PORT * _digital;
    E_GPIO * _A0;
    E_GPIO * _A1;
    E_GPIO * _A2;
    E_GPIO * _A3;

    volatile unsigned char _flag;
    void _close(void);
};

#endif

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
#include	"ebox.h"
#include	"driver_led.h"

// 显示缓冲区，包含3个数据，分别是：要显示的数据，消隐标志（是否显示），小数点标志
unsigned char Dis_Buffer[4][3] = {{0,1,DP_ON},{1,0,DP_OFF},{2,1,DP_OFF},{3,1,DP_OFF}};

/*0-9  a-f*/
static const unsigned char Num_code[] =
{
    0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,
    0x80,0x90,0x88,0x83,0xc6,0x86,0x8e,0x82
};

NUMLED::NUMLED(E_PORT *digital,E_GPIO *A0,E_GPIO *A1,E_GPIO *A2,E_GPIO *A3){
    _digital = digital;
    _A0 = A0;
    _A1 = A1;
    _A2 = A2;
    _A3 = A3;

    _digital->mode(OUTPUT_PP);
    _A0->mode(OUTPUT_PP);
    _A1->mode(OUTPUT_PP);
    _A2->mode(OUTPUT_PP);
    _A3->mode(OUTPUT_PP);

    _flag = Display_ON;
    _close();
}

NUMLED::NUMLED(E_PORT *digital,E_PinID A0,E_PinID A1,E_PinID A2,E_PinID A3){
    _digital = digital;
    _A0 = new E_GPIO(A0);
    _A1 = new E_GPIO(A1);
    _A2 = new E_GPIO(A2);
    _A3 = new E_GPIO(A3);

    _digital->mode(OUTPUT_PP);
    _A0->mode(OUTPUT_PP);
    _A1->mode(OUTPUT_PP);
    _A2->mode(OUTPUT_PP);
    _A3->mode(OUTPUT_PP);

    _flag = Display_ON;
    _close();
}
void NUMLED::_close(void){
    _A0->set();
    _A1->set();
    _A2->set();
    _A3->set();
}

void NUMLED::display(unsigned char Ctr){

    _close();
    // 如果该位不显示，则返回
    if (Dis_Buffer[Ctr-1][1] == 0) return;
    // 判断是否显示小数点
    //		Num_code[Dis_Buffer[Ctr-1][0]]) & Dis_Buffer[Ctr-1][2] -- 置小数点位
    * _digital = ((unsigned char)(Num_code[Dis_Buffer[Ctr-1][0]])& Dis_Buffer[Ctr-1][2]);

    switch (Ctr)
    {
    case 1:
        _A0->reset();
        break;
    case 2:
        _A1->reset();
        break;
    case 3:
        _A2->reset();
        break;
    case 4:
        _A3->reset();
        break;
    default:
        break;
    }
}

void NUMLED::scan(void)
{
    static uint8_t bit = 1;
	
    if (_flag){
			bit = bit >= 4 ? 1 :(bit +1);
			display(bit);
    }
}

void NUMLED::on(void){
    _flag = Display_ON;
}

void NUMLED::off(void){
    _flag = Display_OFF;
    _close();
}

void NUMLED::bufferUpdate(unsigned char BufferBit,unsigned char Number,unsigned char DisBit,unsigned char DPBit){
    Dis_Buffer[BufferBit-1][0] = Number;
    Dis_Buffer[BufferBit-1][1] = DisBit;
    Dis_Buffer[BufferBit-1][2] = DPBit;
}


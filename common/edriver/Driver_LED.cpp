/******************************************************************************
�� �� ���� ����
�� д �ˣ� LQM��
��дʱ�䣺 2014��8��21��
Ӳ��֧�֣� STM32F030F4-20Pin����
�ӿ�˵���� 4λ�������룬�������ݶ˿ں͹����˿ھ�Ϊ�͵�ƽ��Ч.�ͺ� SM410564
�޸���־������
����NO.1-  2017��2��28�� ��ֲ��eboxƽ̨
*******************************************************************************
˵����
******************************************************************************/
#include	"ebox.h"
#include	"driver_led.h"

// ��ʾ������������3�����ݣ��ֱ��ǣ�Ҫ��ʾ�����ݣ�������־���Ƿ���ʾ����С�����־
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
    // �����λ����ʾ���򷵻�
    if (Dis_Buffer[Ctr-1][1] == 0) return;
    // �ж��Ƿ���ʾС����
    //		Num_code[Dis_Buffer[Ctr-1][0]]) & Dis_Buffer[Ctr-1][2] -- ��С����λ
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


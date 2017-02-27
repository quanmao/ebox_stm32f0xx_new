#include "ad7705.h"

/* ͨ�żĴ���bit���� */
enum
{
	/* �Ĵ���ѡ��  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* ͨ�żĴ��� */
	REG_SETUP	= 0x10,	/* ���üĴ��� */
	REG_CLOCK	= 0x20,	/* ʱ�ӼĴ��� */
	REG_DATA	= 0x30,	/* ���ݼĴ��� */
	REG_ZERO_CH1	= 0x60,	/* CH1 ƫ�ƼĴ��� */
	REG_FULL_CH1	= 0x70,	/* CH1 �����̼Ĵ��� */
	REG_ZERO_CH2	= 0x61,	/* CH2 ƫ�ƼĴ��� */
	REG_FULL_CH2	= 0x71,	/* CH2 �����̼Ĵ��� */

	/* ��д���� */
	WRITE 		= 0x00,	/* д���� */
	READ 		= 0x08,	/* ������ */

	/* ͨ�� */
	CH_1		= 0,	/* AIN1+  AIN1- */
	CH_2		= 1,	/* AIN2+  AIN2- */
	CH_3		= 2,	/* AIN1-  AIN1- */
	CH_4		= 3		/* AIN1-  AIN2- */
};

/* ���üĴ���bit���� */
enum
{
	MD_NORMAL		= (0 << 6),	/* ����ģʽ */
	MD_CAL_SELF		= (1 << 6),	/* ��У׼ģʽ */
	MD_CAL_ZERO		= (2 << 6),	/* У׼0�̶�ģʽ */
	MD_CAL_FULL		= (3 << 6),	/* У׼���̶�ģʽ */

	GAIN_1			= (0 << 3),	/* ���� */
	GAIN_2			= (1 << 3),	/* ���� */
	GAIN_4			= (2 << 3),	/* ���� */
	GAIN_8			= (3 << 3),	/* ���� */
	GAIN_16			= (4 << 3),	/* ���� */
	GAIN_32			= (5 << 3),	/* ���� */
	GAIN_64			= (6 << 3),	/* ���� */
	GAIN_128		= (7 << 3),	/* ���� */

	/* ����˫���Ի��ǵ����Զ����ı��κ������źŵ�״̬����ֻ�ı�������ݵĴ����ת�������ϵ�У׼�� */
	BIPOLAR			= (0 << 2),	/* ˫�������� */
	UNIPOLAR		= (1 << 2),	/* ���������� */

	BUF_NO			= (0 << 1),	/* �����޻��壨�ڲ�������������) */
	BUF_EN			= (1 << 1),	/* �����л��� (�����ڲ�������) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* ������ */
};

/* ʱ�ӼĴ���bit���� */
enum
{
	CLKDIS_0	= 0x00,		/* ʱ�����ʹ�� ������Ӿ���ʱ������ʹ�ܲ����񵴣� */
	CLKDIS_1	= 0x10,		/* ʱ�ӽ�ֹ �����ⲿ�ṩʱ��ʱ�����ø�λ���Խ�ֹMCK_OUT�������ʱ����ʡ�� */

	/*
		2.4576MHz��CLKDIV=0 ����Ϊ 4.9152MHz ��CLKDIV=1 ����CLK Ӧ�� ��0����
		1MHz ��CLKDIV=0 ���� 2MHz   ��CLKDIV=1 ����CLK ��λӦ��  ��1��
	*/
	CLK_4_9152M = 0x08,
	CLK_2_4576M = 0x00,
	CLK_1M 		= 0x04,
	CLK_2M 		= 0x0C,

	FS_50HZ		= 0x00,
	FS_60HZ		= 0x01,
	FS_250HZ	= 0x02,
	FS_500HZ	= 0x04,

	/*
		��ʮ�š����ӳ�Ӧ�������TM7705 ���ȵķ���
			��ʹ����ʱ��Ϊ 2.4576MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ 84H,��ʱ�������������Ϊ10Hz,��ÿ0.1S ���һ�������ݡ�
			��ʹ����ʱ��Ϊ 1MHz ʱ��ǿ�ҽ��齫ʱ�ӼĴ�����Ϊ80H, ��ʱ�������������Ϊ4Hz, ��ÿ0.25S ���һ��������
	*/
	ZERO_0		= 0x00,
	ZERO_1		= 0x80
};

void AD7705::reset(){
	_rst->set();
	delay_ms(10);
	_rst->reset();
	delay_ms(10);
	_rst->set();
	delay_ms(5);
}

void AD7705::init()
{
	 delay_ms(5);
	 reset();
	 syncSPI();
}

AD7705::AD7705(E_GPIO *cs,E_GPIO *rst,E_GPIO *drdy,E_SPI *spi){
	E_SPI_CONFIG_T spi_config;
	
	spi_config.bit_order = SPI_BITODER_MSB;
	spi_config.prescaler = SPI_CLOCK_DIV256;
	spi_config.mode = SPI_MODE3;
	
	_cs = cs;	
	_rst = rst;
	_drdy = drdy;
	_spi = spi;
	
	_cs->mode(OUTPUT_PP);
	_rst->mode(OUTPUT_PP_PU);
	_drdy->mode(INPUT_PU);
	
	_spi->config(&spi_config);	
}

void AD7705::write(uint8_t data)
{
	 _cs->reset();
	 _spi->writeChar(data);
	 _cs->set();
}

uint8_t AD7705::read(){
	uint8_t t;
	_cs->reset();
	t = _spi->read();
	_cs->set();
	return t;	
}

void AD7705::syncSPI(){
	uint8_t i;
	_cs->reset();	
	_spi->writeChar(0xff);
	_spi->writeChar(0xff);
	_spi->writeChar(0xff);
	_spi->writeChar(0xff);
	_spi->writeChar(0xff);	
	_spi->writeChar(0xff);
	_spi->writeChar(0xff);
	_cs->set();
	
	_cs->reset();
	
	i = _spi->writeChar(REG_CLOCK| READ);	
	i = _spi->read();
	
	if(i == 0x05){
		 _initOK = 1;
	}else{
		 i= 0;
	}
	
	_spi->writeChar(REG_CLOCK | WRITE | CH_1);
	_spi->writeChar(CLKDIS_0 | CLK_4_9152M | FS_50HZ);
	
	_cs->set();
}

void AD7705::waitDRDY(){
		uint32_t i;
	
		/* �����ʼ��ʱδ��⵽оƬ����ٷ��أ�����Ӱ����������Ӧ�ٶ� */
	if (_initOK == 0)
	{
		return;
	}
  _spi->writeChar(REG_CLOCK| READ);	
_cs->reset();	
	for (i = 0; i < 4000000; i++){	
	i = _spi->read();
	if((i & 0x80) == 0x00){
		i = _drdy->read();
//		_cs->set();
		 break;
	}
	}
//_cs->set();

	
//	for (i = 0; i < 4000000; i++)
//	{
//		if (_drdy->read() == 0)
//		{
//			break;
//		}
//	}
	if (i >= 4000000)
	{
		 _initOK = 0;
//		printf("TM7705_WaitDRDY() Time Out ...\r\n");		/* �������. �����Ŵ� */
	}
}

uint16_t AD7705::readAdc(uint8_t ch){
	uint8_t i;
	uint16_t read = 0;
	_cs->reset();
	/* Ϊ�˱���ͨ���л���ɶ���ʧЧ����2�� */
	for (i = 0; i < 2; i++)
	{
		waitDRDY();		/* �ȴ�DRDY����Ϊ0 */

		if (ch == 1)
		{
			_spi->writeChar(0x38);
		}
		else if (ch == 2)
		{
			_spi->writeChar(0x39);
		}

		read = _spi->read();
		read <<= 8;
		read += _spi->read();
	}
	_cs->set();
	return read;
}

#include "ad7705.h"

/* 通信寄存器bit定义 */
enum
{
	/* 寄存器选择  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* 通信寄存器 */
	REG_SETUP	= 0x10,	/* 设置寄存器 */
	REG_CLOCK	= 0x20,	/* 时钟寄存器 */
	REG_DATA	= 0x30,	/* 数据寄存器 */
	REG_ZERO_CH1	= 0x60,	/* CH1 偏移寄存器 */
	REG_FULL_CH1	= 0x70,	/* CH1 满量程寄存器 */
	REG_ZERO_CH2	= 0x61,	/* CH2 偏移寄存器 */
	REG_FULL_CH2	= 0x71,	/* CH2 满量程寄存器 */

	/* 读写操作 */
	WRITE 		= 0x00,	/* 写操作 */
	READ 		= 0x08,	/* 读操作 */

	/* 通道 */
	CH_1		= 0,	/* AIN1+  AIN1- */
	CH_2		= 1,	/* AIN2+  AIN2- */
	CH_3		= 2,	/* AIN1-  AIN1- */
	CH_4		= 3		/* AIN1-  AIN2- */
};

/* 设置寄存器bit定义 */
enum
{
	MD_NORMAL		= (0 << 6),	/* 正常模式 */
	MD_CAL_SELF		= (1 << 6),	/* 自校准模式 */
	MD_CAL_ZERO		= (2 << 6),	/* 校准0刻度模式 */
	MD_CAL_FULL		= (3 << 6),	/* 校准满刻度模式 */

	GAIN_1			= (0 << 3),	/* 增益 */
	GAIN_2			= (1 << 3),	/* 增益 */
	GAIN_4			= (2 << 3),	/* 增益 */
	GAIN_8			= (3 << 3),	/* 增益 */
	GAIN_16			= (4 << 3),	/* 增益 */
	GAIN_32			= (5 << 3),	/* 增益 */
	GAIN_64			= (6 << 3),	/* 增益 */
	GAIN_128		= (7 << 3),	/* 增益 */

	/* 无论双极性还是单极性都不改变任何输入信号的状态，它只改变输出数据的代码和转换函数上的校准点 */
	BIPOLAR			= (0 << 2),	/* 双极性输入 */
	UNIPOLAR		= (1 << 2),	/* 单极性输入 */

	BUF_NO			= (0 << 1),	/* 输入无缓冲（内部缓冲器不启用) */
	BUF_EN			= (1 << 1),	/* 输入有缓冲 (启用内部缓冲器) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* 不启用 */
};

/* 时钟寄存器bit定义 */
enum
{
	CLKDIS_0	= 0x00,		/* 时钟输出使能 （当外接晶振时，必须使能才能振荡） */
	CLKDIS_1	= 0x10,		/* 时钟禁止 （当外部提供时钟时，设置该位可以禁止MCK_OUT引脚输出时钟以省电 */

	/*
		2.4576MHz（CLKDIV=0 ）或为 4.9152MHz （CLKDIV=1 ），CLK 应置 “0”。
		1MHz （CLKDIV=0 ）或 2MHz   （CLKDIV=1 ），CLK 该位应置  “1”
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
		四十九、电子秤应用中提高TM7705 精度的方法
			当使用主时钟为 2.4576MHz 时，强烈建议将时钟寄存器设为 84H,此时数据输出更新率为10Hz,即每0.1S 输出一个新数据。
			当使用主时钟为 1MHz 时，强烈建议将时钟寄存器设为80H, 此时数据输出更新率为4Hz, 即每0.25S 输出一个新数据
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
	
		/* 如果初始化时未检测到芯片则快速返回，避免影响主程序响应速度 */
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
//		printf("TM7705_WaitDRDY() Time Out ...\r\n");		/* 调试语句. 用语排错 */
	}
}

uint16_t AD7705::readAdc(uint8_t ch){
	uint8_t i;
	uint16_t read = 0;
	_cs->reset();
	/* 为了避免通道切换造成读数失效，读2次 */
	for (i = 0; i < 2; i++)
	{
		waitDRDY();		/* 等待DRDY口线为0 */

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

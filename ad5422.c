#include "ad5422.h"
#include <lpc213x.h>
#include "ds18b20.h"

/*----------------------- Byte transfer through SPI-------------------------------------*/
static char spiTransferByteAD(char byte)
{
S0SPDR = byte;
while(!(S0SPSR & 0x80));				//Wait for transfer finish
byte=S0SPDR;

return byte;
}
/*----------------------- AD5422 Initialization ----------------------------------------*/
int AD5422_init(void)
{
DIR_LATCH();
CLR_LATCH();
PINSEL0 |= 0x1500;					//PIN function select register
S0SPCCR = 10;						//SPI Clock Counter Register 8bits The register indicates the number pclk cycles that make up SPI CLOCK
S0SPCR  = 0x20;						//CPHA 0 CPOL 0 8bit Register control the operation of the SPI0 as per-zgodnie z czyms
FIO0DIR &=~(1<<16);
FIO0DIR &=~(1<<15);
//RESET AD5422 It should be done because if power rising very slow it couse wrong operation of the AD5422
spiTransferByteAD(0x56);//RSTREG);		//First AD5412/22 Reset Register
spiTransferByteAD(0x00);				//
spiTransferByteAD(0x01);				//Reset
SET_LATCH();
delay_ms(1);
CLR_LATCH();
spiTransferByteAD(CNTRLREG);			//First AD5412/22 Address Word Configuration Register
spiTransferByteAD(0x00);
spiTransferByteAD(0x08);				//Daisy-Chain Enable
SET_LATCH();
delay_ms(1);
CLR_LATCH();
spiTransferByteAD(CNTRLREG);			//First AD5412/22 Address Word Configuration Register
spiTransferByteAD(0x30);
spiTransferByteAD(0x0D);				//0x05=4-20mA,
spiTransferByteAD(RSTREG);				//Second AD5412/22 Reset Register
spiTransferByteAD(0x00);
spiTransferByteAD(0x01);				//RESET
SET_LATCH();
delay_ms(1);
CLR_LATCH();
spiTransferByteAD(CNTRLREG);			//First AD5412/22 Address Word Configuration Register
spiTransferByteAD(0x30);
spiTransferByteAD(0x0D);				//0x05=4-20mA, Daisy-Chain
spiTransferByteAD(CNTRLREG);			//Second AD5412/22 Address Word Configuration Register
spiTransferByteAD(0x30);
spiTransferByteAD(0x0D);				//4-20mA
SET_LATCH();
delay_ms(1);
CLR_LATCH();
return 1;
}
/*------------------------ Set proper output -------------------------------------------*/
unsigned int AD5422_SetOutputVorI(CHANNEL *ch)
{					 
unsigned int i;
float voltage;
float dummy;
unsigned short data[2];


CLR_LATCH();
for(i=0;i<2;i++){
	if((*(ch[i].status)==OK)||(*(ch[i].status)==ALERT)||(*(ch[i].status)==ALARM))
		switch(_4_20mA)
		{
		case _0_5V:
			{dummy=(5.0/((ch[i].up_lvl)-(ch[i].dw_lvl)));
			voltage =((dummy)*((*ch[i].temp)+ch[i].offset)-(ch[i].dw_lvl)*(dummy))*1.0;
			data[i] = (voltage/5)*0xffff;
			break;}
		case _0_10V:
			{dummy=(10.0/((ch[i].up_lvl)-(ch[i].dw_lvl)));
			voltage =((dummy)*((*ch[i].temp)+ch[i].offset)-(ch[i].dw_lvl)*(dummy))*1.0;
			data[i] = (voltage/10)*0xffff;
			break;}
		case _4_20mA:
			{dummy=(16.0/((ch[i].up_lvl)-(ch[i].dw_lvl)));
			voltage =((dummy)*((*ch[i].temp)+ch[i].offset)+4.0-(ch[i].dw_lvl)*(dummy))*1.0;
			if(voltage<=20)
				data[i] = (unsigned int)(((voltage-4.0)/16.0)*0xffff);
			else
				data[i] = 0xffff;
			break;}
		case _0_20mA:
			{break;}
		case _0_24mA:
			{break;}
		}
	else
		data[i]=0;
}
spiTransferByteAD(0x01);
spiTransferByteAD((unsigned char)(data[0]>>8));
spiTransferByteAD((unsigned char)(data[0]));
spiTransferByteAD(0x01);
spiTransferByteAD((unsigned char)(data[1]>>8));
spiTransferByteAD((unsigned char)(data[1]));
SET_LATCH();
CLR_LATCH();
return 1;
}

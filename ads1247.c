#include <lpc213x.h>
#include "ads1247.h"
#include "DS18b20.h"
#include <math.h>


extern short usRegInputBuf[];
/*----------------------- Linearization coefficient for Pt100----------------------------*/
const  double Z1 = -0.0039083;
const  double Z2 = 0.00001758480889;
const  double Z3 = -0.00000002310;
const  double Z4 = -0.000001155;
/*----------------------- Linearization coefficient for Thermocouple K Type--------------*/
const  double A0 = 0.226584602;
const  double A1 = 24152.10900;
const  double A2 = 67233.4248;
const  double A3 = 2210340.682;
const  double A4 = -860963914.9;
const  double A5 = 48350600000.0;
const  double A6 = -1184520000000.0;
const  double A7 = 13869000000000.0;
const  double A8 = -63370800000000.0;

const double CJcomp_A = -0.00001879999;
const double CJcomp_B = -0.03951820605;
const double CJcomp_C = -0.0008277165;

/*----------------------- Byte transfer through SPI-------------------------------------*/

static char spiTransferByte(char byte)
{
SSPDR = byte;
while(SSPSR & 0x10);				//Wait until transfer finish
byte=SSPDR;
return byte;
}

/*----------------------- SPI initialization for Temperature measurement ---------------*/

void ads1247_init(void)
{
int i;
PINSEL1 |= 0xa8; 					//MOSI SSP
SSPCR0=0x07 | 1<<7;					//CPHA 1 CPOL 0 8bit
SSPCPSR = 40;						//Clock Prescale Register
SSPCR1=2;							//Control Register 1

FIO0DIR |= (1<<30);					//CS
FIO0PIN &= ~(1<<30);

FIO1DIR |= 1<<16;					//START
FIO1PIN &= ~(1<<16);

FIO0DIR &= ~(1<<29);			    //DRDY

spiTransferByte(0xff);				//SDATAC
spiTransferByte(0x16);

FIO1PIN |= (1<<16);					//START

spiTransferByte(0xff);				//PO INIT
spiTransferByte(0x0E);				
for(i=0;i<9999999;i++);

spiTransferByte(0x62);				//Self Offset Calibration
while(FIO0PIN & (1<<29));

}

/*-----------------------  Check if sensor is connected---------------------------------*/

unsigned int ads1247_BurnoutCurrentSense(unsigned char channel)
{
unsigned char tab[]={0x00,0xc1,0xd3,0x00,0x00};
	FIO0PIN &= ~(1<<30);				//CS
	FIO1PIN |= (1<<16);             //Set Start

spiTransferByte(0x4a);				
spiTransferByte(0x01);
spiTransferByte(0x00);				//Turn Off Current sources
spiTransferByte(0xff);

spiTransferByte(0x40);				//Write Function
spiTransferByte(0x03);	   			//Write 4 registers
	if(channel == 1)
		spiTransferByte(0xda);		//AIN2 Negative AIN3 Positive Burnoutcurrent source 10uA //D3 Pt100		channel 1
		
	if(channel == 2)
		spiTransferByte(0xc8);		//AIN0 positive AIN1 Negative Burnoutcurrent source 10uA //C1 Pt100		channel 2 
spiTransferByte(0x00);				//BIAS turned off
spiTransferByte(0x30);				//Internal reference is always on, On board reference selected 
spiTransferByte(0x07);				//2000SPS



spiTransferByte(0x20);				//Read Function
spiTransferByte(0x03);				//Read 4 registers it is for connection validation 
tab[0]=spiTransferByte(0xff);		// if some of this register have improper value, function returns 250  
tab[1]=spiTransferByte(0xff);		
tab[2]=spiTransferByte(0xff);
tab[3]=spiTransferByte(0xff);

if((tab[0]==0xC8 || tab[0]==0xdA) && tab[1]==0x00 && tab[2]==0x30 && tab[3]==0x07)
{
	while(FIO0PIN & (1<<29));
	while(!(FIO0PIN & (1<<29)));

	spiTransferByte(0x12);
	tab[0]=spiTransferByte(0xff);
	tab[1]=spiTransferByte(0xff);
	tab[2]=spiTransferByte(0xff);
	if(!(tab[0] & 0x80))
		return (tab[0]);
	if(tab[0] & 0x80)
		return ((~tab[0]) & 0x7f); //SPI WORKS FINE
}else{
FIO0PIN |= (1<<30);
delay_us(2);
FIO0PIN &=~(1<<30);
}
return 250;//SPI DONT WORK PROPERLY
}

/*--------------------------PT100------------------------------------------------------*/
float PT100(unsigned char channel,short *mb)
{
unsigned char tab[]={0x01,0x13,0x00,0xff};
unsigned int temp;
double dummy;
//float err;

FIO0PIN &= ~(1<<30); //cs

//if(channel == 1)
//	err= 169.00;		//By modyfiing this value we can add correction Channel 2 podobno 
//if(channel == 2)
//	err= 169.00;		//like above but its for second channel			Channel 1 podobno

spiTransferByte(0x4a);  //at first we gona set IDAC Control Register 
spiTransferByte(0x01);
spiTransferByte(0x03);
	if(channel ==1)
		spiTransferByte(0x23); //IDAC Configuration for channel 1
	if(channel ==2)
		spiTransferByte(0x01); //IDAC Configuration for channel 2
spiTransferByte(0x41); 
spiTransferByte(0x02);
spiTransferByte(0x00); 		   //VBIAS
spiTransferByte(0x20); 		   //MUX1
spiTransferByte(0x30);		   //SYS0

spiTransferByte(0x40);
spiTransferByte(0x00);
if(channel == 1)
		spiTransferByte(0x13);//MUX0
if(channel == 2)
		spiTransferByte(0x01);//MUX0
spiTransferByte(0x20);				//Read Function
spiTransferByte(0x03);				//Read 4 registers it is for connection validation 
tab[0]=spiTransferByte(0xff);		// if some of this register have improper value, function returns 250  
tab[1]=spiTransferByte(0xff);		
tab[2]=spiTransferByte(0xff);
tab[3]=spiTransferByte(0xff);

if((tab[0]==0x13 || tab[0]==0x01) && (tab[1]==0x00) && tab[2]==0x20 && tab[3]==0x30)
{

			while(FIO0PIN & (1<<29));
			while(!(FIO0PIN & (1<<29)));

		spiTransferByte(0x12);
		/*usRegInputBuf[30+channel*3]=*/tab[0]=spiTransferByte(0xff);
		/*usRegInputBuf[31+channel*3]=*/tab[1]=spiTransferByte(0xff);
		/*usRegInputBuf[32+channel*3]=*/tab[2]=spiTransferByte(0xff);
		
		if((tab[0]==127)&&(tab[1]==255)&&(tab[2]==255))
			{*mb=9999;
			return 4;}

		if(!(tab[0] & 0x80)){
		temp = (tab[0]<<16) | (tab[1]<<8) | tab[2];
		dummy = ((temp*2475.0)/0x7fffff)*1.0;
		dummy=((dummy/12.0)+169.0);
		*mb=((Z1+sqrt((Z2+Z3*dummy)))/Z4)*10.0;
		return 0;}

		if(tab[0] & 0x80){
		temp =((~tab[0] & 0x7f)<<16) | ((~tab[1] & 0xff)<<8) | ((~tab[2] & 0xff));
		dummy = ((temp*2475.0)/0x7fffff)*1.0;
		dummy=(169.0-(dummy/12.0));
		*mb=((Z1+sqrt((Z2+Z3*dummy)))/Z4)*10.0;
		return 0;}
}else{

FIO0PIN |= (1<<30);
delay_us(2);
FIO0PIN &=~(1<<30);
}
return 250;
}
/*--------------------------Thermocouple-----------------------------------------------*/
float Thermocouple(unsigned char channel, short *mb, short env_temp)
{

FIO0PIN &= ~(1<<30);

int sign=1;
 unsigned char tab[]={0x01,0x13,0x00,0x00};
 unsigned int temp;
 double dummy,t;
 //unsigned int er;
spiTransferByte(0x40);
spiTransferByte(0x03);
	if(channel == 1)
	{
		spiTransferByte(0x1a);
		spiTransferByte(0x08);//0x04	   
	}
	if(channel == 2)
	{
		spiTransferByte(0x08);
		spiTransferByte(0x02);		   	 
	}
spiTransferByte(0x30);	   
spiTransferByte(0x60);	//0x60=64  0x70=128


spiTransferByte(0x20);
spiTransferByte(0x03);
tab[0]=spiTransferByte(0xff);
tab[1]=spiTransferByte(0xff);
tab[2]=spiTransferByte(0xff);
tab[3]=spiTransferByte(0xff);

if((tab[0]==0x1a || tab[0]==0x08) && (tab[1]==0x02 || tab[1]==0x08) && tab[2]==0x30 && tab[3]==0x60)
{
			while(FIO0PIN & (1<<29));
			while(!(FIO0PIN & (1<<29)));
			//while(FIO0PIN & (1<<29));

		spiTransferByte(0x12);
		/*usRegInputBuf[30+channel*3]=*/tab[0]=spiTransferByte(0xff);
		/*usRegInputBuf[31+channel*3]=*/tab[1]=spiTransferByte(0xff);
		/*usRegInputBuf[32+channel*3]=*/tab[2]=spiTransferByte(0xff);

		temp = (tab[0]<<16) | (tab[1]<<8) | tab[2];
		if(0x800000 & temp)
		{sign=-1;
		temp = (~temp)&0xffffff;}

		dummy = ((temp*2048.0)/0x7fffff)*1.0;
		dummy = dummy/64000.0;


		t=-CJcomp_A*(env_temp/100.0)*(env_temp/100.0)-CJcomp_B*(env_temp/100.0)-CJcomp_C;
		usRegInputBuf[33]=dummy*1000000.0;
		usRegInputBuf[34]=t*1000.0;
		if(sign==-1)
			{dummy =dummy - (t/1000.0);}
		else
			{dummy+=(t/1000.0);}

		dummy= A0  +A1*dummy 
					  	+A2*(dummy*dummy) 
					  	+A3*(dummy*dummy*dummy) 
					  	+A4*(dummy*dummy*dummy*dummy) 
					  	+A5*(dummy*dummy*dummy*dummy*dummy) 
					  	+A6*(dummy*dummy*dummy*dummy*dummy*dummy)
					  	+A7*(dummy*dummy*dummy*dummy*dummy*dummy*dummy)
					  	+A8*(dummy*dummy*dummy*dummy*dummy*dummy*dummy*dummy);
		//er=0;

		if(sign == -1)
			{dummy=-dummy;
			 mb[0]=(dummy)*10;}
		else
			{//dummy=dummy;
			mb[0]=(dummy)*10;}
		return 1;
}else{
FIO0PIN |= (1<<30);
delay_us(2);
FIO0PIN &=~(1<<30);
return 250;
}
}

/*int offset(unsigned char channel)
{
spiTransferByte(0x40);
spiTransferByte(0x03);
	if(channel == 1)
	{
		spiTransferByte(0x1a);
		spiTransferByte(0x08);//0x04	   
	}
	if(channel == 2)
	{
		spiTransferByte(0x08);
		spiTransferByte(0x02);		   	 
	}
spiTransferByte(0x31);	   
spiTransferByte(0x70);	//0x60=64
			while(FIO0PIN & (1<<29));
			while(!(FIO0PIN & (1<<29)));

		spiTransferByte(0x12);
		usRegInputBuf[30+channel*3]=spiTransferByte(0xff);
		usRegInputBuf[31+channel*3]=spiTransferByte(0xff);
		usRegInputBuf[32+channel*3]=spiTransferByte(0xff);


	
}*/


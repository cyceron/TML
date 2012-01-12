#include <lpc213x.h>
#include "rtc.h"
extern short usRegInputBuf[];
#define CH1_STATUS 19
#define CH2_STATUS 20

/*-------------------------- Real Time Clock Initialization --------------*/
void rtc_init(void)
{
	PREINT=114;
	PREFRAC = 11264448;
	CIIR =0x01;
	CCR=0x01;
	VICVectAddr13 = (unsigned int) rtc_isr;
	VICVectCntl13 = 0x02d;
}
/*-------------------------- Real Time Clock Interrupt Routine -----------*/
void rtc_isr(void)
{
if(usRegInputBuf[CH1_STATUS]==4)  //1
	{if(FIO1PIN & D2G)
	{	FIO1PIN&=~D2G;}
	else
	{	FIO1PIN|=D2G;}}
if(usRegInputBuf[CH2_STATUS]==4)  //2
	{if(FIO0PIN & D1G)
	{   FIO0PIN&=~D1G;}
	else
	{	FIO0PIN|=D1G;}}
ILR = 0x03;
VICVectAddr = 0x00000000;
}

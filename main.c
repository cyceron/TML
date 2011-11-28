
/* ------------------------LPC213X ------------------------------------------*/
#include <lpc213x.h>
/* ------------------------ADS1247 ------------------------------------------*/
#include "ads1247.h"
/* ------------------------AD5422  ------------------------------------------*/
#include "ad5422.h"
/* ------------------------DS18B20 ------------------------------------------*/
#include "DS18b20.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Non Volatile Storage -----------------------------*/
#include "NVS\flash_nvol.h"
/* ----------------------- Real Time Clock ----------------------------------*/
#include "rtc.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1
#define REG_INPUT_NREGS 50

/* ----------------------- Static variables ---------------------------------*/
USHORT   usRegInputStart = REG_INPUT_START;
SHORT   usRegInputBuf[REG_INPUT_NREGS];
/*------------------------ Configuration and Data Register Index --------------*/

#define CH1_THERMO 3
#define CH1_DW_LVL 4
#define CH1_UP_LVL 5

#define CH2_THERMO 0
#define CH2_DW_LVL 1
#define CH2_UP_LVL 2

#define OFFSET1    7
#define OFFSET2    6

#define CH1_ALERT  10
#define CH1_ALARM  11

#define CH2_ALERT  8
#define CH2_ALARM  9

#define CH1_RELAY_DELAY_ALERT 12
#define CH1_RELAY_DELAY_ALARM 13

#define CH2_RELAY_DELAY_ALERT 14
#define CH2_RELAY_DELAY_ALARM 15

#define CH1_STATUS 19
#define CH2_STATUS 20

#define CH1_TEMP   16
#define CH2_TEMP   17
#define ENV_TEMP   18
/*----------------------- Channel Configuration structure --------------------*/
CHANNEL ch[2];



int main(void)
{
	unsigned int state=1;
	short dummy;
	SCS |= 0x03;
	/*-----------Port Direction---------------*/
	DIODE1();
	DIODE2();
	/*----------------------------------------*/
	/*------Tempearture Measurement IC--------*/
	ads1247_init();
	/* ---------------------------------------*/
	/*-------Modbus RTU Protocol stack--------*/
    eMBInit(0x0A, 0, 9600, MB_PAR_NONE );
    /* Enable the Modbus Protocol Stack. */
    eMBEnable(  );
	delay_ms(1000);
	/*----------------------------------------*/
	/*----------------AD5422------------------*/
	AD5422_init();
	/*----------------------------------------*/
	/*TEST RTC*/
	rtc_init();	
		//Default settings
	usRegInputBuf[CH1_THERMO]=1;
	usRegInputBuf[CH2_THERMO]=1;
	usRegInputBuf[CH1_UP_LVL]=100;
	usRegInputBuf[CH1_DW_LVL]=0;
	usRegInputBuf[OFFSET1]=0;
	usRegInputBuf[CH2_UP_LVL]=100;
	usRegInputBuf[CH2_DW_LVL]=0;
	usRegInputBuf[OFFSET2]=0;
	usRegInputBuf[CH1_ALERT]=70;
	usRegInputBuf[CH1_ALARM]=100;
	usRegInputBuf[CH2_ALERT]=70;
	usRegInputBuf[CH2_ALARM]=100;
	NVOL_Init();
	NVOL_GetVariable(CH1_THERMO, (unsigned char* )&usRegInputBuf[CH1_THERMO], sizeof(unsigned short));
	NVOL_GetVariable(CH1_UP_LVL, (unsigned char* )&usRegInputBuf[CH1_UP_LVL], sizeof(unsigned short));
	NVOL_GetVariable(CH1_DW_LVL, (unsigned char* )&usRegInputBuf[CH1_DW_LVL], sizeof(unsigned short));
	NVOL_GetVariable(CH2_THERMO, (unsigned char* )&usRegInputBuf[CH2_THERMO], sizeof(unsigned short));
	NVOL_GetVariable(CH2_UP_LVL, (unsigned char* )&usRegInputBuf[CH2_UP_LVL], sizeof(unsigned short));
	NVOL_GetVariable(CH2_DW_LVL, (unsigned char* )&usRegInputBuf[CH2_DW_LVL], sizeof(unsigned short));
	NVOL_GetVariable(OFFSET1,    (unsigned char* )&usRegInputBuf[OFFSET1],    sizeof(unsigned short));
	NVOL_GetVariable(OFFSET1,    (unsigned char* )&usRegInputBuf[OFFSET2],    sizeof(unsigned short));
	NVOL_GetVariable(CH1_ALERT,  (unsigned char* )&usRegInputBuf[CH1_ALERT],  sizeof(unsigned short));
	NVOL_GetVariable(CH2_ALERT,  (unsigned char* )&usRegInputBuf[CH2_ALERT],  sizeof(unsigned short));
	NVOL_GetVariable(CH1_ALARM,  (unsigned char* )&usRegInputBuf[CH1_ALARM],  sizeof(unsigned short));
	NVOL_GetVariable(CH2_ALARM,  (unsigned char* )&usRegInputBuf[CH2_ALARM],  sizeof(unsigned short));

	ch[0].CurThermo = usRegInputBuf[CH1_THERMO];
	ch[0].up_lvl=usRegInputBuf[CH1_UP_LVL];
	ch[0].dw_lvl=usRegInputBuf[CH1_DW_LVL];
	ch[0].threshold_ART = usRegInputBuf[CH1_ALERT];
	ch[0].threshold_ALM = usRegInputBuf[CH1_ALARM];
	ch[0].temp=&usRegInputBuf[CH1_TEMP];
	ch[0].offset=usRegInputBuf[OFFSET1];
	ch[0].status=&usRegInputBuf[CH1_STATUS];
	ch[1].CurThermo = usRegInputBuf[CH2_THERMO];
	ch[1].up_lvl=usRegInputBuf[CH2_UP_LVL];
	ch[1].dw_lvl=usRegInputBuf[CH2_DW_LVL];	
	ch[1].threshold_ART = usRegInputBuf[CH2_ALERT];
	ch[1].threshold_ALM = usRegInputBuf[CH2_ALARM];
	ch[1].temp=&usRegInputBuf[CH2_TEMP];
	ch[1].offset=usRegInputBuf[OFFSET2];
	ch[1].status=&usRegInputBuf[CH2_STATUS];
 
	state = 5;

    for( ;; )
    {
		switch(state)
		{
		case 1:
			if(ch[0].CurThermo == NC)
				{state = 3;usRegInputBuf[CH1_STATUS] = Broken_connection;}
			else
				{if((usRegInputBuf[30]=ads1247_BurnoutCurrentSense(1))>10)
					{state=3;usRegInputBuf[CH1_STATUS]=Broken_connection;}
				else
					{ state=2;}}
			break;
		case 2:
			 if(ch[0].CurThermo == PT_100)
				{usRegInputBuf[CH1_STATUS]=PT100(1,&dummy);}
			 if(ch[0].CurThermo == thermocouple)
			 	{Thermocouple(1,&dummy,(short)usRegInputBuf[ENV_TEMP]);}
			 if((usRegInputBuf[27]=ads1247_BurnoutCurrentSense(1))<10)
				{if((dummy)<8000){	//Changes
					usRegInputBuf[CH1_TEMP]=dummy;
					usRegInputBuf[CH1_STATUS] = OK;}}
			 	else
					{usRegInputBuf[CH1_STATUS]=Broken_connection;}
			 if(ch[1].CurThermo == NC)
				{state=5;usRegInputBuf[CH2_STATUS]=Broken_connection;}
			 else
				{state=3;}
			break;
		case 3:
			if(ch[1].CurThermo == NC)
				{state = 5;usRegInputBuf[CH2_STATUS] = Broken_connection;}
			else
				{if((usRegInputBuf[29]=ads1247_BurnoutCurrentSense(2))>10)
					{state=5;usRegInputBuf[CH2_STATUS]=Broken_connection;}
				else
					{state=4;}}
			break;
		case 4:
			if(ch[1].CurThermo == PT_100)
				{usRegInputBuf[CH2_STATUS]=PT100(2,&dummy);}
			if(ch[1].CurThermo == thermocouple)
				{Thermocouple(2,&dummy,(short)usRegInputBuf[ENV_TEMP]);}	
			if((usRegInputBuf[26]=ads1247_BurnoutCurrentSense(2))<10)
				{if((dummy)<8000){
					usRegInputBuf[CH2_TEMP]=dummy;
					usRegInputBuf[CH2_STATUS] = OK;}}
			else
				usRegInputBuf[CH2_STATUS]=Broken_connection;
				state=5;
			break;
		case 5:
			if(!ds1820_read(&usRegInputBuf[ENV_TEMP]))
				{usRegInputBuf[CH1_STATUS] = Broken_connection; usRegInputBuf[CH2_STATUS] = Broken_connection;
					while(!ds1820_read(&usRegInputBuf[ENV_TEMP]));}
			if(ch[0].CurThermo == NC){state=3;usRegInputBuf[CH1_STATUS] = Broken_connection;
				if(ch[1].CurThermo == NC){state=5;usRegInputBuf[CH2_STATUS] = Broken_connection;}}
			else{state=1;}
			break;
		default :
			break;
		}
		 
		if(usRegInputBuf[CH1_STATUS] != Broken_connection){
			VICIntEnClr |= 0x2000;//Disable RTC Int
			if((ch[0].threshold_ART+2000) <(usRegInputBuf[CH1_TEMP]+2000))
				{if((ch[0].threshold_ALM+2000) < (usRegInputBuf[CH1_TEMP]+2000))
					{FIO0PIN&=~D2Y;FIO1PIN&=~D2G;FIO0PIN|=D2R;
					usRegInputBuf[CH1_STATUS]=ALARM;}										//ALARM
			 	else
					{SET_D2Y();
					usRegInputBuf[CH1_STATUS]=ALERT;}}									 	//ALERT
			else
			{FIO0PIN&=~D2Y;FIO1PIN|=D2G;usRegInputBuf[CH1_STATUS]=OK;}}else{FIO0PIN&=~(D2Y|D2R);VICIntEnable |= 0x2000;} //Enable RTC Int

		if(usRegInputBuf[CH2_STATUS] != Broken_connection){
			VICIntEnClr |= 0x2000;//Disable RTC Int
			if((ch[1].threshold_ART+2000) < (usRegInputBuf[CH2_TEMP]+2000))
				{if((ch[1].threshold_ALM+2000) < (usRegInputBuf[CH2_TEMP]+2000))
					{FIO0PIN&=~D1Y;FIO0PIN&=~D1G;FIO1PIN|=D1R;
					usRegInputBuf[CH2_STATUS]=ALARM;}									 	//ALARM
			 	else
			 		{SET_D1Y();
					usRegInputBuf[CH2_STATUS]=ALERT;}}										//ALERT
			else
			{SET_D1G(); usRegInputBuf[CH2_STATUS]=OK;}}else{FIO0PIN&=~D1Y;FIO1PIN&=~D1R;VICIntEnable |= 0x2000;} //Enable RTC Int  

		AD5422_SetOutputVorI(ch);
																							 
        ( void )eMBPoll(  );	
    }
return 0;
}


eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
     eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;
if(eMode==0){	
    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
}
if(eMode==1){
	if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
		
		iRegIndex = (int) (usAddress - usRegInputStart);
		while(usNRegs > 0)
		{
		  usRegInputBuf[iRegIndex]	  = ( unsigned short )( (*pucRegBuffer++)<<8 );
		  usRegInputBuf[iRegIndex]   |= ( unsigned short )( (*pucRegBuffer++));
		  iRegIndex++; 
            usNRegs--;
		}
	NVOL_SetVariable(CH1_THERMO, (unsigned char* )&usRegInputBuf[CH1_THERMO], sizeof(unsigned short));
	NVOL_SetVariable(CH1_UP_LVL, (unsigned char* )&usRegInputBuf[CH1_UP_LVL], sizeof(unsigned short));
	NVOL_SetVariable(CH1_DW_LVL, (unsigned char* )&usRegInputBuf[CH1_DW_LVL], sizeof(unsigned short));
	NVOL_SetVariable(CH2_THERMO, (unsigned char* )&usRegInputBuf[CH2_THERMO], sizeof(unsigned short));
	NVOL_SetVariable(CH2_UP_LVL, (unsigned char* )&usRegInputBuf[CH2_UP_LVL], sizeof(unsigned short));
	NVOL_SetVariable(CH2_DW_LVL, (unsigned char* )&usRegInputBuf[CH2_DW_LVL], sizeof(unsigned short));
	NVOL_SetVariable(OFFSET1,    (unsigned char* )&usRegInputBuf[OFFSET1],    sizeof(unsigned short));
	NVOL_SetVariable(CH1_ALERT , (unsigned char* )&usRegInputBuf[CH1_ALERT] , sizeof(unsigned short));
	NVOL_SetVariable(CH2_ALERT , (unsigned char* )&usRegInputBuf[CH2_ALERT] , sizeof(unsigned short));
	NVOL_SetVariable(CH1_ALARM , (unsigned char* )&usRegInputBuf[CH1_ALARM] , sizeof(unsigned short));
	NVOL_SetVariable(CH2_ALARM , (unsigned char* )&usRegInputBuf[CH2_ALARM] , sizeof(unsigned short));

	ch[0].CurThermo = usRegInputBuf[CH1_THERMO];
	ch[0].up_lvl=usRegInputBuf[CH1_UP_LVL];
	ch[0].dw_lvl=usRegInputBuf[CH1_DW_LVL];
	ch[1].CurThermo = usRegInputBuf[CH2_THERMO];
	ch[1].up_lvl=usRegInputBuf[CH2_UP_LVL];
	ch[1].dw_lvl=usRegInputBuf[CH2_DW_LVL];
	ch[1].threshold_ART = usRegInputBuf[CH2_ALERT];
	ch[0].threshold_ART = usRegInputBuf[CH1_ALERT];
	ch[1].threshold_ALM = usRegInputBuf[CH2_ALARM];
	ch[0].threshold_ALM = usRegInputBuf[CH1_ALARM];	
	ch[0].offset=usRegInputBuf[OFFSET1];
	ch[1].offset=usRegInputBuf[OFFSET2];
	//AD5422_SelectRange(0,ch1.dw_lvl,ch1.up_lvl);
	}
}
    return eStatus;
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,eMBRegisterMode eMode  )
{

    return 0;
}

eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNCoils;
    ( void )eMode;
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}


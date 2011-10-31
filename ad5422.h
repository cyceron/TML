#ifndef _AD5422
#define _AD5422

/*ADDRESS BYTE FUNCTIONS */

#define AD_NOP 			0x00
#define DATA_REGISTER 	0x01
#define READBACK		0x02
#define CNTRLREG		0x55
#define RSTREG			0x56

/*  READ ADDRESS DECODING*/

#define RSTATUSREG 		0x00
#define RDATAREG		0x01
#define RCNTRLREG		0x02

/*  OUTPUT RANGE OPTIONS */

#define _0_5V 			0x00
#define _0_10V			0x01
#define _4_20mA			0x05
#define _0_20mA			0x06
#define _0_24mA			0x07

/* CONNECTED PINS */

#define ILATCH 			(1<<20)		//P1.20
#define FAULT2			(1<<15)		//P0.15
#define FAULT1			(1<<16)		//P0.16

/* MACROs 		  */
#define DIR_LATCH()		(FIO1DIR |= ILATCH)
#define SET_LATCH()		(FIO1PIN |= ILATCH)
#define CLR_LATCH() 	(FIO1PIN &= ~ILATCH)

#define CHECK_FAULT(x)	(FIO0PIN & (~x))
/* Declaration */
enum thermomether{
	PT_100=1,
	thermocouple,
	NC
	};
enum status{
	OK=1,
	ALERT,
	ALARM,
	Broken_connection
	};

typedef struct Channel{
enum thermomether CurThermo;
int up_lvl;
int dw_lvl;
unsigned int threshold_ART;
unsigned int threshold_ALM;
unsigned short offset;
short *temp;
short *status;
}CHANNEL;

/* FUNCTION DECLARATION */

int AD5422_init(void);
char conf_ad5422(void);
unsigned int AD5422_SetOutputVorI(CHANNEL*);


#endif

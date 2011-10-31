#ifndef _ADS1247
#define _ADS1247

#define MUX0	0x00 //Multiplexer Control Register 0
#define VBIAS 	0X01 //Bias Voltage Register
#define MUX1	0x02 //Multiplexer Control REgister 1
#define SYS0	0x03 //System Control Register 0
#define OFC0	0X04 //Offset Calibration Coefficient Register 0
#define OFC1	0x05 //Offset Calibration Coefficient Register 1
#define OFC2	0x06 //Offset Calibration Coefficient Register 2
#define FSC0 	0x07 //Full scale Callibration Coefficient Register 0
#define FSC1	0x08 //Full scale Callibration Coefficient Register 1
#define FSC2	0x09 //Full scale Callibration Coefficient REgister 2
#define IDAC0	0x0A //IDAC Control Register 0
#define IDAC1	0x0B //IDAC Control Register 1
#define GPIOCFG 0x0C //GPIO Configuration Register
#define GPIODIR 0x0D //GPIODirection REgister
#define GPIODAT 0x0E //GPIO Data Register

//SPI COMMAND DEFINITIONS

/*SYSTEM CONTROL */

#define		WAKEUP		0x00 	//Exit Sleep Mode
#define 	SLEEP 		0x01	//Enter Sleep Mode
#define 	SYNC 		0x04	//Synchornize the A/D Conversion
#define 	RESET		0x06	//Reset To Power UP values
#define 	ADS_NOP			0xff	//NO Operation

/*DATA READ*/

#define		RDATA		0x12	//Read Data at Once
#define 	RDATAC		0x14	//Read Data Continously
#define 	SDATAC 		0x16	//Stop Reading Data Continously

/*READ REGISTER */
#define 	RREG		0x20	//Read From Register
#define 	WREG 		0x40	//Write To Register

/*Calibration */
#define 	SYSOCAL		0x60	//System Offset Calibration
#define 	SYSGCAL		0x61	//System Gain Calibration
#define 	SELFOCAL	0x62	//Self Offset Calibration

/*Function */
void ads1247_init(void);
unsigned int ads1247_BurnoutCurrentSense(unsigned char channel);
unsigned char ads1247_readData(void);
float PT100(unsigned char,short *);
float Thermocouple(unsigned char,short *, short env_temp);
	

#endif

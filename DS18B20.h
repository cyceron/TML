#ifndef __DS18B20__
#define __DS18B20__

/* ----------------------- Start implementation -----------------------------*/
#define DQ 25
#define DQ_pin			(1<<25)
#define input()			(FIO0PIN & DQ_pin)
#define output_float()	(FIO0DIR = FIO0DIR & (~DQ_pin))

void delay_us (unsigned int usec);
void delay_ms(unsigned int msec);
char onewire_reset(void);
void onewire_write(unsigned char data);
char onewire_read(void);
unsigned int ds1820_read(short* temp);


#endif

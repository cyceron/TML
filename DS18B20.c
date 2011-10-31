
#include <lpc213x.h>
#include "DS18b20.h"

void __inline output_low()
{
 FIO0DIR = FIO0DIR | DQ_pin ;
 FIO0CLR	= DQ_pin ;
}
unsigned int Timer_W8(int flag)
{
return (T1IR & flag);
}
void delay_us (unsigned int usec)
{
 T1TCR = 0x02; // Reset
 T1MCR = 0x07; // stop, reset and Interrupt
 T1PR  = 15;
 T1MR0 = usec; // Wait this long
 T1TCR = 0x01; // Go
 while(!(T1IR & 1)); // Wait for int flag
 T1IR = 0x01; // Clear interrupt
}

void delay_ms(unsigned int msec)
{
 T1TCR = 0x02; // Reset
 T1MCR = 0x07; // stop, reset and Interrupt
 T1PR  = 15000-1;
 T1MR0 = msec; // Wait this long
 T1TCR = 0x01; // Go
 while(!(T1IR & 1)); // Wait for int flag
 T1IR = 0x01; // Clear interrupt
}


//-------------------------1wire reset----------------------------

char onewire_reset() //works as expected....
{
	char sonuc;
 output_low();
   delay_us( 500 ); // pull 1-wire low for reset pulse
 output_float();    // float 1-wire high
   delay_us( 70 );  // wait-out remaining initialisation window.
 sonuc = input();	// master sampling
   if (sonuc != 0) return 1;//NOT OK
   delay_us(430);
 return 0;	//OK
}
#define Presc	14
//--------------------------16 bit write ----------------------------
void onewire_write(unsigned char data)	// 6, 54, 10
{
 char count;
#define AB	6
#define BC	60
#define DD	10
#define M0_i	1	           // Bit <2:0> <s:r:i>   Stop,Reset,Interrupt
#define M1_i	1<<3           // Bit <5:3> <s:r:i>   "
#define	M2_sri	7<<6           // Bit <8:6> <s:r:i>   "
 T1PR  = Presc;		           // Prescale 15, 60 MHz CClock, 15 MHz PCLCK
 T1MR0 = AB;                   // Wait this long
 T1MR1 = AB+BC;                // Wait this long
 T1MR2 = AB+BC+DD ;            // Wait this long
 T1MCR = M0_i | M1_i | M2_sri; // Interrupt,stop and reset settings of Match regs

 T1TCR = 0x02; // Reset

  for (count=0; count<8; count++)
 {
  output_low();
    T1TCR = 0x01;              // timer1 starts
    while(!(T1IR & 1));        //while(T1MR0!=T1TC); // Wait for int flag
  if ( (data&1) != 0 ) output_float(); // write 0 or 1
    while(!(T1IR & 2));        //while(T1MR1!=T1TC); // Wait for int flag
  output_float();              // set 1-wire high again,
    while(!(T1IR & 4));        //while(T1MR2!=T1TC); // Wait for int flag
    T1IR = 0x07; // Clear M2, M1, M0 interrupts
    data >>= 1;
 }
}

//-------------------------- 16 bit read ----------------------------

char onewire_read()   // 6, 9, 55
{
 unsigned char count, data=0;
#define AA	4
#define EE	10
#define FF	55
#define M0_i	1	           // Bit <2:0> <s:r:i>   i = interrupt
#define M1_i	1<<3           // Bit <5:3> <s:r:i>   r = reset
#define	M2_sri	7<<6           // Bit <8:6> <s:r:i>   s = stop
 T1PR  = Presc;		           // Prescale 15 60 MHz CClock, 15 MHz PCOLCK
 T1MR0 = AA;                   // Wait this long
 T1MR1 = AA+EE;                // Wait this long
 T1MR2 = AA+EE+FF ;            // Wait this long
 T1MCR = M0_i | M1_i | M2_sri; // Interrupt,stop and reset

 T1TCR = 0x02; // Reset

  for (count=0; count<8; count++)
 {
    data >>=1;
  output_low();
    T1TCR = 0x01;              	// timer1 starts
    while(!(T1IR & 1));        	//while(T1MR0!=T1TC); // Wait for int flag
  output_float();              	// now let 1-wire float (high by pullup resistor)
    while(!(T1IR & 2));        	//while(T1MR1!=T1TC); // Wait for int flag for master sampling
  if( input() != 0 ) data |= 0x80;
    while(!(T1IR & 4));        	//while(T1MR2!=T1TC); // Wait for int flag
    T1IR = 0x07; 				// Clear M2, M1, M0 interrupts
 }
 return( data );
}
//----------------------------------------------------------------------
unsigned int ds1820_read(short* temp)
{
 char busy=0, i;
 static unsigned int state=9999,valid=0;
 static char t2, t1;
 int  tt=0;

switch (state)
 {
 case 0:
 	i=onewire_reset();
 if(i==1) { return (valid=0);}else{state=1;}
	break;
 case 1:
 	onewire_write(0xCC); 
	state=2;
	break;
 case 2:
	onewire_write(0x44); 
	state=3;
	break;
 case 3:
    busy = onewire_read();
	if(busy != 0)state =4;
	break;
 case 4: 
 	onewire_reset();
	state =5;
	break;
 case 5:
 	onewire_write(0xCC);
	state =6;
	break;
 case 6:
 	onewire_write(0xBE);
	state =7;
	break;
 case 7:
 	t1 = onewire_read();
	state =8;
	break;
 case 8:
 	t2 = onewire_read();
	state =0;
	tt= ((int) t2<<8) | ((int)t1 & 0xFF);
	if((-3300<tt)&&(tt<7000))
	{
 		*temp =(short)( (100.0*((float)tt / 16.0)));
		valid=1;
		}
	break;
 default:
 	state=0;
	break;
 }
 return(valid);
}



#ifndef _RTC_
#define _RTC_
/*------------------------ RGB Diode        ---------------------------------*/
#define D2R		 (1<<14)	//P0.14
#define D2G		 (1<<22)	//P1.22
#define D2Y		 (1<<13)	//P0.13

#define D1R		 (1<<23) 	//P1.23
#define D1G		 (1<<11) 	//P0.11
#define D1Y		 (1<<12) 	//P0.12

#define DIODE1() FIO1DIR|=D1R;FIO0DIR|=D1G;FIO0DIR|=D1Y
#define DIODE2() FIO0DIR|=D2R;FIO0DIR|=D2Y;FIO1DIR|=D2G

#define CLR_D2()  FIO1PIN&=~D2G;FIO0PIN&=~D2R;FIO0PIN&=~D2Y
#define SET_D2R() FIO0PIN&=~D2Y;FIO1PIN&=~D2G;FIO0PIN|=D2R;//;FIO1PIN&=~D2G
#define SET_D2G() FIO0PIN&=~D2Y;FIO0PIN&=~D2R;FIO1PIN|=D2G;
#define SET_D2Y() FIO0PIN&=~D2R;FIO1PIN&=~D2G;FIO0PIN|=D2Y;

#define CLR_D1()  FIO1PIN&=~D1R;FIO0PIN&=~D1G;FIO0PIN&=~D1Y
#define SET_D1G() FIO0PIN&=~D1Y;FIO1PIN&=~D1R;FIO0PIN|=D1G
#define SET_D1R() FIO0PIN&=~D1Y;FIO0PIN&=~D1G;FIO1PIN|=D1R
#define SET_D1Y() FIO1PIN&=~D1R;FIO0PIN&=~D1G;FIO0PIN|=D1Y


void rtc_init(void);
void rtc_isr(void) __attribute__ ((interrupt("IRQ")));
#endif

#ifndef _DIODE_
#define DIODE

#define D2R		(1<<14)		//P0.14	 Second diode Red colour
#define D2G		(1<<22)		//P1.22	 Second diode Green Colour
#define D2Y		(1<<13)		//P0.13	 Second diode Yellow Colour

#define D1R		(1<<23)		//P1.23	 First diode Red colour
#define D1G		(1<<11)		//P0.11	 First diode Greeen Colour
#define D1Y		(1<<12)		//P0.12	 First diode Yellow Colour

/* MACROS 		*/

#define CLR_D2R	FIO0PIN |= D2R
#define CLR_D2G FIO1PIN |= D2G
#define CLR_D2Y FIO0PIN |= D2Y

#define SET_D2R FIO0PIN &= ~D2R
#define SET_D2G FIO1PIN &= ~D2G
#define SET_D2Y FIO0PIN &= ~D2Y

#define CLR_D1R FIO1PIN |= D1R
#define CLR_D1G FIO0PIN |= D1G
#define CLR_D1Y FIO0PIN |= D1Y

#define SET_D1R FIO1PIN &= ~D1R
#define SET_D1G FIO0PIN &= ~D1G
#define SET_D1Y FIO0PIN &= ~D1Y

#define GPIO_IN_0(X)  (FIO0DIR &= ~X)
#define GPIO_OUT_0(X) (FIO0DIR |= X)
#define GPIO_IN_1(X)  (FIO1DIR &= ~X)
#define GPIO_OUT_1(X) (FIO1DIR |= X)

#endif
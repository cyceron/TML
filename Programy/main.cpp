#include <cstdlib>
#include <iostream>

using namespace std;

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

int main(int argc, char *argv[])
{
    unsigned short mb;
    int sign=1;
    volatile unsigned char tab[]={0x01,0x13,0x00};
    volatile unsigned int temp;
    volatile double dummy,t;
    tab[0]=104;
    tab[1]=63;
    tab[2]=128;
    temp = (tab[0]<<16) | (tab[1]<<8) | tab[2];
		if(0x800000 & temp)
		{sign=-1;
		temp = (~temp)&0xffffff;}

		dummy = ((temp*2048.0)/0x7fffff)*1.0;
		dummy = dummy/64000.0;
		dummy=0.018516;
		t= A0  +A1*dummy 
					  	+A2*(dummy*dummy) 
					  	+A3*(dummy*dummy*dummy) 
					  	+A4*(dummy*dummy*dummy*dummy) 
					  	+A5*(dummy*dummy*dummy*dummy*dummy) 
					  	+A6*(dummy*dummy*dummy*dummy*dummy*dummy)
					  	+A7*(dummy*dummy*dummy*dummy*dummy*dummy*dummy)
					  	+A8*(dummy*dummy*dummy*dummy*dummy*dummy*dummy*dummy);
    cout<<"temperatura  "<<t<<"Voltage : "<<dummy;
    
    short tt;
    tt=0x9101;
    if((-3300<tt)&&(tt<7000))
    cout<<endl<<"OK"<<endl<<tt<<endl;
    system("PAUSE");
    return EXIT_SUCCESS;
}

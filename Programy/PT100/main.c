
#include <stdio.h > // Standard I/O so we can use the printf function
#include <iostream>
#include <math.h>
using namespace std;
/*----------------------- Linearization coefficient for Pt100----------------------------*/
const  double Z1 = -0.0039083;
const  double Z2 = 0.00001758480889;
const  double Z3 = -0.00000002310;
const  double Z4 = -0.000001155;
int main(void) {
    
unsigned char tab[3];
double temp, dummy,err;
tab[0]=84;
tab[1]=87;
tab[2]=150;
err=169.0;
     if(!(tab[0] & 0x80)){
		temp = (tab[0]<<16) | (tab[1]<<8) | tab[2];
		dummy = ((temp*2475.0)/0x7fffff)*1.0;
		temp=dummy=((dummy/12.0)+err);
		dummy=((Z1+sqrt((Z2+Z3*dummy)))/Z4);}

		if(tab[0] & 0x80){
		temp =((~tab[0] & 0x7f)<<16) | ((~tab[1] & 0xff)<<8) | ((~tab[2] & 0xff));
		dummy = ((temp*2475.0)/0x7fffff)*1.0;
		temp=dummy=(err-(dummy/12.0));

		dummy=((Z1+sqrt((Z2+Z3*dummy)))/Z4);}
		cout<<"temperatura : "<<dummy<<"  "<<temp<<endl;
     system("PAUSE");
     return 0;
}

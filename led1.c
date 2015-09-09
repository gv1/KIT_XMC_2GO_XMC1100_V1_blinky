/* Original files from : http://eleceng.dit.ie/frank/arm/BareMetalXMC2Go/index.html */

#include "xmc1100.h"
void delay(unsigned len)
{
	while(len--);
}
void initPorts()
{
	// LED's are on P1.0 and P1.1
	// So make these push-pull outputs
	P1_IOCR0 = BIT15 + BIT7;
}
void main()
{
	initPorts();
	while(1)
	{
		P1_OUT = 0x00;
		delay(180000);
		P1_OUT = 0x01;
		delay(180000);
	};
}

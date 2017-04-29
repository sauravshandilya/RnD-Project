#include <stdio.h>
#include <msp430.h> 

/*
 * main.c
 */
int main(void)
{
    volatile unsigned int i ;
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |= 0x01;

    while(1)
    {
    	P1OUT ^= 0x01;
    	i = 10000;
    	do i--;
    	while(i!=0);

    }

	return 0;
}

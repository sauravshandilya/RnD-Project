/*
 * my_delay.c
 *
 *  Created on: 30-Mar-2017
 *      Author: saurav
 *
 */

#include <msp430.h>

void delay_sec(unsigned int number_of_seconds)
{
	TA0CCTL0 |= CM_0 + CCIE;	//Enable compare mode + Interrupts on Timer
	TA0CTL |= TASSEL_1;			//Use ACLK as source for timer - 32KHz

	unsigned int i = 0;
	unsigned int delayCycles = 32000; // for 1 sec delay Number of cycles in the timer

	TA0CCR0 = delayCycles;
	TA0CTL |= MC_1;			//Use UP mode timer

	for (i = 0; i<number_of_seconds; i++)
	{
	  TA0R = 0;				// Timer/counter register
          while (TA0R < delayCycles)
	  {
	    __bis_SR_register(LPM3_bits + GIE); //Enter Low Power Mode 3 with interrupts
	    __no_operation();
	  }
	}
}

void delay_millisec(unsigned int number_of_milliseconds)
{
	TA0CCTL0 |= CM_0 + CCIE;	//Enable compare mode + Interrupts on Timer
	TA0CTL |= TASSEL_1;			//Use ACLK as source for timer - 32KHz

	unsigned int i = 0;
	unsigned int delayCycles = 32; // for 1 milli sec delay Number of cycles in the timer

	TA0CCR0 = delayCycles;
	TA0CTL |= MC_1;			//Use UP mode timer

	for (i = 0; i<number_of_milliseconds; i++)
	{
	  TA0R = 0;				// Timer/counter register
	  while (TA0R < delayCycles)
	  {
	    __bis_SR_register(LPM3_bits + GIE); //Enter Low Power Mode 3 with interrupts
	    __no_operation();
	  }
	}
}




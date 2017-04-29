// blink both led - using driverlib

//#include <msp430.h>
#include <driverlib.h>
#include "rtc.h"

/*
 * main.c
 */
int main(void)
{
    WDT_A_hold(WDT_A_BASE);			//stop watchdog timer

    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);		//Red LED
    GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN7);		//Green LED

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2,GPIO_PIN1);	// S1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN1);	// S2

    volatile unsigned short usiButton1 = 0;
    volatile unsigned short usiButton2 = 0;

    while(1)
    {
    	usiButton1 = GPIO_getInputPinValue(GPIO_PORT_P2,GPIO_PIN1);	// s1
    	usiButton2 = GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1);	// s2

    	if(usiButton1 == GPIO_INPUT_PIN_LOW)
    	{
    		GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);	// red led on
    	}

    	if (usiButton2 == 0)
    	{
    		GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);	// green led on
		}

    	else
    	{
    		GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);		// red led off
    		GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);		// green led off
    	}
    }


	return 0;
}

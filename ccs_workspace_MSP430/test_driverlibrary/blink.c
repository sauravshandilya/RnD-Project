// blink both led - using driverlib

//#include <msp430.h>
#include <driverlib.h>

/*
 * main.c
 */
int main(void)
{
    WDT_A_hold(WDT_A_BASE);			//stop watchdog timer

    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_PJ,GPIO_PIN_ALL8);
    while(1)
    {
    	GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);
    	GPIO_setOutputHighOnPin(GPIO_PORT_P4,GPIO_PIN7);
    	_delay_cycles(800000);
    	GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0);
    	GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN7);
    	_delay_cycles(800000);
    }


	return 0;
}

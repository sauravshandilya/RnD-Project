#include <msp430.h> 

/*
 * main.c
 */

void uart_init(void)
{
	P4SEL |= 0x30; 					//P4.4 and P4.5 USCI_A1 Tx and Rx
	UCA1CTL1 |= UCSWRST; 			// reset
	UCA1CTL1 |= UCSSEL_1;			// ACLK 32768

	UCA1BR0 = 3;					// baud rate 9600
	UCA1BR1 = 0x00;					// baud rate

	UCA1MCTL = 0x06;				//UCBRSx UCBRFx
	UCA1CTL0 = 0x00;

	UCA1CTL1 &= ~UCSWRST; 			// set
}

int main(void)
{
    volatile unsigned int i ;
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
	uart_init();

	char welcome_message[] = "Welcome to Serial Programming";

//    P1DIR |= 0x01;

    while(1)
    {
    	i = 0;

    	while(welcome_message[i]!='\0')
    	{
    		UCA1TXBUF = welcome_message[i];
    		while(UCA1STAT & UCBUSY);
    		i++;
    	}

    	UCA1TXBUF = 0x0a;				// newline
    	while(UCA1STAT & UCBUSY);

    	UCA1TXBUF = 0x0d;				// carriage return
    	while(UCA1STAT & UCBUSY);

    }

	return 0;
}

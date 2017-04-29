#include <msp430.h> 

/*
 * main.c
 *
 * testing UART 1 (over USB) - No delay..
 */

void uart_init(void)
{
	P4SEL |= 0x30; 					//P4.4 and P4.5 USCI_A1 Tx and Rx
	UCA1CTL1 |= UCSWRST; 			// reset

	// ACLK 32768 Hz
	UCA1CTL1 |= UCSSEL_1;

	// baud rate 9600
	UCA1BR0 = 3;
	UCA1BR1 = 0x00;

	UCA1MCTL = 0x06;				//UCBRSx UCBRFx
	UCA1CTL0 = 0x00;

	UCA1CTL1 &= ~UCSWRST; 			// set

	UCA1IE |= UCRXIE;				// Enable USCI_A1 RX interrupt

//	__bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    while (!(UCA1IFG & UCTXIFG));             // USCI_A0 TX buffer ready?
//    UCA1TXBUF = UCA1RXBUF;                  // TX -> RXed character
    UCA1TXBUF = (UCA1RXBUF + 1);                  // TX -> RXed character
}

/*
 * uart_char()
 * Description: Print single character
 * Example uart_char('A'); prints letter A
 */
void uart_char(char data)
{
	UCA1TXBUF = data;
	while(UCA1STAT & UCBUSY);
}

/*
 * uart_string()
 * Description: Print string
 * Example: uart_string("Hello World");
 */
void uart_string(char *g)
{
	while(*g)
		uart_char(*g++);
	//UARTCharPut(UART0_BASE, '\n');
	//UARTCharPut(UART0_BASE, '\r');
}

int main(void)
{
    volatile unsigned int i ;
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
	P1DIR |= 0x01;				// Port 1 pin 0 - LED1
	P4DIR |= 0b10000000;		// Port 4 pin 7 - LED2

	uart_init();

	char welcome_message[] = "Welcome !!";

	while(welcome_message[i]!='\0')
	{
		UCA1TXBUF = welcome_message[i];
		while(UCA1STAT & UCBUSY);
	  		i++;
	}
	while(1)
	    {
			uart_char('S');
			uart_char('\n');
			uart_char('\r');
			uart_string("Hello");
			uart_char('\n');
			uart_char('\r');
	    }
}

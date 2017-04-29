/*
 * my_uart0.c
 *
 * UART 1 header file - connected to USB and on isolation header.
 *  Created on: 30-Mar-2017
 *      Author: saurav
 */

#include "commonheader.h"

void uart0_init(void)
{
	P3SEL |= 0x18; 					//P3.3 and P3.4 USCI_A0 Tx and Rx
	UCA0CTL1 |= UCSWRST; 			// reset
	UCA0CTL1 |= UCSSEL_1;			// ACLK 32768

	UCA0BR0 = 3;					// baud rate 9600
	UCA0BR1 = 0x00;					// baud rate

	UCA0MCTL = 0x06;				//UCBRSx UCBRFx
	UCA0CTL0 = 0x00;

	UCA0CTL1 &= ~UCSWRST; 			// set

	UCA0IE |= UCRXIE;				// Enable USCI_A1 RX interrupt
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    while (!(UCA0IFG & UCTXIFG));             // USCI_A0 TX buffer ready?
//    UCA1TXBUF = UCA1RXBUF;                  // TX -> RXed character
    UCA0TXBUF = (UCA0RXBUF + 1);                  // TX -> RXed character
}

/*
 * uart_char()
 * Description: Print single character
 * Example uart_char('A'); prints letter A
 */
void uart_char(char data)
{
	UCA0TXBUF = data;
	while(UCA0STAT & UCBUSY);
}

/*
 * uart_string()
 * Description: Print string
 * Example: uart_string("Hello World");
 */
void uart_string(char *string)
{
	while(*string)
		uart_char(*string++);
	//UARTCharPut(UART0_BASE, '\n');
	//UARTCharPut(UART0_BASE, '\r');
}

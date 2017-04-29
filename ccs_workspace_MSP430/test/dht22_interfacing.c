/*
 * dht22_interfacing.c
 *
 *  Created on: 02-Apr-2017
 *      Author: saurav
 */

#include <msp430.h>
#include <stdio.h>
#include "my_uart1.h"
#include "driverlib.h"
#include "my_delay.h"

#include "mydht22.h"

#define DHTPIN P2_0

bool flag;

GPIO_

void main(void)
{
	begin();
//	flag = get();

	while(1)
	{
//		delay_sec(2);
		// Reading temperature or humidity takes about 250 milliseconds!
		// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
		flag = get();
		int32_t h = humidityX10();
		int32_t t = temperatureX10();
		printf("%d,%d", t, h);
		delay_sec(2);
	}
}
/*
char txbuf[256];
int t = 0;
int h = 0;

void uart_send(int len)
{
  int i;
  for(i = 0; i < len; i++) {
    while (UCA1STAT & UCBUSY);
    UCA1TXBUF = txbuf[i];
  }
}

int main()
{
  WDTCTL = WDTPW | WDTHOLD;
//  DCOCTL = 0;
//  BCSCTL1 = CALBC1_16MHZ;
//  DCOCTL = CALDCO_16MHZ;

  uart1_init();

  _BIS_SR(GIE);

  while(1)
  {
	uart_string("Reading..");
    dht_start_read();
    int t = dht_get_temp();
    int h = dht_get_rh();

    printf("reading \n");
    printf("%d %d\r\n", t,h);
    delay_millisec(2000);
//    uart_send(sprintf(txbuf, "%3d.%1d C; %3d.%1d %%RH\r\n", t/10, t%10, h/10, h%10));
  }
}

*/

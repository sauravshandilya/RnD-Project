/*
 * mydht22.c
 *
 *  Created on: 03-Apr-2017
 *      Author: saurav
 */

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "commonheader.h"
#include "my_delay.h"
#include "driverlib.h"

#if defined(__MSP430G2452__) || defined(__MSP430G2553__) || defined(__MSP430G2231__) \
	|| defined(__MSP430F5529__) || defined (__MSP430FR5739) || defined (__MSP430FR5969__) // MSP430 family
	#define DHT22_DELAY_US 3
	#define DHT22_CYCLES_CNT 8
#else // Other boards, tested only with Stellaris Launchpad (EK-LM4F120XL)
	#define DHT22_DELAY_US 1
	#define DHT22_CYCLES_CNT 13
#endif

#define MAXTIMINGS 85

uint8_t data[6];
uint8_t _pin;
uint32_t _lastMillis;
bool _lastResult;
uint32_t _temperature, _humidity;

//void init(uint8_t pin)
//{
//  _pin = pin;
//
//}

void begin(void)
{
	_lastMillis = 0;
	_lastResult = false;

	P2DIR &= ~BIT0;			// Input
	//pinMode(_pin, INPUT);
	P2OUT |= 0x01;			// high
	//	digitalWrite(_pin, HIGH);
}

int32_t temperatureX10(void)
{
  return _temperature;
}

int32_t humidityX10(void)
{
  return _humidity;
}


bool get()
{
  uint8_t lastState = 1;
  uint16_t counter = 0;
  uint8_t j = 0, i;

  // sensor frequency = 0.5 Hz
  // check last time + 2000 ms

//  if (millis() < _lastMillis)
//  {
//    return _lastResult; // return last correct measurement
//  }
//
//  _lastMillis = millis() + 2000;

  _temperature = 0;
  _humidity    = 0;
  _lastResult  = false;

  // pull the pin high and wait 250 milliseconds
//  digitalWrite(_pin, HIGH);
//  delay(250);
  	  P2OUT |= 0x01;
  	  delay_millisec(250);

  	data[0] = 0;
  	data[1] = 0;
  	data[2] = 0;
  	data[3] = 0;
  	data[4] = 0;

  	// now pull it low for ~20 milliseconds
  	//  pinMode(_pin, OUTPUT);
  	//  digitalWrite(_pin, LOW);
  	//  delay(20);
  	P2OUT &= ~BIT0;
  	delay_millisec(20);


  //noInterrupts();

//  digitalWrite(_pin, HIGH);
  P2OUT |= 0x01;
//  delayMicroseconds(40);
  delay_30micro(2);

  //pinMode(_pin, INPUT);
  P2DIR &= ~ BIT0;

  // read in timings
  for ( i=0; i< MAXTIMINGS; i++)
  {
    counter = 0;
    while (P2IN == lastState)
    {
      counter++;

      // LaunchPad implementation
      // LaunchPad faster than Arduino
      // 1. replace delayMicroseconds(1) with delayMicroseconds(3)
      // or
      // 2. compare counter to a higher number
      // by energia » Tue Jun 26, 2012 9:24 pm
      // see http://www.43oh.com/forum/viewtopic.php?p=20821#p20821

//      delayMicroseconds(DHT22_DELAY_US);
      delay_30micro(1);

      if (counter == 255)
      {
        break;
      }
   }

    lastState = P2IN;

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      data[j/8] <<= 1;

      // 1. replace delayMicroseconds(1) with delayMicroseconds(3)
      // or
      // 2. compare counter to a higher number
      // by energia » Tue Jun 26, 2012 9:24 pm
      // see http://www.43oh.com/forum/viewtopic.php?p=20821#p20821
      //      if (counter > 8)
      if (counter > DHT22_CYCLES_CNT)
           data[j/8] |= 1;
      j++;
    }
  }

//  interrupts();
   // check we read 40 bits and that the checksum matches
  if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xff)) ) {

    _temperature = data[2] & 0x7f;
    _temperature *= 256;
    _temperature += data[3];
    if (data[2] & 0x80) _temperature *= -1;

    _humidity = data[0];
    _humidity *= 256;
    _humidity += data[1];

    _lastResult = true;
  }

  return _lastResult;
}

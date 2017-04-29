#include <driverlib.h>
#include <stdint.h>
#include <msp430.h>
#include "my_adc.c"

void measure_bat_voltage(void)
{
  unsigned int data=0;
  float voltage=0;
  configure_ADC_12();
  //ADC12CTL0 |= ADC12ENC;                    // Enable conversions
      ADC12CTL0 |= ADC12SC;                     // Start conversion
      
      while (!(ADC12IFG & BIT0));
          data=ADC12MEM0;
          voltage = (0.009 * data);
          Serial.print ("data=");
          Serial.println(data);
          Serial.print("voltage=");
          Serial.println(voltage);     
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  Serial.println("sss");
  while(1){
  measure_bat_voltage();
  delay(2000);
  }
  
}

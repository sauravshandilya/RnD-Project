#include <driverlib.h>
#include <stdint.h>
#include <msp430.h>

//void configure_ADC_12(void)
//{
//    P6SEL |= 0x01;                            // Enable A/D channel A0 (digital pin P6.0 acting as A0)
//
//
//    REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to
//                                               // ADC12_A ref control registers
//    ADC12MCTL0 = ADC12INCH_1 + ADC12SREF_1;                 // ref+=AVcc, channel = A1
//     ADC12CTL0 = ADC12ON + ADC12SHT02 + ADC12REFON + ADC12REF2_5V + ADC12MSC;
//                                               // Turn on ADC12, Sampling time
//                                               // On Reference Generator and set to
//                                               // 2.5V
//     ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 + ADC12SSEL_1;                     // Use sampling timer
//     ADC12CTL2 = ADC12RES_0;
//     //ADC12MCTL0 = ADC12SREF_1 + ADC12RES_0;                 // Vr+=Vref+ and Vr-=AVss
//
//}

void configure_ADC_12(void)
{
    volatile unsigned int i;
  WDTCTL = WDTPW+WDTHOLD;                   // Stop watchdog timer
  P6SEL |= 0x01;                            // Enable A/D channel A0
  REFCTL0 &= ~REFMSTR;                      // Reset REFMSTR to hand over control to 
                                            // ADC12_A ref control registers
  ADC12CTL0 = ADC12ON+ADC12SHT02+ADC12REFON+ADC12REF2_5V;
                                            // Turn on ADC12, Sampling time
                                            // On Reference Generator and set to
                                            // 2.5V
  ADC12CTL1 = ADC12SHP;                     // Use sampling timer
  ADC12MCTL0 = ADC12SREF_1;                 // Vr+=Vref+ and Vr-=AVss

  for ( i=0; i<0x30; i++);                  // Delay for reference start-up

  ADC12CTL0 |= ADC12ENC;                    // Enable conversions

}

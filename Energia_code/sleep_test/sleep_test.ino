#include <driverlib.h>
#include <stdint.h>
#include <DHT22_430.h>      // DHT22
#include <msp430.h>
//#include "my_uart0.c"
#include "my_delay.c"

const char node_id = 2;

//// DHT related variables
#define DHTPIN P2_0          // DHT pin connected to P2.0
DHT22 mySensor(DHTPIN);
boolean detect_dht;

boolean flag_wake_up_signal;
boolean flag_node_id_match;

volatile char incomingByte = 0;   // for incoming serial data

unsigned int datapacket[5]={0};

void low_power_setup(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P5,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);    
    GPIO_setAsOutputPin(GPIO_PORT_PJ,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3);
    
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2);    
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3);
    
    UCSCTL6 = XT2OFF | XT1OFF | SMCLKOFF;
    //UCSCTL4 = SELA_1;                       // Ensure VLO is ACLK source
    //UCSCTL4 = SELA_2;                       // Ensure VLO is REF0 source
    
    // Disable VUSB LDO and SLDO
    USBKEYPID   =     0x9628;           // set USB KEYandPID to 0x9628
                                      // access to USB config registers enabled
    USBPWRCTL &= ~(SLDOEN+VUSBEN);      // Disable the VUSB LDO and the SLDO
    USBKEYPID   =    0x9600;            // access to USB config registers disabled
  
    // Disable SVS - Power Management Module (PMM) 
    PMMCTL0_H = PMMPW_H;                // PMM Password
    SVSMHCTL &= ~(SVMHE+SVSHE);         // Disable High side SVS
    SVSMLCTL &= ~(SVMLE+SVSLE);         // Disable Low side SVS

}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    while (!(UCA0IFG & UCTXIFG));             // USCI_A0 TX buffer ready?
//    UCA1TXBUF = UCA1RXBUF;                  // TX -> RXed character
      incomingByte = UCA0RXBUF;
      //UCA0TXBUF = (UCA0RXBUF + 1);                  // TX -> RXed character
}


/*
 * ISR for Timer0_A0
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
	TA0CTL |= MC_0;			//Use UP mode timer
	__bic_SR_register_on_exit(LPM3_bits|GIE); //When we exit the interrupt routine we return to Low Power Mode

}

void setup()
{
  // put your setup code here, to run once:
  //uart0_init();
  disableWatchDog();
  
  pinMode(P1_5,OUTPUT);    // DHT MOSFET
  pinMode(P1_0,OUTPUT);    //LED
  
  low_power_setup();
  
  //uart0_string("Setup Done \n\r");
}

void periodic_led_on_off()
{
    digitalWrite(P1_0, HIGH);
    delay_sec(5);
    digitalWrite(P1_0, LOW);    
    delay_sec(5);

}
void loop()
{
  // put your main code here, to run repeatedly:
   unsigned int data;
   uint8_t data1,data2 = 0;
   unsigned int i = 0;
   
   int temperature, humidity;
   periodic_led_on_off(); 
 
  //suspend();
  __bis_SR_register(LPM3_bits + GIE); //Enter Low Power Mode 3 with interrupts
}


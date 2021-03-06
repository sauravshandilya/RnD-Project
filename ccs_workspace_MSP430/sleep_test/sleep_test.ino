#include <driverlib.h>
#include <stdint.h>
#include <DHT22_430.h>      // DHT22
#include <msp430.h>
#include "my_uart0.c"

const char node_id = 2;

//// DHT related variables
#define DHTPIN P2_0          // DHT pin connected to P2.0
DHT22 mySensor(DHTPIN);
boolean detect_dht;

boolean flag_wake_up_signal;
boolean flag_node_id_match;

uint32_t mclk = 0;
uint32_t smclk = 0;
uint32_t aclk = 0;

volatile char incomingByte = 0;   // for incoming serial data

unsigned int datapacket[5]={0};

void low_power_setup(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6);    // 0-6 pins
    GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P3,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setAsOutputPin(GPIO_PORT_P6,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6);    // 0-6 pins
    GPIO_setAsOutputPin(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN4);    // 0,4 pins
    GPIO_setAsOutputPin(GPIO_PORT_P8,GPIO_PIN1|GPIO_PIN2);    // 1,2 pins
    GPIO_setAsOutputPin(GPIO_PORT_PJ,GPIO_PIN0);    // 0-7 pins;
    
    GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6);    // 0-6 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P2,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P4,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6);    // 0-7 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P7,GPIO_PIN0|GPIO_PIN4);    // 0,4 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_P8,GPIO_PIN1|GPIO_PIN2);    // 1,2 pins
    GPIO_setOutputLowOnPin(GPIO_PORT_PJ,GPIO_PIN0);    // 1,2 pins

    // Disable VUSB LDO and SLDO
    USBKEYPID   =     0x9628;           // set USB KEYandPID to 0x9628
                                        // access to USB config registers enabled
    USBPWRCTL &= ~(SLDOEN+VUSBEN);      // Disable the VUSB LDO and the SLDO
    USBKEYPID   =    0x9600;            // access to USB config registers disabled

    // Disable SVS
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

void setup()
{
  // put your setup code here, to run once:
  //uart0_init();
  //pinMode(P1_5,OUTPUT);    // DHT MOSFET
  pinMode(P1_0,OUTPUT);    //LED
  
  low_power_setup();
  
  uart0_string("Setup Done \n\r");
}

void loop()
{
  // put your main code here, to run repeatedly:
   unsigned int data;
   uint8_t data1,data2 = 0;
   unsigned int i = 0;
   


   int temperature, humidity;

//   aclk=UCS_getACLK();
//   mclk=UCS_getMCLK();
//   smclk=UCS_getSMCLK();
//   while (1);



  digitalWrite(P1_0, HIGH);
  	  sleep(1000);
  digitalWrite(P1_0, LOW);
  sleepSeconds(3);
//  sleep(3000);

   __bis_SR_register(LPM3_bits + GIE); //Enter Low Power Mode 3 with interrupts

}


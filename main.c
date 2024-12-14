//#include "tm4c123gh6pm.h"
//#include "/DIO/DIO.h"
//#include <stdint.h>
//#include <stdlib.h>
//void Delay(unsigned long counter); // used to add delay
//void HC05_init(void); // Initialize UART5 module for HC-05
//char Bluetooth_Read(void); //Read data from Rx5 pin of TM4C123
//void Bluetooth_Write(unsigned char data); // Transmit a character to HC-05 over Tx5 pin
//void Bluetooth_Write_String(char *str); // Transmit a string to HC-05 over Tx5 pin
//void toggleLED(char c);
//
//volatile char received_char = 0;
//
//int main(void) {
//	HC05_init(); // call HC05_init() to initialze UART5 of TM4C123GH6PM
//
//	/* Set PF1, PF2 and PF3 as digital output pins */
//	SYSCTL_RCGCGPIO_R |= 0x20;   /* enable clock to GPIOF */
//    GPIO_PORTF_DIR_R |= 0x0E;         //set PF1, PF2 and PF3 as digital output pin
//    GPIO_PORTF_DEN_R |= 0x0E;         // CON PF1, PF2 and PF3 as digital GPIO pins
//
//	while(1) {
////		char c = Bluetooth_Read();          /* get a character from UART5 */
//    }
//}
//
//
//void toggleLED(char c){
////    static const uint8_t gpio_actions[256] = {
////            ['A'] = (1 << 1), // RED LED ON
////            ['B'] = (1 << 1), // RED LED OFF
////            ['C'] = (1 << 2), // BLUE LED ON
////            ['D'] = (1 << 2), // BLUE LED OFF
////            ['E'] = (1 << 3), // GREEN LED ON
////            ['F'] = (1 << 3)  // GREEN LED OFF
////    };
////
////    if (c == 'A' || c == 'C' || c == 'E') {
////        GPIO_PORTF_DATA_R |= gpio_actions[(unsigned char)c];
////    } else if (c == 'B' || c == 'D' || c == 'F') {
////        GPIO_PORTF_DATA_R &= ~gpio_actions[(unsigned char)c];
////    }
//    if (c == 'A') {
//        GPIO_PORTE_DATA_R &= ~(1 << 1);
//        Bluetooth_Write_String("PE1 LOW\n");
//    } else if (c == 'B') {
//        GPIO_PORTE_DATA_R |= (1 << 1);
//        Bluetooth_Write_String("PE1 HIGH\n");
//    } else if (c == 'C') {
//        GPIO_PORTF_DATA_R |= (1 << 2);
//        Bluetooth_Write_String("BLUE LED ON\n");
//    } else if (c == 'D') {
//        GPIO_PORTF_DATA_R &= ~(1 << 2);
//        Bluetooth_Write_String("BLUE LED OFF\n");
//    } else if (c == 'E') {
//        GPIO_PORTF_DATA_R |= (1 << 3);
//        Bluetooth_Write_String("GREEN LED ON\n");
//    } else if (c == 'F') {
//        GPIO_PORTF_DATA_R &= ~(1 << 3);
//        Bluetooth_Write_String("GREEN LED OFF\n");
//    }
//}
//char Bluetooth_Read(void)
//{
//    char data;
//    while((UART5_FR_R & (1<<4)) != 0); /* wait until Rx buffer is not full */
//    data = UART5_DR_R ;  	/* before giving it another byte */
//    return (unsigned char) data;
//}
//
//void Bluetooth_Write(unsigned char data)
//{
//    while((UART5_FR_R & (1<<5)) != 0); /* wait until Tx buffer not full */
//    UART5_DR_R = data;                  /* before giving it another byte */
//}
//
//void Bluetooth_Write_String(char *str)
//{
//    while(*str)
//    {
//        Bluetooth_Write(*(str++));
//    }
//}
//
//void Delay(unsigned long counter)
//{
//	unsigned long i = 0;
//
//	for(i=0; i< counter; i++);
//}
//
//
//void UART1_Init(void) {
//
//}
//
//
//void UART3_Init(void) {
//
//}
//
//// Initialize UART5 module for HC-05
//void HC05_init(void)
//{
//    // Initialize port E pins 4 and 5 for UART5
//    dio_init('E', 0x30, Input);
//
//    SYSCTL_RCGCUART_R |= 0x20;  /* enable clock to UART5 */
//    SYSCTL_RCGCGPIO_R |= 0x10;  /* enable clock to PORTE for PE4/Rx and RE5/Tx */
//    while ((SYSCTL_PRUART_R & (1 << 5)) == 0);
//    while ((SYSCTL_PRGPIO_R & (1 << 4)) == 0);
//    /* UART5 initialization */
//    UART5_CTL_R = 0;         /* UART5 module disable */
//    UART5_IBRD_R = 104;      /* for 9600 baud rate, integer = 104 (Start Bit / Stop Bit, Parity Bit & Data) */
//    UART5_FBRD_R = 11;       /* for 9600 baud rate, fractional = 11*/
//    UART5_CC_R = 0;          /*select system clock*/
//    UART5_LCRH_R = 0x60;     /* data lenght 8-bit, not parity bit, no FIFO */
//    UART5_CTL_R = 0x301;     /* Enable UART5 module, Rx and Tx */
//
//    UART5_IM_R |= (1 << 4);   // Enable RX interrupt
//    NVIC_EN1_R |= (1 << 29);   // Enable IRQ29 for UART5 in NVIC
//
//    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
//    GPIO_PORTE_DEN_R = 0x32;      /* set PE4 and PE5 as digital */
//    GPIO_PORTE_DIR_R |= (1 << 1); /* Set PE1 as output */
//    GPIO_PORTE_AFSEL_R = 0x30;    /* Use PE4,PE5 alternate function */
//    GPIO_PORTE_AMSEL_R = 0;    /* Turn off analg function*/
//    GPIO_PORTE_PCTL_R = 0x00110000;     /* configure PE4 and PE5 for UART */
//}
//
//
//// Interrupt Service Routine for UART5
//void UART5_Handler(void) {
//    while (!(UART5_FR_R & (1 << 4))) {  // While RX buffer is not empty
//        received_char = (char)(UART5_DR_R & 0xFF); // Read the character
//        toggleLED(received_char);
//    }
//    UART5_ICR_R = (1 << 4); // Clear RX interrupt flag
//}

#include "Modules/UART/UART.h"
#include "Modules/Relay/Relay.h"
#include "Modules/Alarm/Alarm.h"
#include "Modules/DoorStatus/DoorStatus.h"
#include "Modules/Temperature/LM35.h"
#include "Modules/DataLogging/DataLogging.h"

void relayControl(char c);
volatile char received_char = 0;

int main(void) {
    // Initialize modules
    Relay_Init('E', Pin1 | Pin2); // Lamp and Plug relays
    UART_Init(5, 9600); // UART5 for relay control
    DoorStatus_init();
    LM35_init();
    Alarm_init('E', Pin3);        // Buzzer
    DoorLogging_init();

    while (1) {
//        // Check door status and log events
//        bool doorOpen = getDoorStatus();
//        setDataLog(doorOpen);
//
//        // Check temperature
//        uint32_t temperature = GetTemp();
//        if (temperature >= 45) {
//            Toggle_alarm('E', Pin3); // Trigger buzzer
//        }
    }
}

// Interrupt Service Routine for UART5
void UART5_Handler(void) {
    while (!(UART5_FR_R & (1 << 4))) {  // While RX buffer is not empty
        received_char = (char)(UART5_DR_R & 0xFF); // Read the character
        relayControl(received_char);
    }
    UART5_ICR_R = (1 << 4); // Clear RX interrupt flag
}
void relayControl(char c){
    if (c == 'L') {
        Relay_Control('E', Pin1, RELAY_ON);
    } else if (c == 'l') {
        Relay_Control('E', Pin1, RELAY_OFF);
    } else if (c == 'P') {
        Relay_Control('E', Pin2, RELAY_ON);
    } else if (c == 'p') {
        Relay_Control('E', Pin2, RELAY_OFF);
    }
}
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdlib.h>
void Delay(unsigned long counter); // used to add delay
void HC05_init(void); // Initialize UART5 module for HC-05
char Bluetooth_Read(void); //Read data from Rx5 pin of TM4C123
void Bluetooth_Write(unsigned char data); // Transmit a character to HC-05 over Tx5 pin 
void Bluetooth_Write_String(char *str); // Transmit a string to HC-05 over Tx5 pin

int main(void) {
	HC05_init(); // call HC05_init() to initialze UART5 of TM4C123GH6PM
	
	/* Set PF1, PF2 and PF3 as digital output pins */
	
	SYSCTL_RCGCGPIO_R |= 0x20;   /* enable clock to GPIOF */
    GPIO_PORTF_DIR_R |= 0x0E;         //set PF1, PF2 and PF3 as digital output pin
    GPIO_PORTF_DEN_R |= 0x0E;         // CON PF1, PF2 and PF3 as digital GPIO pins
  	Delay(10); 
	
	while(1) {
		char c = Bluetooth_Read();          /* get a character from UART5 */
		
        /* Check the received character and take action to control onboard LEDs of TM4C123 */
        /* Send status string to Andriod app after turning on/off LEDs */

        if( c=='A'){
			GPIO_PORTF_DATA_R |=(1<<1);
			Bluetooth_Write_String("RED LED ON\n");
		}
		else if( c=='B'){
			GPIO_PORTF_DATA_R &=~(1<<1);
			Bluetooth_Write_String("RED LED OFF\n");
		}
		else if( c=='C'){
			GPIO_PORTF_DATA_R |=(1<<2);
			Bluetooth_Write_String("BLUE LED ON\n");
		}
		else if( c=='D'){
			GPIO_PORTF_DATA_R &=~(1<<2);
			Bluetooth_Write_String("BLUE LED OFF\n");
		}
		if( c=='E'){
			GPIO_PORTF_DATA_R |=(1<<3);
			Bluetooth_Write_String("GREEN LED ON\n");
		}
		else if( c=='F'){
			GPIO_PORTF_DATA_R &=~(1<<3);
			Bluetooth_Write_String("GREEN LED OFF\n");
		}	
	}
}

void HC05_init(void)
{
	SYSCTL_RCGCUART_R |= 0x20;  /* enable clock to UART5 */
    SYSCTL_RCGCGPIO_R |= 0x10;  /* enable clock to PORTE for PE4/Rx and RE5/Tx */
    Delay(1);
    /* UART0 initialization */
    UART5_CTL_R = 0;         /* UART5 module disable */
    UART5_IBRD_R = 104;      /* for 9600 baud rate, integer = 104 */
    UART5_FBRD_R = 11;       /* for 9600 baud rate, fractional = 11*/
    UART5_CC_R = 0;          /*select system clock*/
    UART5_LCRH_R = 0x60;     /* data lenght 8-bit, not parity bit, no FIFO */
    UART5_CTL_R = 0x301;     /* Enable UART5 module, Rx and Tx */

    /* UART5 TX5 and RX5 use PE4 and PE5. Configure them digital and enable alternate function */
    GPIO_PORTE_DEN_R = 0x30;      /* set PE4 and PE5 as digital */
    GPIO_PORTE_AFSEL_R = 0x30;    /* Use PE4,PE5 alternate function */
    GPIO_PORTE_AMSEL_R = 0;    /* Turn off analg function*/
    GPIO_PORTE_PCTL_R = 0x00110000;     /* configure PE4 and PE5 for UART */
}
char Bluetooth_Read(void)  
{
    char data;
	  while((UART5_FR_R & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART5_DR_R ;  	/* before giving it another byte */
    return (unsigned char) data; 
}

void Bluetooth_Write(unsigned char data)  
{
    while((UART5_FR_R & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART5_DR_R = data;                  /* before giving it another byte */
}

void Bluetooth_Write_String(char *str)
{
  while(*str)
	{
		Bluetooth_Write(*(str++));
	}
}

void Delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter; i++);
}
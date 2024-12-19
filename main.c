#include "Modules/UART/UART.h"
#include "Modules/Relay/Relay.h"
#include "Modules/Alarm/Alarm.h"
#include "Modules/DoorStatus/DoorStatus.h"
#include "Modules/Temperature/LM35.h"
#include "Modules/Systick/Systick.h"

volatile char received_char = 0;
volatile uint8_t received_data = 0;
volatile uint8_t testTemp = 30;
volatile bool doorStatus = false;  // Tracks door status: true = open, false = closed
void relayControl(char c);
void PortB_Init(void);
void PortD_Init(void);
void tempEN(char c);

int main(void) {
    // Initialize modules
    Relay_Init('E', Pin1 | Pin2); // Lamp and Plug relays
    PortB_Init(); // Door status
    PortD_Init(); // Temperature sensor
    UART_Init(5, 9600); // UART5 for relay control
//    DoorStatus_init();
    LM35_init();
    Alarm_init('E', Pin3);        // Buzzer

    // Main Body
    while (1) {
        // Enter sleep mode until interrupt happen (To save power)
        __asm("wfi");
    }
}


// Interrupt Service Routine for UART5
void UART5_Handler(void) {
    while (!(UART5_FR_R & (1 << 4))) {  // While RX buffer is not empty
        received_char = (char)(UART5_DR_R & 0xFF); // Read the character
        relayControl(received_char);
        tempEN(received_char);
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

void tempEN(char c){
    if (received_char == 'T') {
        GPIO_PORTD_DATA_R |= (1 << 0);
    } else if (received_char == 't') {
        GPIO_PORTD_DATA_R &= ~(1 << 0);
    }
}

void PortB_Init(void) {
    // 1. Enable the clock for Port B
    SYSCTL_RCGCGPIO_R |= (1 << 1);  // Enable clock for Port B
    while ((SYSCTL_PRGPIO_R & (1 << 1)) == 0);  // Wait for the clock to stabilize

    // 2. Configure PB0 as a digital input
    GPIO_PORTB_DIR_R &= ~(1 << 0);  // PB0 as input
    GPIO_PORTB_DEN_R |= (1 << 0);   // Enable digital functionality for PB0
    GPIO_PORTB_PDR_R |= (1 << 0);  // Enable pull-down resistor for PB0
    // 3. Configure PB0 for edge-triggered interrupts
    GPIO_PORTB_IS_R &= ~(1 << 0);   // PB0 is edge-sensitive
    GPIO_PORTB_IBE_R |= (1 << 0);   // Interrupt on both rising and falling edges

    // 4. Clear any prior interrupts and enable interrupt for PB0
    GPIO_PORTB_ICR_R |= (1 << 0);   // Clear any prior interrupt on PB0
    GPIO_PORTB_IM_R |= (1 << 0);    // Enable interrupt for PB0

    // 5. Configure NVIC to handle Port B interrupts
    NVIC_EN0_R |= (1 << 1);         // Enable IRQ1 (Port B)
}

// GPIO Port B Interrupt Handler
void GPIOB_Handler(void) {
    if (GPIO_PORTB_RIS_R & (1 << 0)) {  // Check if PB0 caused the interrupt
        // Check the current state of PB0
        if (((GPIO_PORTB_DATA_R & (1 << 0)) == 0) && !doorStatus) {  // Door open (PB0 LOW)
            doorStatus = true;
            UART_Transmit(5,'O');  // Send 'O' for Door Open
        }
        else if (((GPIO_PORTB_DATA_R & (1 << 0)) == 1) && doorStatus) {  // Door closed (PB0 HIGH)
            doorStatus = false;
            UART_Transmit(5,'C');  // Send 'C' for Door Closed
        }
    }
    SysTick_Init(3 * (16000 - 1));
    SysTick_Wait();  // Wait for 3 ms for debounce
    GPIO_PORTB_ICR_R |= (1 << 0);  // Clear interrupt flag for PB0
}

void PortD_Init(void) {
    // 1. Enable the clock for Port D
    SYSCTL_RCGCGPIO_R |= (1 << 3);  // Enable clock for Port D
    while ((SYSCTL_PRGPIO_R & (1 << 3)) == 0);  // Wait for the clock to stabilize

    // 2. Configure PD0 as a digital input
    GPIO_PORTD_DIR_R |= (1 << 0);  // PD0 as output
    GPIO_PORTD_DEN_R |= (1 << 0);   // Enable digital functionality for PD0
    GPIO_PORTD_PDR_R |= (1 << 0);  // Enable pull-down resistor for PD0

    // 3. Configure PD0 for edge-triggered interrupts
    GPIO_PORTD_IS_R |= (1 << 0);   // PD0 is edge-sensitive
    GPIO_PORTD_IEV_R |= (1 << 0);   // Interrupt on rising level

    // 4. Clear any prior interrupts and enable interrupt for PD0
    GPIO_PORTD_ICR_R |= (1 << 0);   // Clear any prior interrupt on PD0
    GPIO_PORTD_IM_R |= (1 << 0);    // Enable interrupt for PD0

    // 5. Configure NVIC to handle Port D interrupts
    NVIC_EN0_R |= (1 << 3);         // Enable IRQ1 (Port D)
    NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF0FFF) | (2 << 13);   // Clear bits [15:13], set priority to 2
}

// GPIO Port D Interrupt Handler
void GPIOD_Handler(void) {
    if (GPIO_PORTD_RIS_R & (1 << 0)) {  // Check if PD0 caused the interrupt
        if ((GPIO_PORTD_DATA_R & (1 << 0)) != 0) {  // Temperature Send (PD0 HIGH)
            SysTick_Init(300 * (16000 - 1)); // Initialize SysTick timer
            SysTick_Wait(); // Wait for 100 ms for debounce
            UART_Transmit_Temperature(5, testTemp);// Capture Temprature for Testing
            testTemp += 1; // Capture Temprature for Testing
            SysTick_Init(800 * (16000 - 1)); // Initialize SysTick timer
            SysTick_Wait(); // Wait for 100 ms for debounce
        }
    }
    GPIO_PORTD_ICR_R |= (1 << 0);  // Clear interrupt flag for PD0
}

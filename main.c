#include "Modules/UART/UART.h"
#include "Modules/Relay/Relay.h"
#include "Modules/Alarm/Alarm.h"
#include "Modules/DoorStatus/DoorStatus.h"
#include "Modules/Temperature/LM35.h"
#include "Modules/Systick/Systick.h"

void relayControl(char c);
volatile char received_char = 0;
volatile uint8_t received_data = 0;
volatile bool doorStatus = false;  // Tracks door status: true = open, false = closed
void PortB_Init(void);

void SysTick_Wait(void);

int main(void) {
    // Initialize modules
    Relay_Init('E', Pin1 | Pin2); // Lamp and Plug relays
    PortB_Init(); // Door status
    UART_Init(5, 9600); // UART5 for relay control
//    DoorStatus_init();
    LM35_init();
    Alarm_init('E', Pin3);        // Buzzer

    SYSCTL_RCGCGPIO_R |= (1 << 5);    // Enable clock for Port F
    GPIO_PORTF_DIR_R |= (1 << 1);     // Set PF1 as output
    GPIO_PORTF_DEN_R |= (1 << 1);     // Enable digital functionality on PF1
    SysTick_Init(3*(16000 - 1)); // Initialize SysTick timer
    while (1) {
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
    SysTick_Wait();  // Wait for 3 ms for debounce
    GPIO_PORTB_ICR_R |= (1 << 0);  // Clear interrupt flag for PB0
}

void SysTick_Wait(void) {
    while (!SysTick_Is_Time_Out()); // Wait for COUNT flag
    NVIC_ST_CURRENT_R = 0;                  // Clear the flag by writing to CURRENT
}

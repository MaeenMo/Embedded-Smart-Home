#include "LM35.h"

void LM35_init(void) {
    SYSCTL_RCGCGPIO_R |= (1 << 4);  // Enable clock for Port E
    while ((SYSCTL_PRGPIO_R & (1 << 4)) == 0);

    GPIO_PORTE_DIR_R &= ~Pin3;  // Configure PE3 as input
    GPIO_PORTE_AFSEL_R |= Pin3; // Enable alternate function for PE3
    GPIO_PORTE_DEN_R &= ~Pin3;  // Disable digital functionality for PE3
    GPIO_PORTE_AMSEL_R |= Pin3; // Enable analog functionality for PE3

    SYSCTL_RCGCADC_R |= 1;          // Enable ADC0
    while ((SYSCTL_PRADC_R & 1) == 0);

    ADC0_ACTSS_R &= ~8;             // Disable SS3 during configuration
    ADC0_EMUX_R |= 0xF000;         // Configure SS3 for software trigger
    ADC0_SSMUX3_R |= 0;             // Set channel (AIN0 -> PE3)
    ADC0_SSCTL3_R |= 0x02;          // Single-ended
    ADC0_SAC_R |= 0x06;             // 64x hardware oversampling (0 1x, 1 2x, 2 4x, 3 8x, 4 16x, 5 32x, 6 64x)
    ADC0_PC_R |= 0x05;               // 125k samples/sec (1 125K, 3 250K, 5 500K, 7 1M)
    ADC0_ACTSS_R |= 8;              // Re-enable SS3
}

double GetTemp(void) {
    uint32_t reading = 0.0;
    ADC0_PSSI_R |= 8;                // Start SS3
    reading = ADC0_SSFIFO3_R & 0xFFF;
    return (double)(reading * 0.08335966222); // (3414 Volts / 4096) * 0.1 => calibration value for LM35

}

void tempEn_init(char port, uint8_t pins) {
    // 1. Enable the clock for Port D
    dio_init(port, pins, Output);
    GPIO_PORTD_PUR_R &= ~pins;
    GPIO_PORTD_PDR_R |= pins;  // Enable pull-down resistor for PD0

    // 3. Configure PD0 for rising level interrupts
    GPIO_PORTD_IS_R |= pins;   // PD0 is level-sensitive
    GPIO_PORTD_IEV_R |= pins;   // Interrupt on rising level

    // 4. Clear any prior interrupts and enable interrupt for PD0
    GPIO_PORTD_ICR_R |= pins;   // Clear any prior interrupt on PD0
    GPIO_PORTD_IM_R |= pins;    // Enable interrupt for PD0

    // 5. Configure NVIC to handle Port D interrupts
    NVIC_EN0_R |= (1 << 3);         // Enable IRQ1 (Port D)
    NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF0FFF) | (2 << 13);   // Clear bits [15:13], set priority to 2
}

void Read_Temperature_Handler(void) {
    if (GPIO_PORTD_RIS_R & (1 << 0)) {  // Check if PD0 caused the interrupt
        if ((GPIO_PORTD_DATA_R & (1 << 0)) != 0) {  // Temperature Send (PD0 HIGH)
            SysTick_Init(300 * (16000 - 1)); // Initialize SysTick timer
            SysTick_Wait(); // Wait for 100 ms for debounce
            double temperature = GetTemp();  // Get temperature from LM35
            UART_Transmit_Temperature(5, temperature); // Transmit temperature
            if(temperature >= 36) {
                Toggle_alarm('B', 1);
            }
            SysTick_Init(800 * (16000 - 1)); // Initialize SysTick timer
            SysTick_Wait(); // Wait for 100 ms for debounce
        }
    }
    GPIO_PORTD_ICR_R |= (1 << 0);  // Clear interrupt flag for PD0
}

void tempEN(char c){
    if (c == 'T') {
        GPIO_PORTD_DATA_R |= (1 << 0);
    } else if (c == 't') {
        GPIO_PORTD_DATA_R &= ~(1 << 0);
    }
}
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

#include "Modules/DIO/DIO.h"
#include "Modules/UART/UART.h"
#include "Modules/Relay/Relay.h"
#include "Modules/Alarm/Alarm.h"
#include "Modules/DoorStatus/DoorStatus.h"
#include "Modules/Temperature/LM35.h"
#include "Modules/Systick/Systick.h"

int main(void) {
    // Initialize modules
    UART_Init(5, 9600);               // UART5 for relay control
    Relay_Init('E', Pin1, Pin2);      // Lamp and Plug relays
    DoorStatus_init('B', Pin0);       // Door status
    LM35_init();                      // Initialize LM35 sensor
    tempEn_init('D',Pin0);            // Temperature sensor
    Alarm_init('B', Pin1);            // Buzzer

    // Main Body
    while (1) {
        // Enter sleep mode until interrupt happen (To save power)
        __asm("wfi");
    }
}


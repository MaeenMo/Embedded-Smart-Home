#ifndef SYSTICKTIMER_H
#define SYSTICKTIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "../../tm4c123gh6pm.h"

// Function prototypes for polling mode
void SysTick_Init(uint32_t reloadValue);
void SysTick_Enable(void);
void SysTick_Disable(void);
void SysTick_PeriodSet(uint32_t ui32Period);
uint32_t SysTick_PeriodGet(void);
uint32_t SysTick_ValueGet(void);
bool SysTick_Is_Time_Out(void);
void SysTick_Wait(void);

// Function prototypes for interrupt mode
void SysTick_InitInterrupt(uint32_t reloadValue, void (*callback)(void));

#endif

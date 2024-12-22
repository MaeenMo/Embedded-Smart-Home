#ifndef LM35_H
#define LM35_H

#include <stdint.h>
#include "../DIO/DIO.h"

void LM35_init(void);
uint32_t GetTemp(void);
void tempEn_init(char port, uint8_t pins);
void Read_Temperature_Handler(void);
void tempEN(char c);

#endif // LM35_H

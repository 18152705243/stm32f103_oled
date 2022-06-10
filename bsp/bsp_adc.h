#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "bsp.h"


void ADC1_Init(void);
void ADC_SetTimerFre(uint16_t psc, uint16_t arr);
void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 
#endif 

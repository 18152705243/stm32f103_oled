#ifndef BSP_TIMER_H
#define BSP_TIMER_H

#include "bsp.h"

void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM2_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);

#endif

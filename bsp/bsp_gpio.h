#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "bsp.h"

typedef enum{
	LED_ON = 0,
	LED_OFF = !LED_ON
}LED_FLAG;

#define LED 		PCout(13)
#define LED_TOGGLE()	GPIOC->ODR ^= GPIO_Pin_13

#define KEY_UP()	(!PAin(4))
#define KEY_DOWN()	(!PAin(5))
#define KEY_LEFT()	(!PAin(6))
#define KEY_RIGHT()	(!PAin(7))
#define KEY_MID()	(!PBin(0))

#define KEY_UP_PRESS	1
#define KEY_DOWN_PRESS	2
#define KEY_LEFT_PRESS	3
#define KEY_RIGHT_PRESS	4
#define KEY_MID_PRESS	4


void LED_Init(void);
void Key_Init(void);
uint8_t Key_Scan(uint8_t mode);

#endif

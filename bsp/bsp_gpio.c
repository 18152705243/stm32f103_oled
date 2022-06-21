#include "bsp_gpio.h"

void LED_Init()
{
    GPIO_InitTypeDef GPIO_Initstructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_Initstructure);
    LED = LED_OFF;
}

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_Initstructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_Initstructure);

    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_Initstructure);
}

uint8_t Key_Scan(uint8_t mode)
{
    static uint8_t key_up = 1; //按键按松开标志
    if (mode)
        key_up = 1; //支持连按
    if (key_up && (KEY_UP() || KEY_DOWN() || KEY_LEFT() || KEY_RIGHT() || KEY_MID() || KEY_SET() || KEY_RESET()))
    {
        delay_ms(10); //去抖动
        key_up = 0;
        if (KEY_UP())
            return KEY_UP_PRESS;
        else if (KEY_DOWN())
            return KEY_DOWN_PRESS;
        else if (KEY_LEFT())
            return KEY_LEFT_PRESS;
        else if (KEY_RIGHT())
            return KEY_RIGHT_PRESS;
        else if (KEY_MID())
            return KEY_MID_PRESS;
        else if (KEY_SET())
            return KEY_SET_PRESS;
        else if (KEY_RESET())
            return KEY_RESET_PRESS;
    }
    else if ((!KEY_UP()) && (!KEY_DOWN()) && (!KEY_LEFT()) && (!KEY_RIGHT()) && (!KEY_MID()) && (!KEY_SET()) && (!KEY_RESET()))
        key_up = 1;
    return 0; // 无按键按下
}

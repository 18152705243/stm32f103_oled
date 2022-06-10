#include "bsp_gpio.h"


void LED_Init()
{
	GPIO_InitTypeDef GPIO_Initstructure = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstructure);
	LED = LED_OFF;
}

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_Initstructure = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
}

uint8_t Key_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY_UP()||KEY_DOWN()||KEY_LEFT()||KEY_RIGHT()))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY_UP())return KEY_UP_PRESS;
		else if(KEY_DOWN())return KEY_DOWN_PRESS;
		else if(KEY_LEFT())return KEY_LEFT_PRESS;
		else if(KEY_RIGHT())return KEY_RIGHT_PRESS; 
	}else if((!KEY_UP())&&(!KEY_DOWN())&&(!KEY_LEFT())&&(!KEY_RIGHT()))key_up=1; 	     
	return 0;// �ް�������
}
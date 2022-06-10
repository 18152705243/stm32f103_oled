#include "bsp.h"
#include "bsp_timer.h"
#include "bsp_gpio.h"
#include "oled.h"
#include "scope.h"
#include "bsp_adc.h"


uint16_t adc_buf[1024] = {0};

extern scope_t hscope;

int main()
{
	uint8_t key_value;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// 配置中断向量
	delay_init();	// 初始化延时
	
	UART1_Init(115200);	// 初始化串口1，波特率为115200
	TIM2_Init(1000, 71);
	TIM1_PWM_Init(100, 71);
	TIM_SetCompare1(TIM1, 50);
    ADC1_Init();
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	Key_Init();
	Scope_Start();
	
	hscope.time_base = TB_10ms;
    Scope_SetTimeBase();
	
	
	OLED_Refresh();
	while (1)
	{
		key_value = Key_Scan(0);
		if (key_value == KEY_UP_PRESS)
		{
			hscope.time_base++;
			if (hscope.time_base >= TB_Max)
				hscope.time_base = TB_Max - 1;
			Scope_SetTimeBase();
		}
		else if (key_value == KEY_DOWN_PRESS)
		{
			hscope.time_base--;
			if (hscope.time_base <= TB_Min)
				hscope.time_base = TB_Min + 1;
			Scope_SetTimeBase();
		}
		Scope_WaveRefresh(adc_buf);
        Scope_ParaRefresh();
		delay_ms(200);
	}
}

#include "bsp.h"
#include "bsp_timer.h"
#include "bsp_gpio.h"
#include "oled.h"
#include "scope.h"
#include "bsp_adc.h"
#include <string.h>

uint16_t adc_buf[4000];

extern scope_t hscope;

int main()
{
	uint8_t key_value;
	uint8_t adc_cmd_sta = 1;
	uint16_t i;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// �����ж�����
	delay_init();	// ��ʼ����ʱ
	
	UART1_Init(115200);	// ��ʼ������1��������Ϊ115200
	TIM2_Init(999, 55);
	TIM1_PWM_Init(199, 5599);
	TIM_SetCompare1(TIM1, 50);
    ADC1_Init();
	OLED_Init();
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	Key_Init();
	Scope_Init();
	Scope_Start();

	
	hscope.trig_level = 1024;

	OLED_Refresh();

	while (1)
	{
		key_value = Key_Scan(0);
		if (key_value == KEY_UP_PRESS)
		{
			if (hscope.time_base++ >= TB_Max)
				hscope.time_base = TB_Max;
			Scope_SetTimeBase();
		}
		else if (key_value == KEY_DOWN_PRESS)
		{
			if (hscope.time_base-- <= TB_Min)
				hscope.time_base = TB_Min;
			Scope_SetTimeBase();
		}
		else if (key_value == KEY_MID_PRESS)
		{
			adc_cmd_sta = !adc_cmd_sta;
            hscope.sta.blocked = !hscope.sta.blocked;
		}
        else if (key_value == KEY_LEFT_PRESS)
        {
            Scope_Auto(adc_buf);
        }
		
		if (adc_cmd_sta)
		{
			while (!ADC_GetTransmitedSta())
			{
				delay_ms(1);
				if (i++ >= 3000)
				{
					i = 0;
					break;
				}
			}
		}
		
        Scope_Task(adc_buf);

		if (adc_cmd_sta)
			ADC_DMAStartTransmit();
		else
			ADC_DMAStopTransmit();
		ADC_ResetTransmitedSta();
	}
}

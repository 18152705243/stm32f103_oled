#include "scope.h"
#include "bsp.h"
#include "bsp_adc.h"

scope_t hscope = {0};


void Scope_Init(void)
{
    hscope.time_base = TB_1s;
    hscope.time_base_buf = "1s";
}

/**
 * @brief Set The Time base according to the parament of hscope.time_base
 * one screen is divided to 5 timebases, 
 * SCOPE_WAVE_POINT_NUM/5 points are asigned between two timebase line
 */
void Scope_SetTimeBase(void)
{
    switch (hscope.time_base)
    {
    case TB_50s:    // 2.5s per point
        ADC_SetTimerFre(7199, 24999);
        hscope.time_base_buf = "  50s";
        break;
    case TB_20s:    // 1s per point
        ADC_SetTimerFre(7199, 9999);
        hscope.time_base_buf = "  20s";
        break;
    case TB_10s:    // 0.5s per point
        ADC_SetTimerFre(7199, 4999);
        hscope.time_base_buf = "  10s";
        break;
    case TB_5s:
        ADC_SetTimerFre(7199, 2499);
        hscope.time_base_buf = "   5s";
        break;
    case TB_2s:
        ADC_SetTimerFre(7199, 999);
        hscope.time_base_buf = "   2s";
        break;
    case TB_1s:
        ADC_SetTimerFre(7199, 499);
        hscope.time_base_buf = "   1s";
        break;
    case TB_0s5:
        ADC_SetTimerFre(719, 2499);
        hscope.time_base_buf = "500ms";
        break;
    case TB_0s2:
        ADC_SetTimerFre(719, 999);
        hscope.time_base_buf = "200ms";
        break;
    case TB_0s1:
        ADC_SetTimerFre(719, 999);
        hscope.time_base_buf = "100ms";
        break;
    case TB_50ms:
        ADC_SetTimerFre(71, 2499);
        hscope.time_base_buf = " 50ms";
        break;
    case TB_20ms:
        ADC_SetTimerFre(71, 999);
        hscope.time_base_buf = " 20ms";
        break;
    case TB_10ms:
        ADC_SetTimerFre(71, 499);
        hscope.time_base_buf = " 10ms";
        break;
    case TB_5ms:
        ADC_SetTimerFre(71, 249);
        hscope.time_base_buf = "  5ms";
        break;
    case TB_2ms:
        ADC_SetTimerFre(71, 99);
        hscope.time_base_buf = "  2ms";
        break;
    case TB_1ms:
        ADC_SetTimerFre(71, 49);
        hscope.time_base_buf = "  1ms";
        break;
    case TB_0ms5:
        ADC_SetTimerFre(71, 24);
        hscope.time_base_buf = "500us";
        break;
    case TB_0ms2:
        ADC_SetTimerFre(71, 9);
        hscope.time_base_buf = "200us";
        break;
    case TB_0ms1:
        ADC_SetTimerFre(71, 4);
        hscope.time_base_buf = "100us";
        break;
    case TB_50us:
        ADC_SetTimerFre(35, 5);
        hscope.time_base_buf = " 50us";
        break;
    case TB_20us:
        ADC_SetTimerFre(71, 1);
        hscope.time_base_buf = " 20us";
        break;
    case TB_10us:
        ADC_SetTimerFre(71, 1);
        hscope.time_base_buf = " 10us";
        break;
    case TB_5us:
        ADC_SetTimerFre(71, 1);
        hscope.time_base_buf = "  5us";
        break;
    case TB_2us:
        ADC_SetTimerFre(71, 1);
        hscope.time_base_buf = "  2us";
        break;
    case TB_1us:
        ADC_SetTimerFre(71, 1);
        hscope.time_base_buf = "  1us";
        break;
    
    default:
        break;
    }
}


void Scope_DrawNet(void)
{
    scope_uint16_t i, j;
    static scope_uint16_t lines_x_per_gap =  (SCOPE_END_X - SCOPE_START_X) / 5;	// x：列每两格之间的像素
	static scope_uint16_t lines_y_per_gap =  (SCOPE_END_Y - SCOPE_START_Y) / 5;	// y：行每两格之间的像素

    for (i = 1; i <= 4; i++)
	{
        for (j = 1; j <= 4; j++)
        {
		    Scope_DrawPoint(SCOPE_START_X + j * lines_x_per_gap,
                            SCOPE_START_Y + i * lines_y_per_gap, SCOPE_COLOR_WHITE);
        }
	}

}

void Scope_Start()
{
	scope_uint16_t i;
	
    Scope_DrawRect(SCOPE_START_X, SCOPE_START_Y, SCOPE_END_X, SCOPE_END_Y, SCOPE_COLOR_WHITE);
	Scope_DrawNet();

    Scope_Fill(SCOPE_END_X + 2, SCOPE_START_Y, SCOPE_WIDTH - 1, SCOPE_END_Y, 1);
    Scope_Fill(SCOPE_END_X + 2, SCOPE_START_Y + 12*1, SCOPE_WIDTH - 1, SCOPE_START_Y + 12*2, 0);
    Scope_Fill(SCOPE_END_X + 2, SCOPE_START_Y + 12*3, SCOPE_WIDTH - 1, SCOPE_START_Y + 12*4, 0);
    Scope_ShowStrReverse(SCOPE_END_X + 5, SCOPE_START_Y + 2, "VPP", 8, SCOPE_COLOR_WHITE);
	Scope_ShowStrReverse(SCOPE_END_X + 5, SCOPE_START_Y + 2 + 12*2, "Fre", 8, SCOPE_COLOR_WHITE);
}       


void Scope_WaveRefresh(scope_uint16_t *adc_buf)
{
    scope_uint16_t i, y0, y1;
	
	Scope_Fill(SCOPE_START_X + 1, SCOPE_START_Y + 1,
				SCOPE_START_X + SCOPE_WAVE_POINT_NUM - 1, SCOPE_END_Y - 1,
				SCOPE_COLOR_BLACK);

    for (i = 0; i < SCOPE_WAVE_POINT_NUM; i++)
    {
        y0 = (adc_buf[i]/4096.0)*40;
		y1 = (adc_buf[i+1]/4096.0)*40;
		
		// Scope_DrawLine(SCOPE_START_X + i, SCOPE_END_Y - y0 - 10, 
		// 				SCOPE_START_X + i + 1, SCOPE_END_Y - y1 - 10, 
		// 				SCOPE_COLOR_WHITE);
       Scope_DrawPoint(SCOPE_START_X + i, SCOPE_END_Y - 10 - y0, SCOPE_COLOR_WHITE);
    }
    Scope_DrawNet();

	Scope_Refresh();
}

void Scope_ParaRefresh(void)
{
	Scope_ShowStrReverse(SCOPE_START_X, SCOPE_END_Y + 1, hscope.time_base_buf, 8, SCOPE_COLOR_WHITE);
}




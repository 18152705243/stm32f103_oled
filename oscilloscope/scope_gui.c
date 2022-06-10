#include "scope_gui.h"
#include "oled.h"



void Scope_DrawPoint(scope_uint16_t coord_x, scope_uint16_t coord_y, scope_color_t fill_color)
{
    OLED_DrawPoint(coord_x, coord_y, 1);
}

void Scope_ClearPoint(scope_uint16_t coord_x, scope_uint16_t coord_y, scope_color_t fill_color)
{
    OLED_DrawPoint(coord_x, coord_y, 0);
}

void Scope_DrawLine(scope_uint16_t x0, scope_uint16_t y0, 
                    scope_uint16_t x1, scope_uint16_t y1,
                    scope_color_t fill_color)
{
    OLED_DrawLine(x0, y0, x1, y1, 1);

}


void Scope_DrawRect(scope_uint16_t x0, scope_uint16_t y0, 
                    scope_uint16_t x1, scope_uint16_t y1,
                    scope_color_t fill_color)
{
    OLED_DrawLine(x0, y0, x1, y0, 1);
	OLED_DrawLine(x0, y1, x1, y1, 1);
	OLED_DrawLine(x0, y0, x0, y1, 1);
	OLED_DrawLine(x1, y0, x1, y1, 1);
}

void Scope_Fill(scope_uint16_t x0, scope_uint16_t y0, 
                scope_uint16_t x1, scope_uint16_t y1,
                scope_color_t fill_color)
{
    scope_uint16_t i = 0, j = 0;
    if (fill_color == 1)
    {
        for (i = 0; i <= x1 - x0; i++)
        {
            for (j = 0; j <= y1 - y0; j++)
            {
                Scope_DrawPoint(x0 + i, y0 + j, SCOPE_COLOR_WHITE);
            }
        }
    }
    else
    {
        for (i = 0; i <= x1 - x0; i++)
        {
            for (j = 0; j <= y1 - y0; j++)
            {
                Scope_ClearPoint(x0 + i, y0 + j, SCOPE_COLOR_BLACK);
            }
        }
    }
}

void Scope_ShowStr(scope_uint16_t x, scope_uint16_t y, char *buf, scope_uint8_t size, scope_color_t color)
{
    OLED_ShowString(x, y, buf, size, 1);
}

void Scope_ShowStrReverse(scope_uint16_t x, scope_uint16_t y, char *buf, scope_uint8_t size, scope_color_t color)
{
    OLED_ShowString(x, y, buf, size, 0);
}

void Scope_Refresh(void)
{
	OLED_Refresh();
}



//void Scope_WaveRefreshPoint(scope_uint16_t adc_value)
//{
//    scope_uint16_t i, y0, y1;
//    for (i = 0; i < SCOPE_WAVE_POINT_NUM; i++)
//    {
//        y0 = (adc_value/4096.0)*80;
//		y1 = (adc_buf[i+1]/4096.0)*80;
//		Scope_DrawLine(SCOPE_START_X + 10 + i, SCOPE_START_Y + 1, 
//						SCOPE_START_X + 10 + i, SCOPE_END_Y - 10 + 1, 
//						SCOPE_COLOR_BLACK);
//		
//		Scope_DrawLine(SCOPE_START_X + 10 + i, SCOPE_END_Y - 10 - y0, 
//						SCOPE_START_X + 10 + i + 1, SCOPE_END_Y - 10 - y1, 
//						SCOPE_COLOR_WHITE);
////        Scope_DrawPoint(SCOPE_START_X + 10 + i, SCOPE_END_Y - 10 - y0, SCOPE_COLOR_WHITE);
//    }
//}

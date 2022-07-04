#include "scope.h"
#include "bsp.h"
#include "bsp_adc.h"
#include "bsp_gpio.h"
#include <stdio.h>
#include <string.h>



#define Scope_DisplayFre(buf)       {Scope_DrawPicReverse(SCOPE_START_X, SCOPE_START_Y, 8, 8, bmp_frequence); \
                                    Scope_ShowStr(SCOPE_START_X + 8, SCOPE_START_Y, buf, SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);}
#define Scope_DisplayDuty(buf)      {Scope_DrawPicReverse(SCOPE_START_X + (6) * SCOPE_FONT_HOR + 8 * 2, SCOPE_START_Y, 8, 8, bmp_duty); \
                                    Scope_ShowStr(SCOPE_START_X + (6) * SCOPE_FONT_HOR + 8 * 3, SCOPE_START_Y, buf, SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);}
#define Scope_DisplayProgressBar(percent)   {Scope_DrawPic(SCOPE_START_X + (11) * SCOPE_FONT_HOR + 8 * 3, SCOPE_START_Y, 16, 8, bmp_progress_rect); \
                                            Scope_Fill(SCOPE_START_X + (11) * SCOPE_FONT_HOR + 8 * 3 + 1, SCOPE_START_Y + 1, SCOPE_START_X + (11) * SCOPE_FONT_HOR + 8 * 3 + 14, SCOPE_START_Y + 6, SCOPE_COLOR_BLACK); \
                                            Scope_DrawLine(SCOPE_START_X + (11) * SCOPE_FONT_HOR + 8 * 3 + (percent / 100.0) * 14, SCOPE_START_Y + 1, SCOPE_START_X + (11) * SCOPE_FONT_HOR + 8 * 3 + (percent / 100.0) * 14, SCOPE_START_Y + 7, SCOPE_COLOR_WHITE);}


#define Scope_DisplayTimeBase()     Scope_ShowStrReverse(SCOPE_WIN_START_X, SCOPE_WIN_END_Y + 1, hscope.time_base_buf, SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE)

#define Scope_DisplayMode()         {if (hscope.mode.messurement == SCOPE_MODE_NORMAL) \
                                    {Scope_ShowStr(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 7 + 2, SCOPE_WIN_END_Y + 1, "N", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);} \
                                    else if (hscope.mode.messurement == SCOPE_MODE_AUTO) \
                                    {Scope_ShowStr(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 7 + 2, SCOPE_WIN_END_Y + 1, "A", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);} \
                                    else if (hscope.mode.messurement == SCOPE_MODE_SINGLE) \
                                    {Scope_ShowStr(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 7 + 2, SCOPE_WIN_END_Y + 1, "S", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);}}
#define Scope_DisplayModeReverse()  {if (hscope.mode.messurement == SCOPE_MODE_NORMAL) \
                                    {Scope_ShowStrReverse(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 7 + 2, SCOPE_WIN_END_Y + 1, "N", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);} \
                                    else if (hscope.mode.messurement == SCOPE_MODE_AUTO) \
                                    {Scope_ShowStrReverse(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 7 + 2, SCOPE_WIN_END_Y + 1, "A", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);} \
                                    else if (hscope.mode.messurement == SCOPE_MODE_SINGLE) \
                                    {Scope_ShowStrReverse(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 7 + 2, SCOPE_WIN_END_Y + 1, "S", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);}}

#define Scope_DisplayBlock()        {if (hscope.mode.blocked) \
                                    {Scope_ShowStrReverse(SCOPE_WIN_START_X + 6 * SCOPE_FONT_HOR + 7 + 4, SCOPE_WIN_END_Y + 1, "S", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);} \
                                    else \
                                    {Scope_ShowStrReverse(SCOPE_WIN_START_X + 6 * SCOPE_FONT_HOR + 7 + 4, SCOPE_WIN_END_Y + 1, "R", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);} \
                                    Scope_DrawPicReverse(SCOPE_WIN_START_X + 7 * SCOPE_FONT_HOR + 7 + 4, SCOPE_WIN_END_Y + 1, 1, 8, bmp_fill);}

#define Scope_DisplayVoltage(buf)      {Scope_ShowStr((SCOPE_WIDTH - 1) - 12 * SCOPE_FONT_HOR, SCOPE_WIN_END_Y + 1, buf, SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);}

#define Scope_DisplayTrigRising()   {Scope_DrawPicReverse(SCOPE_WIN_START_X + 7 * SCOPE_FONT_HOR + 7 + 4, SCOPE_WIN_END_Y + 1, 1, 8, bmp_fill); \
                                    Scope_DrawPicReverse(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 1, SCOPE_WIN_END_Y + 1, 7, 8, (u8 *)bmp_edge_rising);}
#define Scope_DisplayTrigFalling()  {Scope_DrawPicReverse(SCOPE_WIN_START_X + 7 * SCOPE_FONT_HOR + 7 + 4, SCOPE_WIN_END_Y + 1, 1, 8, bmp_fill); \
                                    Scope_DrawPicReverse(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 1, SCOPE_WIN_END_Y + 1, 7, 8, (u8 *)bmp_edge_falling);}
#define Scope_DisplayTrigRisingReverse()    {Scope_DrawPic(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 1, SCOPE_WIN_END_Y + 1, 7, 8, (u8 *)bmp_edge_rising);}
#define Scope_DisplayTrigFallingReverse()   {Scope_DrawPic(SCOPE_WIN_START_X + 5 * SCOPE_FONT_HOR + 1, SCOPE_WIN_END_Y + 1, 7, 8, (u8 *)bmp_edge_falling);}

#define Scope_DisplayVoltageBase()  {Scope_DrawPic(SCOPE_WIN_START_X + 1, SCOPE_WIN_WAVE_END_Y - 3, 3, 8, bmp_voltage_base);}
#define Scope_DisplayTrigBase()     {Scope_DrawPic(SCOPE_WIN_END_X - 3, SCOPE_WIN_WAVE_END_Y - ((SCOPE_WIN_WAVE_END_Y - SCOPE_WIN_WAVE_START_Y + 1) * ((float)hscope.trig_level / SCOPE_WAVE_VALUE_MAX)), 3, 8, bmp_trig_base);}

scope_t hscope = {0};

static scope_uint16_t sample_gap_per_point; // 屏幕上每点对应采样点的间隔数（采样点的间隔时间小于等于屏幕上的时间间隔，所以要进行缩放）
static scope_uint16_t second_period_start_index;    // 第二个周期在缓存中的开始位置
static scope_uint32_t scope_heart_counter;
static scope_sta_t scope_blinked_sta;


void Scope_ClearHeart(void);
scope_uint32_t Scope_GetHeart(void);
void Scope_HandleEvent(scope_wave_t *wave_buf);
void Scope_HandleInput(void);
void Scope_ReadInput(void);


/**
 * @brief Initial the parament of oscilloscope 
 */
void Scope_Init(void)
{
    hscope.time_base = TB_0ms1;
    hscope.wave_width = 0;
    hscope.trig_level = 1024; // 1/4 of max messure range
	hscope.duty = 0.0;
	hscope.wave_fre = 0;

    hscope.mode.messurement = 0;
    hscope.mode.blocked = false;
    hscope.mode.setting = false;

	Scope_SetTimeBase();
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
    // case TB_50s:    // 2.5s per point
    //     ADC_SetTimerFre(7199, 24999);
    //     hscope.time_base_buf = " 50 s";
    //     break;
    // case TB_20s:    // 1s per point
    //     ADC_SetTimerFre(7199, 9999);
    //     hscope.time_base_buf = " 20 s";
    //     break;
    // case TB_10s:    // 0.5s per point
    //     ADC_SetTimerFre(7199, 4999);
    //     hscope.time_base_buf = " 10 s";
    //     break;
    // case TB_5s:
    //     ADC_SetTimerFre(7199, 2499);
    //     hscope.time_base_buf = "  5 s";
    //     break;
    // case TB_2s:
    //     ADC_SetTimerFre(7199, 999);
    //     hscope.time_base_buf = "  2 s";
    //     break;
    case TB_1s:
        ADC_SetTimerFre(5599, 499);
        hscope.time_base_buf = "  1 s";
        break;
    case TB_0s5:
        ADC_SetTimerFre(559, 2499);
        hscope.time_base_buf = "500ms";
        break;
    case TB_0s2:
        ADC_SetTimerFre(559, 999);
        hscope.time_base_buf = "200ms";
        break;
    case TB_0s1:
        ADC_SetTimerFre(55, 999);
        hscope.time_base_buf = "100ms";
        break;
    case TB_50ms:
        ADC_SetTimerFre(55, 499); // 500us sample time
        sample_gap_per_point = 5;
        hscope.time_base_buf = " 50ms";
        break;
    case TB_20ms:
        ADC_SetTimerFre(55, 49); // 50us sample time
        sample_gap_per_point = 20;
        hscope.time_base_buf = " 20ms";
        break;
    case TB_10ms:
        ADC_SetTimerFre(55, 19); //  20us sample time
        sample_gap_per_point = 25;
        hscope.time_base_buf = " 10ms";
        break;
    case TB_5ms:
        ADC_SetTimerFre(55, 9); // 10us sample time
        sample_gap_per_point = 25;
        hscope.time_base_buf = " 5 ms";
        break;
    case TB_2ms:
        ADC_SetTimerFre(55, 4); // 5us sample time
        sample_gap_per_point = 20;
        hscope.time_base_buf = " 2 ms";
        break;
    case TB_1ms:
        ADC_SetTimerFre(55, 4); // 5us sample time
        sample_gap_per_point = 10;
        hscope.time_base_buf = " 1 ms";
        break;
    case TB_0ms5:
        ADC_SetTimerFre(55, 4); // 5us sample time
        sample_gap_per_point = 5;
        hscope.time_base_buf = "500us";
        break;
    case TB_0ms2:
        ADC_SetTimerFre(55, 4); // 5us sample time
        sample_gap_per_point = 2;
        hscope.time_base_buf = "200us";
        break;
    case TB_0ms1:
        ADC_SetTimerFre(55, 4); // 5us sample time
        sample_gap_per_point = 1;
        hscope.time_base_buf = "100us";
        break;
    case TB_50us:
        ADC_SetTimerFre(27, 4);   // 2.5us sample time
        sample_gap_per_point = 1; //
        hscope.time_base_buf = " 50us";
        break;
    case TB_20us:
        ADC_SetTimerFre(27, 1);
        sample_gap_per_point = 1;
        hscope.time_base_buf = " 20us";
        break;
        // case TB_10us:
        //     ADC_SetTimerFre(71, 1);
        //     hscope.time_base_buf = " 10us";
        //     break;
        // case TB_5us:
        //     ADC_SetTimerFre(71, 1);
        //     hscope.time_base_buf = " 5 us";
        //     break;
        // case TB_2us:
        //     ADC_SetTimerFre(71, 1);
        //     hscope.time_base_buf = " 2 us";
        //     break;
        // case TB_1us:
        //     ADC_SetTimerFre(71, 1);
        //     hscope.time_base_buf = " 1 us";
        break;

    default:
        break;
    }
}

void Scope_DrawNet(void)
{
    scope_uint16_t i, j;
    static scope_uint16_t lines_x_per_gap = (SCOPE_WIN_WAVE_END_X - SCOPE_WIN_WAVE_START_X) / 5; // x：列每两格之间的像素
    static scope_uint16_t lines_y_per_gap = (SCOPE_WIN_WAVE_END_Y - SCOPE_WIN_WAVE_START_Y + 1) / 5; // y：行每两格之间的像素

    Scope_DrawLine(SCOPE_WIN_START_X, SCOPE_WIN_START_Y, SCOPE_WIN_START_X, SCOPE_WIN_END_Y, SCOPE_COLOR_WHITE);
    Scope_DrawLine(SCOPE_WIN_END_X, SCOPE_WIN_START_Y, SCOPE_WIN_END_X, SCOPE_WIN_END_Y, SCOPE_COLOR_WHITE);
    
    Scope_DrawLine(SCOPE_WIN_START_X, SCOPE_WIN_START_Y, 
                        SCOPE_WIN_START_X + 3, SCOPE_WIN_START_Y, SCOPE_COLOR_WHITE);
    Scope_DrawLine(SCOPE_WIN_END_X - 3, SCOPE_WIN_START_Y, 
                        SCOPE_WIN_END_X, SCOPE_WIN_START_Y, SCOPE_COLOR_WHITE);
    Scope_DrawLine(SCOPE_WIN_END_X - 6, SCOPE_WIN_WAVE_START_Y, 
                        SCOPE_WIN_END_X + 3, SCOPE_WIN_WAVE_START_Y, SCOPE_COLOR_WHITE);
    Scope_DrawLine(SCOPE_WIN_START_X, SCOPE_WIN_END_Y, 
                        SCOPE_WIN_END_X, SCOPE_WIN_END_Y, SCOPE_COLOR_WHITE);

    for (i = 1; i <= 4; i++)
    {
        for (j = 0; j < 6; j++)
        {
            Scope_DrawPoint(SCOPE_WIN_WAVE_START_X + i * lines_x_per_gap,
                            SCOPE_WIN_WAVE_START_Y + j * lines_y_per_gap, SCOPE_COLOR_WHITE);

        }
        Scope_DrawLine(SCOPE_WIN_START_X + i * lines_x_per_gap - 2, SCOPE_WIN_START_Y, 
                        SCOPE_WIN_START_X + i * lines_x_per_gap + 3, SCOPE_WIN_START_Y, 
                        SCOPE_COLOR_WHITE);

        Scope_DrawLine(SCOPE_WIN_START_X + i * lines_x_per_gap, SCOPE_WIN_START_Y, 
                        SCOPE_WIN_START_X + i * lines_x_per_gap, SCOPE_WIN_START_Y + 3, 
                        SCOPE_COLOR_WHITE);
        
        Scope_DrawLine(SCOPE_WIN_START_X + i * lines_x_per_gap, SCOPE_WIN_END_Y - 2, 
                        SCOPE_WIN_START_X + i * lines_x_per_gap, SCOPE_WIN_END_Y, 
                        SCOPE_COLOR_WHITE);
    }
}

scope_uint16_t Scope_Value2Net(float data_percent)
{
	scope_uint16_t res;
    if (data_percent >= 1.0)
        data_percent = 1.0;
    else if (data_percent < 0)
        data_percent = 0.0;
	res = (SCOPE_WIN_WAVE_END_Y - SCOPE_WIN_WAVE_START_Y + 1) * data_percent;
	res = SCOPE_WIN_WAVE_END_Y - res;
    return res;
}

void Scope_Start(void)
{
    
    Scope_DrawNet();

}


void Scope_RefreshWaveAuto(scope_wave_t *wave_buf)
{
    scope_uint16_t i, y0, y1;
    scope_uint16_t wave_buf_index_offset ;
	
	if (second_period_start_index > hscope.wave_width)
	{
		wave_buf_index_offset = second_period_start_index - hscope.wave_width;
	}
	else
	{
		wave_buf_index_offset = 0;
	}

    Scope_Fill(SCOPE_WIN_START_X + 1, SCOPE_WIN_START_Y + 1,
               SCOPE_WIN_START_X + SCOPE_WAVE_POINT_NUM - 1, SCOPE_WIN_END_Y - 1,
               SCOPE_COLOR_BLACK);

    for (i = 0; i < SCOPE_WAVE_POINT_NUM; i++)
    {
        y0 = Scope_Value2Net((float)wave_buf[wave_buf_index_offset + i * sample_gap_per_point] / (SCOPE_WAVE_VALUE_MAX + 1));
        y1 = Scope_Value2Net((float)wave_buf[wave_buf_index_offset + (i + 1) * sample_gap_per_point] / (SCOPE_WAVE_VALUE_MAX + 1));

        Scope_DrawLine(SCOPE_WIN_WAVE_START_X + i, y0,
        				SCOPE_WIN_WAVE_START_X + i + 1, y1,
        				SCOPE_COLOR_WHITE);
        // Scope_DrawPoint(SCOPE_WIN_START_X + i, SCOPE_WIN_END_Y - 10 - y0, SCOPE_COLOR_WHITE);
    }
    Scope_DrawNet();
}

void Scope_RefreshWave(scope_wave_t *wave_buf)
{
    scope_uint16_t i, y0, y1;
	
    Scope_Fill(SCOPE_WIN_START_X + 1, SCOPE_WIN_START_Y + 1,
               SCOPE_WIN_START_X + SCOPE_WAVE_POINT_NUM - 1, SCOPE_WIN_END_Y - 1,
               SCOPE_COLOR_BLACK);

    for (i = 0; i < SCOPE_WAVE_POINT_NUM; i++)
    {
        y0 = Scope_Value2Net((float)wave_buf[i * sample_gap_per_point] / (SCOPE_WAVE_VALUE_MAX + 1));
        y1 = Scope_Value2Net((float)wave_buf[(i + 1) * sample_gap_per_point] / (SCOPE_WAVE_VALUE_MAX + 1));
		
        Scope_DrawLine(SCOPE_WIN_WAVE_START_X + i, y0,
        				SCOPE_WIN_WAVE_START_X + i + 1, y1,
        				SCOPE_COLOR_WHITE);
        // Scope_DrawPoint(SCOPE_WIN_START_X + i, SCOPE_WIN_END_Y - 10 - y0, SCOPE_COLOR_WHITE);
    }
    Scope_DrawNet();
}


/**
 * @brief Calculate wave width
 * @param wave_buf wave buffer
 * @return the state of calculate wave width, ture: success false: false
 */
scope_sta_t Scope_IsWaveTrigPoint(scope_wave_t p1, scope_wave_t p2, scope_wave_t trig_level)
{
    /* rising trigger */
    if (hscope.mode.trigger == 0)
    {
        if (p1 <= trig_level && p2 > trig_level)
            return true;
        else
            return false;
    }
    /* falling trigger */
    else
    {
        if (p1 >= trig_level && p2 < trig_level)
            return true;
        else
            return false;
    }
}

void Scope_AddTimeBase(void)
{
    if (hscope.time_base++ >= TB_Max)
        hscope.time_base = TB_Max;
    Scope_SetTimeBase();
}

void Scope_SubTimeBase(void)
{
    if (hscope.time_base-- <= TB_Min)
        hscope.time_base = TB_Min;
    Scope_SetTimeBase();
}

scope_sta_t Scope_GetWavePara(scope_wave_t *wave_buf)
{
    scope_uint16_t i;
    scope_uint16_t index, iduty = 0;
    scope_uint16_t period_start_pos = 0;
    scope_uint16_t widths[3] = {0};
    float fdutys[3] = {0};
    scope_sta_t res = false;
    scope_sta_t period_start_found_sta = 0;
    scope_uint8_t period_found_count = 0;
    /* rising wave triggle */
    for (i = 0; i < SCOPE_WAVE_POINT_BUF_NUM - 1; i++)
    {
        /* start trigger point had been found */
        if (!period_start_found_sta)
        {
            if (Scope_IsWaveTrigPoint(wave_buf[i], wave_buf[i + 1], hscope.trig_level))
            {
                period_start_pos = i;
                period_start_found_sta = true;

                /* save the second period start point of wave buffer */
                if (period_found_count == 1)
                {
                    second_period_start_index = i;
                }
            }
        }
        else
        {
            if (wave_buf[i] >= hscope.trig_level)
            {
                iduty++;
            }
            if (Scope_IsWaveTrigPoint(wave_buf[i], wave_buf[i + 1], hscope.trig_level))
            {
                widths[period_found_count] = i - period_start_pos;
                fdutys[period_found_count] = (float)iduty / widths[period_found_count];
                period_found_count++;
                iduty = 0;
                res = true;
                i -= 1; // index -1 for setting this point as start trigger point
                period_start_found_sta = false;
            }
        }

        /* meidan fliter */
        if (period_found_count >= 3)
        {
            hscope.wave_width = widths[0] > widths[1] ? (widths[0] > widths[2] ? widths[1] : \
                                (widths[0] > widths[2] ? widths[2] : widths[0])) : \
                                (widths[0] > widths[2] ? widths[0] : \
                                (widths[1] > widths[2] ? widths[2] : widths[1]));

            hscope.duty = fdutys[0] > fdutys[1] ? (fdutys[0] > fdutys[2] ? fdutys[1] : \
                            (fdutys[0] > fdutys[2] ? fdutys[2] : fdutys[0])) : \
                            (fdutys[0] > fdutys[2] ? fdutys[0] : \
                            (fdutys[1] > fdutys[2] ? fdutys[2] : fdutys[1]));
            break;
        }
    }

    if (period_found_count > 0 && period_found_count < 3)
    {
        hscope.wave_width = widths[0];
        hscope.duty = fdutys[0];
    }
    return res;
}

scope_sta_t Scope_GetWaveFre(scope_wave_t *wave_buf)
{
    scope_uint16_t time_gap = hscope.wave_width / sample_gap_per_point;
    /* trigger success, calculate wave frequence */
    if (Scope_GetWavePara(wave_buf))
    {
        switch (hscope.time_base)
        {
        // case TB_50s:    // 2.5s per point
        //     hscope.wave_fre = 1;
        //     break;
        // case TB_20s:    // 1s per point
        //     hscope.wave_fre = 1;
        //     break;
        // case TB_10s:    // 0.5s per point
        //     hscope.wave_fre = 1;
        //     break;
        // case TB_5s:
        //     hscope.wave_fre = 1;
        //     break;
        // case TB_2s:
        //     hscope.wave_fre = 1.0/((2.0/SCOPE_WAVE_POINTS_PER_TIMEBASE)*time_gap);
        //     break;
        case TB_1s:
            hscope.wave_fre = 1.0 / ((1.0 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_0s5:
            hscope.wave_fre = 1.0 / ((0.5 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_0s2:
            hscope.wave_fre = 1.0 / ((0.2 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_0s1:
            hscope.wave_fre = 1.0 / ((0.1 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_50ms:
            hscope.wave_fre = 1.0 / ((0.05 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_20ms:
            hscope.wave_fre = 1.0 / ((0.02 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_10ms:
            hscope.wave_fre = 1.0 / ((0.01 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_5ms:
            hscope.wave_fre = 1.0 / ((0.005 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_2ms:
            hscope.wave_fre = 1.0 / ((0.002 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_1ms:
            hscope.wave_fre = 1.0 / ((0.001 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_0ms5:
            hscope.wave_fre = 1.0 / ((0.0005 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_0ms2:
            hscope.wave_fre = 1.0 / ((0.0002 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_0ms1:
            hscope.wave_fre = 1.0 / ((0.0001 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_50us:
            hscope.wave_fre = 1.0 / ((0.00005 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
        case TB_20us:
            hscope.wave_fre = 1.0 / ((0.00002 / SCOPE_WAVE_POINTS_PER_TIMEBASE) * time_gap);
            break;
            // case TB_10us:
            //     hscope.wave_fre = 1.0/((0.00001/SCOPE_WAVE_POINTS_PER_TIMEBASE)*time_gap);
            //     break;
            // case TB_5us:
            //     hscope.wave_fre = 1.0/((0.000005/SCOPE_WAVE_POINTS_PER_TIMEBASE)*time_gap);
            //     break;
            // case TB_2us:
            //     hscope.wave_fre = 1.0/((0.000002/SCOPE_WAVE_POINTS_PER_TIMEBASE)*time_gap);
            //     break;
            // case TB_1us:
            //     hscope.wave_fre = 1.0/((0.000001/SCOPE_WAVE_POINTS_PER_TIMEBASE)*time_gap);
            //     break;

        default:
            break;
        }
        return true;
    } // ! end of if (Scope_GetWaveWidth(wave_buf))
    return false;
}

void Scope_GetWaveVoltage(scope_wave_t *wave_buf)
{
    scope_uint16_t i;
    scope_wave_t wave_max = 0;
    scope_wave_t wave_min = SCOPE_WAVE_VALUE_MAX;
    for (i = 0; i < SCOPE_WAVE_POINT_NUM * sample_gap_per_point; i++)
    {
        if (wave_max < wave_buf[i])
        {
            wave_max = wave_buf[i];
        }
        if (wave_min > wave_buf[i])
        {
            wave_min = wave_buf[i];
        }
    }
    hscope.wave_value_max = wave_max;
    hscope.wave_value_min = wave_min;
    hscope.vpp = wave_max * (SCOPE_WAVE_VMAX / SCOPE_WAVE_VALUE_MAX);
    hscope.vmin = wave_min * (SCOPE_WAVE_VMAX / SCOPE_WAVE_VALUE_MAX);
}

void Scope_Auto(scope_wave_t *wave_buf)
{
    int i;
    for (i = TB_Max; i >= TB_Min; i--)
    {
        hscope.time_base = i;
        Scope_SetTimeBase();
        ADC_ResetTransmitedSta();
        ADC_DMAStartTransmit();
        while (!ADC_GetTransmitedSta())
            ;
        if (Scope_GetWaveFre(wave_buf))
        {
            if ((hscope.wave_width / sample_gap_per_point) >= 20 && (hscope.wave_width / sample_gap_per_point) < 50)
            {
                break;
            }

            if ((hscope.wave_width / sample_gap_per_point) < 20)
            {
                return;
            }
        }
    }
}

void Scope_Task(scope_wave_t *wave_buf)
{
    Scope_HandleEvent(wave_buf);
}


void Scope_HandleEvent(scope_wave_t *wave_buf)
{
    static char buf[16];
	static scope_uint32_t heart;
	
	heart = Scope_GetHeart();
    /* handle event */
	if (SCOPE_EVENT_READY(heart, SCOPE_EVENT_INPUT))
    {
        Scope_HandleInput();
    }
	
    /* handle adc change */
	if (SCOPE_EVENT_READY(heart, SCOPE_EVENT_MESSURE))
	{
        /* running in normal mode */
        if (hscope.mode.messurement == SCOPE_MODE_NORMAL)
        {
            if (hscope.mode.blocked)
            {
                ADC_DMAStopTransmit();
            }
            else
            {
                if (ADC_GetTransmitedSta())
                {
                    ADC_ResetTransmitedSta();
                    ADC_DMAStartTransmit();     
                }
            }
        }
        /* running in auto mde */
        else if (hscope.mode.messurement == SCOPE_MODE_AUTO)
        {
            if (hscope.mode.blocked)
            {
                ADC_DMAStopTransmit();
            }
            else
            {
                if (ADC_GetTransmitedSta())
                {
                    ADC_ResetTransmitedSta();
                    ADC_DMAStartTransmit();     
                }
            }
        }
        else if (hscope.mode.messurement == SCOPE_MODE_SINGLE)
        {

        }
	}

    /* handle display */
    if (SCOPE_EVENT_READY(heart, SCOPE_EVENT_REFRESH))
    {
        if (hscope.mode.messurement == SCOPE_MODE_NORMAL || hscope.mode.messurement == SCOPE_MODE_AUTO)
        {
            if (hscope.mode.messurement == SCOPE_MODE_AUTO)
            {
                Scope_Auto(wave_buf);
            }
            if (ADC_GetTransmitedSta())
            {
                Scope_GetWaveVoltage(wave_buf);

                /* handler frequence */
                if (Scope_GetWaveFre(wave_buf))
                {
                    /* display duty */
                    sprintf(buf, " %2d%%", (int)(hscope.duty * 100));
                    Scope_DisplayDuty(buf);

                    if (hscope.wave_fre / 1000 > 0)
                    {
                        sprintf(buf, "%3dKHz", hscope.wave_fre / 1000);
                    }
                    else
                    {
                        sprintf(buf, "%3d Hz", hscope.wave_fre);
                    }
                }
                else
                {
                    sprintf(buf, "--- Hz");

                    if (hscope.wave_value_max >= hscope.trig_level)
                    {
                        Scope_DisplayDuty("100\%");
                    }
                    else
                    {
                        Scope_DisplayDuty("  0\%");
                    }
                }

                /* display frequence */
                Scope_DisplayFre(buf);
            }   // ! end of if (ADC_GetTransmitedSta())

            /* display voltage */
            sprintf(buf, "%5.2f-%5.2fv", hscope.vmin, hscope.vpp);
            
            Scope_DisplayVoltage(buf);

            /* display wave */
            if (hscope.mode.messurement == SCOPE_MODE_NORMAL)
            {
                Scope_RefreshWave(wave_buf);
            }
            else
            {
                Scope_RefreshWaveAuto(wave_buf);
            }
            
        }
        else if (hscope.mode.messurement == SCOPE_MODE_SINGLE)
        {


        }

        /* display timebase */
        Scope_DisplayTimeBase();

        if (!hscope.mode.setting)
        {
            /* display trigger edge */
            if (hscope.mode.trigger == 0)
            {
                Scope_DisplayTrigRising();
            }
            else
            {
                Scope_DisplayTrigFalling();
            }

            /* display mode */
            Scope_DisplayMode();
        }
        
        /* display wave blocked sta */
        Scope_DisplayBlock();

        Scope_DisplayVoltageBase();
        Scope_DisplayTrigBase();

        
        Scope_DisplayProgressBar(30);

        Scope_Refresh();
    }
    
    if (SCOPE_EVENT_READY(heart, SCOPE_EVENT_BLINK))
    {
        if (hscope.mode.setting)
        {
            if (!scope_blinked_sta)
            {
                switch (hscope.setting_type)
                {
                case SCOPE_SETTING_MODE:
                    Scope_DisplayMode();
                    break;
                case SCOPE_SETTING_TRIG:
                    if (hscope.mode.trigger == 0)
                    {
                        Scope_DisplayTrigRising();
                    }
                    else
                    {
                        Scope_DisplayTrigFalling();
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch (hscope.setting_type)
                {
                case SCOPE_SETTING_MODE:
                    Scope_DisplayModeReverse();
                    break;
                case SCOPE_SETTING_TRIG:
                    if (hscope.mode.trigger == 0)
                    {
                        Scope_DisplayTrigRisingReverse();
                    }
                    else
                    {
                        Scope_DisplayTrigFallingReverse();
                    }
                    break;
                default:
                    break;
                }
            }
            scope_blinked_sta = !scope_blinked_sta;
			Scope_Refresh();
        }
    }
}

void Scope_HandleInput(void)
{
	Scope_ReadInput();

    if (hscope.event.key_set)
    {
        hscope.mode.setting = !hscope.mode.setting;
        hscope.event.key_set = 0;
    }

    /* setting mode */
    if (hscope.mode.setting)
    {
        /* shift setting type */
        if (hscope.event.key_left)
        {
            if (hscope.setting_type-- <= SCOPE_SETTING_MIN)
                hscope.setting_type = SCOPE_SETTING_MAX;
			hscope.event.key_left = 0;
        }
        else if (hscope.event.key_right)
        {
            if (hscope.setting_type++ >= SCOPE_SETTING_MAX)
                hscope.setting_type = SCOPE_SETTING_MIN;
			hscope.event.key_right = 0;
        }

        /* set messurement mode, eg: normal,auto or single */
        if (hscope.setting_type == SCOPE_SETTING_MODE)
        {
            if (hscope.event.key_up)
            {
                if (hscope.mode.messurement++ >= SCOPE_MODE_MAX)
                    hscope.mode.messurement = SCOPE_MODE_MAX;
                hscope.event.key_up = 0;
            }
            else if (hscope.event.key_down)
            {
                if (hscope.mode.messurement-- <= SCOPE_MODE_MIN)
                    hscope.mode.messurement = SCOPE_MODE_MIN;
                hscope.event.key_down = 0;
            }
        }
        /* set trigger mode, eg: rising trigger or falling trigger */
        else if (hscope.setting_type == SCOPE_SETTING_TRIG)
        {
            if (hscope.event.key_up)
            {
                hscope.mode.trigger = !hscope.mode.trigger;
                hscope.event.key_up = 0;
            }
            else if (hscope.event.key_down)
            {
                hscope.mode.trigger = !hscope.mode.trigger;
                hscope.event.key_down = 0;
            }
        }
    }
    /* not setting mode */
    else
    {
        /* running in the normal mode */
        if (hscope.mode.messurement == SCOPE_MODE_NORMAL)
        {
            /* pause key pressed */
            if (hscope.event.key_mid_u.key_pause)
            {
                hscope.mode.blocked = !hscope.mode.blocked;
                hscope.event.key_mid_u.key_pause = 0;
            }

            if (hscope.event.key_up)
            {
                Scope_AddTimeBase();
                hscope.event.key_up = 0;
            }

            if (hscope.event.key_down)
            {
                Scope_SubTimeBase();
                hscope.event.key_down = 0;
            }

        }
        /* runing in the auto mode */
        else if (hscope.mode.messurement == SCOPE_MODE_AUTO)
        {
        }
        /* runing in the single mode */
        else if (hscope.mode.messurement == SCOPE_MODE_SINGLE)
        {
        }
    }
}

/* nms heart */
void Scope_UpdateHeart(scope_uint8_t nms)
{
    scope_heart_counter += nms;
}

void Scope_ClearHeart(void)
{
    scope_heart_counter = 0;
}

scope_uint32_t Scope_GetHeart(void)
{
    return scope_heart_counter;
}


void Scope_ReadInput(void)
{
    scope_uint8_t key_value;
    key_value = Key_Scan(0);
    if (key_value == KEY_UP_PRESS)
        hscope.event.key_up = 1;
    else if (key_value == KEY_DOWN_PRESS)
        hscope.event.key_down = 1;
    else if (key_value == KEY_MID_PRESS)
        hscope.event.key_mid_u.key_pause = 1;
    else if (key_value == KEY_SET_PRESS)
        hscope.event.key_set = 1;
    else if (key_value == KEY_LEFT_PRESS)
        hscope.event.key_left = 1;
    else if (key_value == KEY_RIGHT_PRESS)
        hscope.event.key_right = 1;
}

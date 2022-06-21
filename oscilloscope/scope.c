#include "scope.h"
#include "bsp.h"
#include "bsp_adc.h"
#include <stdio.h>
#include <string.h>

scope_t hscope = {0};

static scope_uint16_t sample_gap_per_point; // 屏幕上每点对应采样点的间隔数（采样点的间隔时间小于等于屏幕上的时间间隔，所以要进行缩放）
static scope_uint16_t second_period_start_index;    // 第二个周期在缓存中的开始位置
static scope_uint32_t scope_heart_counter;



void Scope_ClearHeart(void);
scope_uint32_t Scope_GetHeart(void);


void Scope_Init(void)
{
    hscope.time_base = TB_1ms;
    hscope.wave_width = 0;
    hscope.trig_level = 1024; // 1/4 of max messure range
	hscope.duty = 0.0;
	hscope.wave_fre = 0;

    hscope.mode.messurement = 0;
    hscope.mode.blocked = false;

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
    static scope_uint16_t lines_x_per_gap = (SCOPE_END_X - SCOPE_START_X) / 5; // x：列每两格之间的像素
    static scope_uint16_t lines_y_per_gap = (SCOPE_END_Y - SCOPE_START_Y) / 5; // y：行每两格之间的像素

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

    // Scope_Fill(SCOPE_END_X + 2, SCOPE_START_Y, SCOPE_WIDTH - 1, SCOPE_END_Y, 1);
    // Scope_Fill(SCOPE_END_X + 2, SCOPE_START_Y + 12*1, SCOPE_WIDTH - 1, SCOPE_START_Y + 12*2, 0);
    // Scope_Fill(SCOPE_END_X + 2, SCOPE_START_Y + 12*3, SCOPE_WIDTH - 1, SCOPE_START_Y + 12*4, 0);
}

void Scope_RefreshWave(scope_wave_t *wave_buf)
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

    Scope_Fill(SCOPE_START_X + 1, SCOPE_START_Y + 1,
               SCOPE_START_X + SCOPE_WAVE_POINT_NUM - 1, SCOPE_END_Y - 1,
               SCOPE_COLOR_BLACK);

    for (i = 0; i < SCOPE_WAVE_POINT_NUM; i++)
    {
        y0 = (wave_buf[wave_buf_index_offset + i * sample_gap_per_point] / (float)(SCOPE_WAVE_VALUE_MAX + 1)) * 40;
        y1 = (wave_buf[wave_buf_index_offset + (i + 1) * sample_gap_per_point] / (float)(SCOPE_WAVE_VALUE_MAX + 1)) * 40;

        Scope_DrawLine(SCOPE_START_X + i, SCOPE_END_Y - y0 - 10,
        				SCOPE_START_X + i + 1, SCOPE_END_Y - y1 - 10,
        				SCOPE_COLOR_WHITE);
        // Scope_DrawPoint(SCOPE_START_X + i, SCOPE_END_Y - 10 - y0, SCOPE_COLOR_WHITE);
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
    static char buf[12];

    /* handler frequence */
    Scope_ShowStr(SCOPE_END_X + 1, SCOPE_START_Y, "     ", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);
    Scope_ShowStr(SCOPE_END_X + 5, SCOPE_START_Y + SCOPE_FONT_SIZE, "    ", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);

    Scope_GetWaveVoltage(wave_buf);

    if (Scope_GetWaveFre(wave_buf))
    {
        sprintf(buf, " %2d%%", (int)(hscope.duty * 100));
        Scope_ShowStr(SCOPE_END_X + 3, SCOPE_START_Y + SCOPE_FONT_SIZE * 3, buf, SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);

        if (hscope.wave_fre / 1000 > 0)
        {
            if (hscope.wave_fre / 100000 > 0)
                sprintf(buf, "%3d", hscope.wave_fre / 1000);
            else
                sprintf(buf, "%.1f", hscope.wave_fre / 1000.0);
            Scope_ShowStr(SCOPE_END_X + 5, SCOPE_START_Y + SCOPE_FONT_SIZE, "KHz", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);
        }
        else
        {
            sprintf(buf, "%3d", hscope.wave_fre);
            Scope_ShowStr(SCOPE_END_X + 5, SCOPE_START_Y + SCOPE_FONT_SIZE, " Hz", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);
        }
    }
    else
    {
        sprintf(buf, " ---");
        Scope_ShowStr(SCOPE_END_X + 5, SCOPE_START_Y + SCOPE_FONT_SIZE, " Hz", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);

        if (hscope.wave_value_max >= hscope.trig_level)
        {
            Scope_ShowStr(SCOPE_END_X + 3, SCOPE_START_Y + SCOPE_FONT_SIZE * 3, "100\%", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);
        }
        else
        {
            Scope_ShowStr(SCOPE_END_X + 3, SCOPE_START_Y + SCOPE_FONT_SIZE * 3, "  0\%", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);
        }
    }
    /* display frequence */
    Scope_ShowStr(SCOPE_END_X + 1 + ((4 - strlen(buf)) * SCOPE_FONT_SIZE), SCOPE_START_Y, buf, SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);

	/* display wave */
    Scope_RefreshWave(wave_buf);
	
    /* display timebase */
    Scope_ShowStrReverse(SCOPE_START_X, SCOPE_END_Y + 1, hscope.time_base_buf, SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);

    /* display trigger edge */
    Scope_DrawPicReverse(SCOPE_START_X + 5 * SCOPE_FONT_HOR + 1, SCOPE_END_Y + 1, 7, 8, (u8 *)bmp_edge_rising);

    /* display mode */
    Scope_ShowStr(SCOPE_START_X + 5 * SCOPE_FONT_HOR + 7 + 2, SCOPE_END_Y + 1, "A", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);

    
    /* display wave blocked sta */
    if (hscope.mode.blocked)
    {
        Scope_ShowStrReverse(SCOPE_START_X + 6 * SCOPE_FONT_HOR + 7 + 4, SCOPE_END_Y + 1, "S", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);
    }
    else
    {
        Scope_ShowStrReverse(SCOPE_START_X + 6 * SCOPE_FONT_HOR + 7 + 4, SCOPE_END_Y + 1, "R", SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);
    }
    Scope_DrawPicReverse(SCOPE_START_X + 7 * SCOPE_FONT_HOR + 7 + 4, SCOPE_END_Y + 1, 1, 8, bmp_fill);

    /* display voltage */
    sprintf(buf, "%.2f-%.2fv", hscope.vmin, hscope.vpp);
    Scope_ShowStr((SCOPE_WIDTH - 1) - strlen(buf) * SCOPE_FONT_HOR, SCOPE_END_Y + 1, buf, SCOPE_FONT_SIZE, SCOPE_COLOR_WHITE);

    Scope_DrawPic(SCOPE_START_X + 1, 64 / 2, 3, 8, bmp_voltage_base);
    Scope_DrawPic(SCOPE_END_X - 3, 64 / 2, 3, 8, bmp_trig_base);

    Scope_Refresh();
}


void Scope_HandleEvent()
{

    /* handle event */
    if ((Scope_GetHeart() % SCOPE_EVENT_INPUT) == 0)
    {
        Scope_HandleInput();
    }

    /* handle display */
    if ((Scope_GetHeart() % SCOPE_EVENT_REFRESH) == 0)
    {
        
    }
}

void Scope_HandleInput(void)
{
    if (hscope.mode.messurement == SCOPE_MODE_NORMAL)
    {
        /* pause key pressed */
        if (hscope.event.key_pause)
        {
            hscope.mode.blocked = !hscope.mode.blocked;

            hscope.event.key_pause = false;
        }

        if (hscope.event.key_up)
        {
            Scope_AddTimeBase();

            hscope.event.key_up = false;
        }

        if (hscope.event.key_down)
        {
            Scope_SubTimeBase();

            hscope.event.key_down = false;
        }

    }
    else if (hscope.mode.messurement == SCOPE_MODE_AUTO)
    {
    }
    else if (hscope.mode.messurement == SCOPE_MODE_SINGLE)
    {
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

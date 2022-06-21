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

void Scope_DrawPic(scope_uint16_t x, scope_uint16_t y, scope_uint16_t sizex, scope_uint16_t sizey, const char *bmp)
{
    u16 j = 0;
    u8 i, n, temp, m;
    u8 x0 = x, y0 = y;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    for (n = 0; n < sizey; n++)
    {
        for (i = 0; i < sizex; i++)
        {
            temp = bmp[j];
            j++;
            for (m = 0; m < 8; m++)
            {
                if (temp & 0x01)
                    Scope_DrawPoint(x, y, SCOPE_COLOR_WHITE);
                else
                    Scope_ClearPoint(x, y, SCOPE_COLOR_BLACK);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == sizex)
            {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}

void Scope_DrawPicReverse(scope_uint16_t x, scope_uint16_t y, scope_uint16_t sizex, scope_uint16_t sizey, const char *bmp)
{
    u16 j = 0;
    u8 i, n, temp, m;
    u8 x0 = x, y0 = y;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    for (n = 0; n < sizey; n++)
    {
        for (i = 0; i < sizex; i++)
        {
            temp = bmp[j];
            j++;
            for (m = 0; m < 8; m++)
            {
                if (temp & 0x01)
                    Scope_ClearPoint(x, y, SCOPE_COLOR_WHITE);
                else
                    Scope_DrawPoint(x, y, SCOPE_COLOR_BLACK);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == sizex)
            {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}


const unsigned char bmp_edge_down[] = {
    0x00, 0x02, 0x0a, 0x7e, 0x50, 0x40, 0x00};

const unsigned char bmp_edge_rising[] = {
    0x00, 0x40, 0x50, 0x7e, 0x0a, 0x02, 0x00};

const unsigned char bmp_voltage_base[] = {
    0x7e, 0x3c, 0x18};

const unsigned char bmp_trig_base[] = {
    0x18, 0x3c, 0x7e};

const unsigned char bmp_fill[] = {
    0x00};
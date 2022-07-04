#ifndef SCOPE_GUI_H
#define SCOPE_GUI_H

#include "scope_conf.h"

extern const unsigned char bmp_edge_down[];
extern const unsigned char bmp_edge_rising[];
extern const unsigned char bmp_edge_falling[];
extern const unsigned char bmp_voltage_base[];
extern const unsigned char bmp_trig_base[];
extern const unsigned char bmp_fill[];
extern const unsigned char bmp_frequence[];
extern const unsigned char bmp_duty[];
extern const unsigned char bmp_beacon_adown[];
extern const unsigned char bmp_progress_rect[];

void Scope_Fill(scope_uint16_t x0, scope_uint16_t y0, 
                scope_uint16_t x1, scope_uint16_t y1,
                scope_color_t fill_color);
void Scope_DrawRect(scope_uint16_t x0, scope_uint16_t y0, 
                    scope_uint16_t x1, scope_uint16_t y1,
                    scope_color_t fill_color);
void Scope_DrawLine(scope_uint16_t x0, scope_uint16_t y0, 
                    scope_uint16_t x1, scope_uint16_t y1,
                    scope_color_t fill_color);
void Scope_ClearPoint(scope_uint16_t coord_x, scope_uint16_t coord_y, scope_color_t fill_color);
void Scope_DrawPoint(scope_uint16_t coord_x, scope_uint16_t coord_y, scope_color_t fill_color);
void Scope_Refresh(void);
void Scope_ShowStr(scope_uint16_t x, scope_uint16_t y, char *buf, scope_uint8_t size, scope_color_t color);
void Scope_ShowStrReverse(scope_uint16_t x, scope_uint16_t y, char *buf, scope_uint8_t size, scope_color_t color);
void Scope_DrawPic(scope_uint16_t x, scope_uint16_t y, scope_uint16_t sizex, scope_uint16_t sizey, const char *bmp);
void Scope_DrawPicReverse(scope_uint16_t x, scope_uint16_t y, scope_uint16_t sizex, scope_uint16_t sizey, const char *bmp);



#endif

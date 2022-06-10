#ifndef SCOPE_GUI_H
#define SCOPE_GUI_H

#include "scope_conf.h"

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

#endif

#ifndef SCOPE_CONF_H
#define SCOPE_CONF_H

#include "bsp.h"

#define SCOPE_WIDTH     128
#define SCOPE_HEIGHT    64

#define SCOPE_WAVE_POINT_NUM    100

#define SCOPE_START_X	(0)
#define SCOPE_END_X		(SCOPE_WAVE_POINT_NUM)

#define SCOPE_START_Y	(0)
#define SCOPE_END_Y		(63-8)

typedef uint8_t scope_uint8_t;
typedef uint16_t scope_uint16_t;
typedef uint32_t scope_uint32_t;

#define SCOPE_COLOR_DEPTH   16
typedef uint16_t scope_color_t;

#define SCOPE_COLOR_BLACK   0x0000
#define SCOPE_COLOR_WHITE   0xFFFF


#endif

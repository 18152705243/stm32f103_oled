#ifndef SCOPE_CONF_H
#define SCOPE_CONF_H

#include "bsp.h"

#define SCOPE_WIDTH     128
#define SCOPE_HEIGHT    64

#define SCOPE_WAVE_POINT_NUM    100
#define SCOPE_WAVE_POINT_BUF_NUM    4000
#define SCOPE_WAVE_POINTS_PER_TIMEBASE  (SCOPE_WAVE_POINT_NUM/5)
#define SCOPE_WAVE_VMAX     (15.0f)  /* maxium voltage */
#define SCOPE_WAVE_VALUE_MAX    (4095)  /* maxium value */


#define SCOPE_START_X	(0)
#define SCOPE_END_X		(SCOPE_WIDTH - 1)
#define SCOPE_START_Y	(0)
#define SCOPE_END_Y		(SCOPE_HEIGHT - 1)


#define SCOPE_WIN_START_X	(0)
#define SCOPE_WIN_END_X		(SCOPE_WAVE_POINT_NUM)
#define SCOPE_WIN_START_Y	(0 + SCOPE_FONT_VER)
#define SCOPE_WIN_END_Y		(63-8)

#define SCOPE_WIN_WAVE_START_X  (SCOPE_WIN_START_X)
#define SCOPE_WIN_WAVE_END_X    (SCOPE_WIN_END_X)
#define SCOPE_WIN_WAVE_START_Y  (SCOPE_WIN_START_Y + 4)
#define SCOPE_WIN_WAVE_END_Y    (SCOPE_WIN_END_Y - 4)

typedef uint8_t scope_uint8_t;
typedef uint16_t scope_uint16_t;
typedef uint32_t scope_uint32_t;

typedef scope_uint16_t scope_wave_t;

#define SCOPE_COLOR_DEPTH   16
typedef uint16_t scope_color_t;

typedef char scope_sta_t;

#define SCOPE_COLOR_BLACK   0x0000
#define SCOPE_COLOR_WHITE   0xFFFF
#define SCOPE_FONT_SIZE 8
#define SCOPE_FONT_HOR  6
#define SCOPE_FONT_VER  8


#define SCOPE_EVENT_INPUT       (50)	/* period of input event */
#define SCOPE_EVENT_REFRESH     (200)   /* period of display event */
#define SCOPE_EVENT_MESSURE     (300)   /* period of display event */
#define SCOPE_EVENT_BLINK       (800)

#ifndef true
    #define true 1
    #define false 0
#endif // !true || false


#endif

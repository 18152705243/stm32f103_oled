#ifndef SCOPE_H
#define SCOPE_H

#include "scope_conf.h"
#include "scope_gui.h"

struct scope_event_s {
    /* the follow is the state of key press, 0: released    1: pressed */
    scope_sta_t key_up : 1;
    scope_sta_t key_down : 1;
    scope_sta_t key_left : 1;
    scope_sta_t key_right : 1;
    scope_sta_t key_set : 1;
    union
    {
        scope_sta_t key_ok : 1;
        scope_sta_t key_pause : 1;
    }key_mid_u;
    
    // union
    // {
    //     scope_sta_t key_ok : 1;
    // }key_reset_u;
    
    // scope_sta_t key_shift : 1;
    // scope_sta_t key_return : 1;
};

struct scope_mode_s {
	scope_sta_t messurement : 2;
	scope_sta_t blocked : 1;
    scope_sta_t trigger : 1;   /* !< trigger mode, 0: rising   1: falling */
    scope_sta_t setting : 1;        /* !< setting mode, 0: not setting  1: setting */
};

typedef struct scope_s {
    struct scope_mode_s mode;
    scope_uint8_t time_base;
    scope_uint16_t wave_width;  /* !< */
    scope_uint16_t trig_level;  /* !< tirgger level of wave */
    scope_wave_t wave_value_max;
    scope_wave_t wave_value_min;
    scope_uint32_t wave_fre;    /* !< wave frequence, wave_fre Hz */
    struct scope_event_s event;
    scope_uint8_t setting_type;
    char *time_base_buf;
    float vpp;
    float vmin;
    float duty;
}scope_t;


enum Scope_TimeBase {
	TB_Min,
    // TB_50s,
    // TB_20s,
    // TB_10s,
    // TB_5s,
    // TB_2s,		
	TB_1s = TB_Min,		
	TB_0s5,		
	TB_0s2,		
	TB_0s1,		
	TB_50ms,	
	TB_20ms,	
	TB_10ms,	
	TB_5ms,		
	TB_2ms,		
	TB_1ms,		
	TB_0ms5,	
	TB_0ms2,	
	TB_0ms1,	
	TB_50us,	
	TB_20us,	
	// TB_10us,	
	// TB_5us,		
	// TB_2us,		
	// TB_1us,
    TB_Max = TB_20us
	// TB_0us5,	
	// TB_0us2,
	// TB_0us1,
	// TB_50ns,
	// TB_20ns,
	// TB_10ns,
};

enum SCOPE_MODE {
    SCOPE_MODE_MIN,
    SCOPE_MODE_NORMAL = SCOPE_MODE_MIN,
    SCOPE_MODE_AUTO,
    SCOPE_MODE_SINGLE,
    SCOPE_MODE_MAX = SCOPE_MODE_SINGLE
};


enum SCOPE_SETTING {
    SCOPE_SETTING_MIN,
    SCOPE_SETTING_MODE = SCOPE_SETTING_MIN,
    SCOPE_SETTING_TRIG,
    SCOPE_SETTING_MAX = SCOPE_SETTING_TRIG
};


#define SCOPE_EVENT_READY(event, time)	((event % time) == 0 ? 1 : 0)


void Scope_Init(void);
void Scope_SetTimeBase(void);
void Scope_Start(void);
void Scope_RefreshWave(scope_wave_t *adc_buf);

void Scope_AddTimeBase(void);
void Scope_SubTimeBase(void);
void Scope_UpdateHeart(scope_uint8_t nms);

/**
 * @brief Calculate wave width
 * @param wave_buf wave buffer
 * @return the state of calculate wave width, ture: success   false: false 
 */
scope_sta_t Scope_GetWaveWidth(scope_wave_t *wave_buf);

scope_sta_t Scope_GetWaveFre(scope_wave_t *wave_buf);

void Scope_GetWaveVoltage(scope_wave_t *wave_buf);

void Scope_Task(scope_wave_t *wave_buf);
void Scope_Auto(scope_wave_t *wave_buf);

#endif

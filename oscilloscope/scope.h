#ifndef SCOPE_H
#define SCOPE_H

#include "scope_conf.h"
#include "scope_gui.h"

typedef struct scope_s {
    scope_uint8_t time_base;
    scope_sta_t trig_sta;
    scope_uint16_t wave_width;  /* !< */
    scope_uint16_t trig_level;  /* !< tirgger level of wave */
    scope_wave_t wave_value_max;
    scope_wave_t wave_value_min;
    scope_uint32_t wave_fre;    /* !< wave frequence, wave_fre Hz */
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



void Scope_SetTimeBase(void);
void Scope_Start(void);
void Scope_RefreshWave(scope_wave_t *adc_buf);

void Scope_AddTimeBase(void);
void Scope_SubTimeBase(void);

/**
 * @brief Calculate wave width
 * @param wave_buf wave buffer
 * @return the state of calculate wave width, ture: success false: false 
 */
scope_sta_t Scope_GetWaveWidth(scope_wave_t *wave_buf);

scope_sta_t Scope_GetWaveFre(scope_wave_t *wave_buf);

void Scope_GetWaveVoltage(scope_wave_t *wave_buf);

void Scope_Task(scope_wave_t *wave_buf);
void Scope_Auto(scope_wave_t *wave_buf);

#endif

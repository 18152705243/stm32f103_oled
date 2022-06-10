#ifndef SCOPE_H
#define SCOPE_H

#include "scope_conf.h"
#include "scope_gui.h"

typedef struct scope_s {
    scope_uint8_t time_base;
    char *time_base_buf;
}scope_t;

enum Scope_TimeBase {
	TB_Min,
    TB_50s,
    TB_20s,
    TB_10s,
    TB_5s,
    TB_2s,		
	TB_1s,		
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
	TB_10us,	
	TB_5us,		
	TB_2us,		
	TB_1us,
    TB_Max
	// TB_0us5,	
	// TB_0us2,
	// TB_0us1,
	// TB_50ns,
	// TB_20ns,
	// TB_10ns,
};



void Scope_SetTimeBase(void);
void Scope_Start(void);
void Scope_WaveRefresh(scope_uint16_t *adc_buf);
void Scope_ParaRefresh(void);

#endif

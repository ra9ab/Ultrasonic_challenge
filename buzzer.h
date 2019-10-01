#ifndef BUZZER_H
#define BUZZER_H
#include "Timer_Draft.h"
#include "stdtypes.h"

extern sint8 object_detected;

#define MAX_TON 23


void buzzer_on(void);
void buzzer_off(void);
sint8 detect_object(void);
uint32 count_object(sint8);




#endif

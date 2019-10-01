#ifndef BUZZER_H
#define BUZZER_H
#include "timers.h"
#include "std_types.h"

extern sint8 object_detected;

#define MAX_TON 23


void buzzer_on(void);
void buzzer_off(void);
sint8 detect_object(void);
uint32 count_object(sint8);




#endif
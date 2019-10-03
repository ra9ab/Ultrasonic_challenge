/*
 * Icu_software.h
 *
 * Created: 10/1/2019 5:22:32 PM
 *  Author: Ashry
 */ 

#ifndef ICU_SOFTWARE_H_
#define ICU_SOFTWARE_H_
#include "stdtypes.h"

void external_interrupt_init();
void Rising_edge();
void Falling_edge();
void Time_Calc();
extern uint32 Over_Flow_Counts ;
extern uint32 Counts ;
extern uint32 x ;
extern uint32 TON;
extern uint32 Time ;
extern uint8 trigger_flag ;
extern uint32 distance ;

#endif /* ICU_SOFTWARE_H_ */

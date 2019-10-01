/*
 * Icu_software.h
 *
 * Created: 10/1/2019 5:22:32 PM
 *  Author: Ashry
 */ 


#ifndef ICU_SOFTWARE_H_
#define ICU_SOFTWARE_H_


void external_interrupt_init(void);
void Rising_edge(void);
void Falling_edge(void);
float32 Time_Calc(void);

extern uint8 running_flag ;

#endif /* ICU_SOFTWARE_H_ */

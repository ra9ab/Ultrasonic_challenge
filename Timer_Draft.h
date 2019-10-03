/*
 * TIMER.h
 *
 *  Created on: Sep 24, 2019
 *      Author: Mohammed
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "TIMER_cnfg.h"

#ifndef F_CPU
#define  F_CPU 8000000ul
#endif

//------------------- Delay_units Macro_Constants -------------------------------//
#define SECOND		0
#define MILLISECOND 1
#define MICROSECOND 2
//------------------ Another Naming for Delay_units ------------//
#define SEC			0
#define MS			1
#define US			2

/*
//this line is used for testing only 
#define Delay_unit_t uint8	
*/
typedef enum {s=0,ms=1,us=2,sec=0,msec=1,usec=2}Delay_unit_t;
typedef enum {AK,NAK} ACK;

//-------------------  Public_Functions-------------------------------//
ACK TIMER_init(void);
ACK Time_Delay ( TIMER_t  , double  , Delay_unit_t );
ACK TIMER_Start ( TIMER_t  );
ACK PWM_generate( TIMER_t  , float  ,uint8 );
ACK interrupt_time_delay (TIMER_t );
ACK Update_Timer_TCNT_Register (TIMER_t , double  , Delay_unit_t );
ACK TIMER_Stop (TIMER_t );
ACK Enable_Timer_Interrupt (TIMER_t );
void timer2_delay_ms(uint32 delay_value);



uint8 PWM_Value;

#endif /* TIMER_H_ */

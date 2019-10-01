/*
 * Icu_software.c
 *
 * Created: 10/1/2019 1:13:28 PM
 *  Author: Ashry
 */ 
#include "macros.h"
#include "stdtypes.h"
#include "registers.h"
#include <avr/interrupt.h>
#include "Trigger.h"
uint8 volatile Flag_edge=0;
uint16 volatile Counts=0;
uint8 running_flag =0;


void Rising_edge(){
	REG_SREG|=(1<<I_BIT);
    REG_GICR|=(1<<INT2_BIT);	
    REG_MCUCSR|=(1<<6);
	Flag_edge=0;
}

void Falling_edge(){	
REG_SREG|=(1<<I_BIT);
REG_GICR|=(1<<INT2_BIT);
REG_MCUCSR &=~ (1<<6);
}


void external_interrupt_init()
{
	Rising_edge();

}

ISR(INT2_vect){
	if(Flag_edge==0){
	  TCCR1B|=0b00000010;
	  TCNT1=0;
	  Flag_edge =1;
	  Falling_edge();	
		
	}
	else if(Flag_edge==1){
		Counts=TCNT1;
		Flag_edge=0;
		Rising_edge();
		running_flag=1;
		Trigger_Pulse ();
	}
}

float32 Time_Calc(){
	float x=0;
	x=Counts/1000;
	return x;
}

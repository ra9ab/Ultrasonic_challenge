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
#include <avr/io.h>
uint8 volatile Flag_edge=0;
uint16 volatile Counts=0;
uint16 volatile Over_Flow_Counts=0;
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
REG_MCUCSR&=~(1<<6);	
}
void external_interrupt_init()
{
	DDRB &= ~(1<<4);
	Rising_edge();

}

ISR(INT2_vect){
	if(Flag_edge==0){
	  TCCR0|=0b00000010;
	  TCNT0=0;
	  Flag_edge++;
	  Falling_edge();	
		
	}
	else if(Flag_edge==1){
		Counts=TCNT0;
		Flag_edge=0;
		running_flag=1;
		Rising_edge();
	}
	
	
	
	
	
}
ISR(TIMER0_OVF_vect){
	Over_Flow_Counts++;
}

float Time_Calc(){
	float x=0;
	x=(Over_Flow_Counts*256+Counts)*1000;
	Over_Flow_Counts=0;
	Counts=0;
	return x;
}
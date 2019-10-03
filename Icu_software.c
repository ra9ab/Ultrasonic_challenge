/*
 * Icu_software.c
 *
 * Created: 10/1/2019 1:13:28 PM
 *  Author: Ashry
 */ 
#include "macros.h"
#include "stdtypes.h"
#include "registers.h"
#include <avr/io.h>
#include "LCD.h"
#include <avr/interrupt.h>
uint8  Flag_edge=0;
uint32  Counts=0;
uint32  Over_Flow_Counts=0;
uint32 TON=0;
uint32 Time ;
uint32 Trigger_Counter=5;
uint8  trigger_flag =0;
uint32 distance =0 ;



void Rising_edge()
{REG_GICR &=~(1<<INT2_BIT);
   // REG_GICR|=(1<<INT2_BIT);
    REG_MCUCSR|=(1<<6);
	Flag_edge=0;
	REG_GICR|=(1<<INT2_BIT);

}
void Falling_edge()
{	REG_GICR &=~(1<<INT2_BIT);
REG_MCUCSR&=~(1<<6);
REG_GICR|=(1<<INT2_BIT);
}
void external_interrupt_init()
{
	DDRB &= ~(1<<3);
    REG_GICR|=(1<<INT2_BIT);
	Rising_edge();

}

ISR(INT2_vect)
{	REG_SREG &=~(1<<I_BIT);
	if(Flag_edge==0) /*if rising edge*/
	{
	  TCCR0|=0b00000101;	//Start Timer
	  TCNT0=0;				// Clear the TCNT Timer Register
	  Flag_edge=1;
	  PORTC |= (1<<PC0);
	  Falling_edge();
	  trigger_flag=0;
		
	}
	else if(Flag_edge==1)
	{
		Counts=TCNT0;
		PORTC &= ~(1<<PC0);
		Flag_edge=0;
		Time_Calc();
		Rising_edge();
		trigger_flag=1;
	}



	REG_SREG |=(1<<I_BIT);


}
ISR(TIMER0_OVF_vect)
{ 	REG_SREG &=~(1<<I_BIT);
	Over_Flow_Counts++;
	PORTC ^= (1<<PC2);
	REG_SREG |=(1<<I_BIT);

}

void Time_Calc(){
	TON=Counts;
	Time = TON*128;
	distance = Time/58;
	Over_Flow_Counts=0;
	Counts=0;
}

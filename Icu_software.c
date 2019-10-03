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
#define  FAllING 1
#define  RISING 0

uint16 volatile Counts=0;
uint16 volatile Over_Flow_Counts=0;
uint8 running_flag =0;
float32 Distance=0;
void (*G_EXT_ptr)(void);


void Set_EXT_inT(void (*ptr)(void))
{
	
	G_EXT_ptr=ptr;
}	

void Rising_edge()
{
	REG_SREG|=(1<<I_BIT);
    REG_GICR|=(1<<INT2_BIT);	
    REG_MCUCSR|=(1<<6);	
	
}

void Falling_edge()
{	
REG_SREG|=(1<<I_BIT);
REG_GICR|=(1<<INT2_BIT);
REG_MCUCSR&=~(1<<6);	
}

ISR(INT2_vect)
{
	
G_EXT_ptr();	
	
}

ISR(TIMER0_OVF_vect)
{
	Over_Flow_Counts++;
}

float Time_Calc()
{
	float TON=0;
	TON=(Over_Flow_Counts*256+Counts)*(0.128);		//we used prescaler 1024 and F_CPU 8MHz
	Over_Flow_Counts=0;
	Counts=0;
	return TON;
}

void Calculate_Distance (float32 TON)
{
	
	Distance = 	(TON * 17 );
}

void Ex_intr_handdling(void)
{
	float TON;
	static uint8 Flag_edge=0;
	if(Flag_edge==RISING)
	{
	TCCR0|=0b00000101;
	TCNT0=0;
	Flag_edge++;
	Falling_edge();
	
	}
	
	else if(Flag_edge==FAllING)
	{
	Counts=TCNT0;
	Flag_edge=0;
	TON=Time_Calc();
	Calculate_Distance(TON);
	
	Rising_edge();
	}
		
}


void external_interrupt_init()
{
	DDRB &= ~(1<<2);
	Rising_edge();
	Set_EXT_inT(Ex_intr_handdling);

}

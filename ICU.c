/*
 * ICU.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: COMPUTER SHOW
 */
#include"ICU.h"

static volatile void (*g_ptr) (void) = NULL_PTR;

void __vector_6 (void)
{
	if( g_ptr != NULL_PTR)
	{
		(*g_ptr)();
	}
}
void ICU_setCallBack(void (*a_ptr) (void))
{
	a_ptr = g_ptr;
}
void Icu_init(void)
{
	REG_DDRD &= ~(1<<6);
	REG_TCNT1L = 0;
	REG_TCNT1H = 0;
	REG_TCCR1A = ((1<<FOC1A)|(1<<FOC1B));
	REG_TCCR1B = (REG_TCCR1B&0xBF)|(icu_config.edge<<ICES1);
	REG_TCCR1B = (REG_TCCR1B&0xF8) |((icu_config.clock)&0x07);
	REG_ICR1L = 0;
	REG_ICR1H = 0;
	REG_TIMSK |= (1<<TICIE1);
}
void Icu_setEdgeDetectionType(const ICU_EDGE edgeType)
{
	REG_TCCR1B = (REG_TCCR1B&0xBF)|(edgeType<<ICES1);
}
uint16 Icu_getInputCaptureValue(void)
{
	uint16 retval = 0;
	retval = (REG_ICR1H&0x0F)|((REG_ICR1L&0x0F)<<8);
	return retval;
}
void Icu_clearTimerValue(void)
{
	REG_TCNT1L = 0;
	REG_TCNT1H = 0;
}
void Icu_DeInit(void)
{
	REG_TCNT1L = 0;
	REG_TCNT1H = 0;
	REG_ICR1H = 0;
	REG_ICR1L = 0;
	REG_TCCR1A = 0;
	REG_TCCR1B = 0;
	REG_TIMSK &= ~ (1<<TICIE1);
}



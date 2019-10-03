/*
 * test.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: COMPUTER SHOW
 */

#include"ICU.h"

uint8 g_count = 0;
uint16 first_calc = 0;
uint16 second_clac = 0;
uint16 third_calc = 0;
uint16 fourth_calc = 0;
uint16 t_period = 0;
float t_dutyCycle = 0.0;

void claculate_TimeStamp (void)
{
	g_count++;
	if(g_count == 1)
	{
		Icu_clearTimerValue();
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_count == 2)
	{
		first_calc = Icu_getInputCaptureValue();
		Icu_setEdgeDetectionType(RISING);
	}
	else if(g_count == 3)
	{
		second_clac = Icu_getInputCaptureValue();
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_count == 4)
	{	g_count = 0;
		Icu_DeInit();
		Icu_clearTimerValue();
		third_calc = Icu_getInputCaptureValue();
		Icu_setEdgeDetectionType(RISING);
	}
}

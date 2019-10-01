#include <stdio.h>
#include "application.h"
#include "DIO.h"
#include "LCD.h"
#include "Trigger.h"
#include "Icu_software.h"
#include "Distance.h"
#include "application.h"

int main()
{	float32 TON , distance   ;
	LCD_INIT_4bit();
	external_interrupt_init();
	Trigger_int ();
	Trigger_Pulse ();
	while(1!=running_flag);
	running_flag=0;

	while(1)
	{
		TON = Time_Calc();
		distance = Calculate_Distance (TON);
		distance_alarm(distance);
		sint8 detect_object_result = detect_object();
		uint32 counter_dectected_object = count_object(detect_object_result);
		if (detect_object_result == 1)
		{
			led_on(LED0);
		}
		else
		{
			led_off(LED0);
		}

	}
}

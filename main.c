#include <stdio.h>

int main()
{
	LCD_INIT_4bit();
	//trigger function from ehab
	while(1)
	{
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
		distance_alarm(distance);
	}
}
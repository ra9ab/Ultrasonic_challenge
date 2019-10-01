#include "application.h"


sint8 detect_object()
{
	uint32 result = Time_Calc();
	if(result == MAX_TON)
	{
		return object_detected = 0; // object not detected
	}
	else if (result < MAX_TON)
	{
		return object_detected = 1; // object detected. As the distance is greater than 0 && TON is less than 40 milliseconds
	}
	else 
	{
		return object_detected = -1;
	}
}

uint32 count_object(object_detected)
{
	static sint8 is_object_detected = 10;
	static uint32 detect_counter = 0;
	if (is_object_detected == 10)
	{
		is_object_detected = object_detected;
	}
	if (is_object_detected == object_detected)
	{
		// Do nothing
	}
	else 
	{
		detect_counter++;
		is_object_detected = object_detected;
	}
	return detect_counter;
}

void led_on(uint8 led_no)
{
	switch (led_no)
	{
		case LED0:
		DIO_set_pin_direction(PORT_B, PIN4,OUTPUT);
		DIO_write_pin_value(PORT_B, PIN4,HIGH);
		break;
		
		case LED1:
		DIO_set_pin_direction(PORT_B, PIN5,OUTPUT);
		DIO_write_pin_value(PORT_B, PIN5,HIGH);
		break;
		
		case LED2:
		DIO_set_pin_direction(PORT_B, PIN6,OUTPUT);
		DIO_write_pin_value(PORT_B, PIN6,HIGH);
		break;
		
		case LED3:
		DIO_set_pin_direction(PORT_B, PIN7,OUTPUT);
		DIO_write_pin_value(PORT_B, PIN7,HIGH);
		break;
		
		default:
		break;
	}
}

void led_off(uint8 led_no)
{
	switch (led_no)
	{
		case LED0:
		DIO_set_pin_direction(PORT_B, PIN4,OUTPUT);
		DIO_write_pin_value(PORT_B, PIN4,LOW);
		break;
		
		case LED1:
		DIO_set_pin_direction(PORT_B, PIN5,OUTPUT);
		DIO_write_pin_value(PORT_B, PIN5,LOW);
		break;
		
		case LED2:
		DIO_set_pin_direction(PORT_B, PIN6,OUTPUT);
		DIO_write_pin_value(PORT_B, PIN6,LOW);
		break;
		
		case LED3:
		DIO_set_pin_direction(PORT_B, PIN7,OUTPUT);
		DIO_write_pin_value(PORT_B, PIN7,LOW);
		break;
		
		default:
		break;
	}
}

void distance_alarm(f32 distance)
{
	uint32 delay_time = (uint32) distance;
	if (distance == MAX_DISTANCE)
	{
		buzzer_on();
		// delay time from timer driver (delay_time * MAX_DELAY_FACTOR);
		buzzer_off();
		//delay time from timer driver (delay_time * MAX_DELAY_FACTOR);
	}
	else if (distance <= MIN_DISTANCE)
	{
		buzzer_on();
		// delay time from timer driver ((delay_time * MIN_DELAY_FACTOR) - delay_time);
		buzzer_off();
		//delay time from timer driver ((delay_time * MIN_DELAY_FACTOR) - delay_time);
	}
	else if ((distance < MAX_DISTANCE)&&(distance > MIN_DISTANCE))
	{
		buzzer_on();
		// delay time from timer driver ((delay_time * AVG_DELAY_FACTOR) - delay_time);
		buzzer_off();
		//delay time from timer driver ((delay_time * AVG_DELAY_FACTOR) - delay_time);
	}
}
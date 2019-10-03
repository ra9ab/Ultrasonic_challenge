#include <avr/io.h>
#include "application.h"
#include "DIO.h"
#include "LCD.h"
#include "Trigger.h"
#include "Icu_software.h"
#include "Distance.h"
#include "macros.h"
#include "avr/delay.h"
#include "buzzer.h"
#define REQ_NUM 1



int main()
{

	TIMER_init();
	DIO_init();
	LCD_init();
	buzzer_init();
	external_interrupt_init();
	PWM_generate(TIMER1,70,NON_INVERTING);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Distance");
	Trigger_Pulse();


	while(1)
	{

#if (REQ_NUM==1)

		LCD_goToRowColumn(1,1);
				LCD_intgerToString(distance);
				while(trigger_flag == 0);
				_delay_ms(500);
				LCD_clearScreen();

				Trigger_Pulse();



#elif (REQ_NUM==2)
				//ICU_REQ
				float TON = 1000;
						float distance = Calculate_Distance (TON);
						distance_alarm(distance);
						sint8 detect_object_result = 1; // detect_object();
						LCD_goToRowColumn(0,0);
						LCD_intgerToString(detect_object_result);
						_delay_ms(1000);
						uint32 counter_dectected_object = count_object(detect_object_result);
						// We need to print this on the LCD to display counted objects.
						LCD_goToRowColumn(0,7);
						LCD_intgerToString(counter_dectected_object);
						_delay_ms(1000);
						if (detect_object_result == 1)
						{
							led_on(LED0);
						}
						else
						{
							led_off(LED0);
						}

#elif (REQ_NUM==3)
						// REQ.1
						LCD_clearScreen();
						float TON = 1000; //Time_Calc();
						distance = Calculate_Distance (TON);

						//call function move_forward(DC = 20, period);
						motor_moveForward(80);
						// call printlcd(speed);
						uint8 speed = calculate_speed(20);
						LCD_goToRowColumn(0,3);
						LCD_intgerToString(speed);
						_delay_ms(2000);
						LCD_clearScreen();

						//call function move_forward(DC = 40, period);
						motor_moveForward(60);
						speed = calculate_speed(40);
						// call printlcd(speed);
						LCD_goToRowColumn(0,3);
						LCD_intgerToString(speed);
						_delay_ms(2000);
						LCD_clearScreen();

						//call function move_forward(DC = 80, period);
						motor_moveForward(20);
						speed = calculate_speed(80);
						// call printlcd(speed);
						LCD_goToRowColumn(0,3);
						LCD_intgerToString(speed);
						_delay_ms(2000);
						LCD_clearScreen();
						//call function move_forward(DC = 100, period);
						motor_moveForward(0);
						speed = calculate_speed(100);
						// call printlcd(speed);
						LCD_goToRowColumn(0,3);
						LCD_intgerToString(speed);
						_delay_ms(2000);
						LCD_clearScreen();
						//call function move_forward(DC = 80, period);
						motor_moveForward(20);
						speed = calculate_speed(80);
						// call printlcd(speed);
						LCD_goToRowColumn(0,3);
						LCD_intgerToString(speed);
						_delay_ms(2000);
						LCD_clearScreen();
						//call function move_forward(DC = 40, period);
						motor_moveForward(60);
						speed = calculate_speed(40);
						// call printlcd(speed);
						LCD_goToRowColumn(0,3);
						LCD_intgerToString(speed);
						_delay_ms(2000);
						LCD_clearScreen();
						//call function move_forward(DC = 20, period);
						motor_moveForward(80);
						speed = calculate_speed(20);
						// call printlcd(speed);
						LCD_goToRowColumn(0,3);
						LCD_intgerToString(speed);
						_delay_ms(2000);
						LCD_clearScreen();
						// Call function stop_function (period);
						motor_Stop();


#elif (REQ_NUM==4)

						//# REQ.2
				sint8 detect_object_result = 1; // detect_object();
				float TON = 1000; //Time_Calc();
				distance = Calculate_Distance (TON);
				if (detect_object_result == 0) // NO object detected
				{
					motor_moveForward(40);
				}
				else if (detect_object_result == 1)
				{
					if(distance < 20)
					{
						motor_moveBackward(40);
					}

					else if(distance > 20)
					{
						motor_moveForward(40);
					}

					else if(distance == 20)
					{
 						motor_Stop();
					}
				}
#else
				//# REQ.3
				sint8 detect_object_result = 1; //detect_object();
				float TON = 1000; //Time_Calc();
				distance = Calculate_Distance (TON);
				if (detect_object_result == 0) // NO object detected
				{
					move_forward(60);
				}
				else if (detect_object_result == 1)
				{
					if (distance == 40)
					{
						move_forward(30);
					}
					else if (distance < 20)
					{
						move_backward(60);

					}

					else if (distance > 20)
					{
						move_backward(60);

					}
					else if (distance == 20)
					{
						motor_Stop();
						motor_Rightmove(60);
					}
				}


#endif



}
}

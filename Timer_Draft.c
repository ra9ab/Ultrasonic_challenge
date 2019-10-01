/*
 * TIMER.c
 *
 *  Created on: Sep 24, 2019
 *      Author: Hoba
 */

#include "TIMER_Draft.h"
#include "DIO.h"
#include <avr/io.h>


//-------------------  Private_Global_Varibles -------------------------------//
static uint16 Prescalar_Factor[]={1,8,64,256,1024,0,0,32,128};
static uint8 Flag_mode[] = {NA,NA,NA} ;			//this flag is used to check PWM_mode or non_PWM_mode
static uint8 Controlling_Flag =0;				//this flag is used to synchronize between time_delay function and Timer_update_TCNT_Function
static double OCR_Value[] = {0,0,0};			//this array of OCR_value to retain the last value in OCR of each Timer to avoid overwriting on OCR value in each Timer
static uint16 Preloaded_Value[]={0,0,0};
	
//-------------------  Private_Function --------------------------------------//
static ACK Polling_Delay (TIMER_t );
//static ACK interrupt_PWM ( TIMER_t );
static void Calculate_OCR_Value (TIMER_t , double  , Delay_unit_t );
static ACK Update_Timer_OCR_Register (TIMER_t TIMER_Select);


//-------------------  Public_Global_Varibles -------------------------------//
uint16 OVF_Counter_Loop=1 ;
uint8 Running_Flag[MAX_NUM_OF_TIMERS]={0,0,0};		//this Flag is used when your Delay occured by using Interrupt so as to run your operation which needs that delay


/**************************** TIMER DRIVER ****************************/


ACK TIMER_init(void)
{

	ACK STATE = AK;
	uint8 loop_index = 0;
	if (NUM_OF_TIMERS > MAX_NUM_OF_TIMERS)
	{
		STATE = NAK;
	}

	else
	{
		for (loop_index = 0; loop_index < NUM_OF_TIMERS; loop_index++)
		{

			if(TIMER_cnfg_arr[loop_index].IS_init == NOT_INITIALIZED)
			{
				continue;
			}

			TIMER_cnfg_arr[loop_index].IS_init = INITIALIZED;

			switch (TIMER_cnfg_arr[loop_index].timer)
		{
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
						/**************************** START OF TIMER0 ****************************/
		case TIMER0:
		{
			REG_TCNT0 = 0; //timer initial value
			/**************************** WGM MODE TIMER0 ****************************/
			switch (TIMER_cnfg_arr[loop_index].WGM_mode)
			{
				case NORMAL_MODE:
				{
					Flag_mode[loop_index]= NON_PWM_MODE;
					/*********** NORMAL_MODE TIMER0 ********/
					REG_TCCR0 &=  ~ ( (1u<<WGM01) | (1u<<WGM00) );	// NORMAL_MODE WGM01=0 & WGM00=0
					//REG_TCCR0 = (1<<FOC0); //Non PWM mode
					// END OF NORMAL_MODE
					break;
				}

				case CTC_MODE:
				{
					Flag_mode[loop_index]=NON_PWM_MODE;
					/*********** CTC MODE *********/
					REG_TCCR0 |= (1u<<WGM01) ; // CTC WGM01=1
					REG_TCCR0 &= ( ~ (1u<<WGM00) );  // CTC  WGM00=0
					//REG_TCCR0 = (1<<FOC0); //Non PWM mode	 Note:- //this Configurable bit cause an logical error in Toggle mode
					// END OF CTC_MODE
					break;
				}

				case FAST_PWM_MODE:
				{
					Flag_mode[loop_index] = PWM_MODE;
					/*********** FAST PWM MODE *********/
					REG_TCCR0 |= ((1u << WGM01) | (1u << WGM00)); // FAST PWM MODE WGM01=1 & WGM00=1
					DDRB |= (1u<<PIN3); // OCO PIN OUTPUT
					// END OF FAST_PWM_MODE
					break;
				}

				case PHASE_CORRECT_MODE:
				{
					Flag_mode[loop_index] = PWM_MODE;
					/*********** PHASE CORRECT MODE *********/
					REG_TCCR0 &= (~(1u << WGM01)); // PHASE CORRECT MODE  WGM01=0
					REG_TCCR0 |= (1u << WGM00);  // PHASE CORRECT MODE  WGM00=1
					DDRB |= (1u<<PIN3);  // OCO PIN OUTPUT
					// END OF FAST_PWM_MODE
					break;
				}

				// INCORRECT INPUT IN  WGM MODE
				default:
				{
					TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
					STATE = NAK;
					break;
				}
			}
			/**************************** END OF WGM MODE ****************************/

			switch(Flag_mode[loop_index])
			{
				case NON_PWM_MODE:
				{
					switch (TIMER_cnfg_arr[loop_index].COM_mode)
					{
						/*********** COM_MODE *********/
						// NORMAL_OPERATION_COM_MODE
						case NORMAL_OPERATION:
						{
							REG_TCCR0 &= ~((1u << COM01) | (1u << COM00));// NORMAL COM01=0 & COM00=0
							break;
						}
						// TOGGLE
						case TOGGLE_OPERATION:
						{
							REG_TCCR0 &= (~(1u << COM01)); // TOGGLE COM01=0
							REG_TCCR0 |= (1u << COM00);  // TOGGLE  COM00=1
							break;
						}
						// CLEAR
						case CLEAR_OPERATION:
						{
							REG_TCCR0 |= (1u << COM01); // CLEAR COM01=1
							REG_TCCR0 &= (~(1u << COM00));  // CLEAR  COM00=0
							break;
						}
						// SET
						case SET_OPERATION:
						{
							REG_TCCR0 |= ((1u << COM01) | (1u << COM00)); // SET COM01=1 & COM00=1
							break;
						}
						// INCORRECT INPUT IN  COM MODE
						default:
						{

							TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
							STATE = NAK;
							break;
						}
						// END OF COM_MODE
					}
					// END OF NON_PWM_MODE
					break;
				}

				case PWM_MODE:
				{
					switch (TIMER_cnfg_arr[loop_index].COM_mode)
					{
						// INVERTING
						case INVERTING:
						{
							REG_TCCR0 |= (1u << COM01); // INVERTING COM01=1
							REG_TCCR0 &= (~(1u << COM00));  // INVERTING COM00=0
							break;
						}
						// NON_INVERTING
						case NON_INVERTING:
						{
							REG_TCCR0 |= ((1u << COM01) | (1u << COM00)); // NON_INVERTING COM01=1 & COM00=1
							break;
						}
						// INCORRECT INPUT IN COM MODE
						default:
						{
							TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
							STATE = NAK;
							break;
						}
						// END OF COM_MODE
					}
					// END OF PWM_MODE
					break;
				}
			}

			/**************************** END OF COM MODE ****************************/

			switch (TIMER_cnfg_arr[loop_index].interrupt)
			{
				/*********** INTERRUPT MODE *********/
				// INTERRUPT
				case INTERRUPT:
				{
					SREG |= (1u<< 7 ) ; // ENABLE GLOBAL INTERRUPT
					switch (TIMER_cnfg_arr[loop_index].WGM_mode)
					{
						case NORMAL_MODE:
						{
							REG_TIMSK |= (1u<<TOIE0); //Overflow Interrupt Enable
							break;

						}
						case CTC_MODE:
						{
							REG_TIMSK |= (1u<<OCIE0); // Output Compare Match Interrupt Enable
							break;
						}

						// INCORRECT INPUT IN  WGM MODE
						default:
						{
							TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
							STATE = NAK;
							break;
						}

					}
					// END OF WGM MODE
					break;
					// END OF INTERRUPT
				}

				// NO INTERRUPT
				case NO_INTERRUPT:
				{
					REG_TIMSK &= (~ ( (1u<<OCIE0) |(1u<<TOIE0) ) );  //Overflow Interrupt & Output Compare Match Interrupt disable
					break;
				}
				default:
				{
					TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
					STATE = NAK;
					break;
				}
				/*********** END OF INTERRUPT MODE *********/
			}


			switch (TIMER_cnfg_arr[loop_index].ICU)
			{
				/*********** ICU MODE *********/
				case NA:
				{
					break;
				}
				default:
				{
					TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
					STATE = NAK;
					break;
				}
				/*********** END OF ICU MODE *********/
			}
			break;
			/**************************** END OF TIMER 0 ****************************/
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

			/**************************** START OF TIMER 1 ****************************/
			case TIMER1:
			{
				TCNT1=0 ;	//CLEAR_TIMER1_TCNT_Register
				/**************************** WGM MODE ****************************/
				switch (TIMER_cnfg_arr[loop_index].WGM_mode)
				{
					case NORMAL_MODE:
					{
						Flag_mode[loop_index] = NON_PWM_MODE;
						REG_TCNT1H=0;
						REG_TCNT1L=0;
						//TCNT1=0;
						NORMAL_MODE_TIMER1();
						break;
					}

					case CTC_MODE:
					{

						Flag_mode[loop_index]=NON_PWM_MODE;
						REG_TCNT1H=0;
						REG_TCNT1L=0;
						//TCNT1=0;
						CTC_OCR1A_MODE_TIMER1();
						break;
					}

					case FAST_PWM_MODE:
					{
						Flag_mode[loop_index]=PWM_MODE;
						PWM_PHASE_CORRECT_OCR1A();
						break;
					}

					case PHASE_CORRECT_MODE:
					{
						Flag_mode[loop_index]=PWM_MODE;
						PWM_PHASE_CORRECT_OCR1A();
						break;
					}

					default:
					{
						TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
						STATE=NAK;
						break;
					}
				}
				/**************************** END OF WGM MODE ****************************/

				/**************************** COM MODE ****************************/
				switch(Flag_mode[loop_index])
				{
					/**************************** END OF NON PWM MODE ****************************/
					case NON_PWM_MODE:
					{
						switch (TIMER_cnfg_arr[loop_index].COM_mode)
						{
							case NORMAL_OPERATION:
							{
								COM_1A_NORMAL();
								break;
							}

							case TOGGLE_OPERATION:
							{

								COM_1A_TOGGLE();
								break;
							}

							case CLEAR_OPERATION:
							{
								COM_1A_CLEAR();
								break;
							}

							case SET_OPERATION:
							{
								COM_1A_SET;
								break;
							}



							default:
							{

								TIMER_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
								STATE=NAK;
								break;
							}

						}
						/**************************** END OF NON PWM MODE ****************************/
						break;
					}

					/**************************** PWM MODE ****************************/
					case PWM_MODE:
					{
						switch (TIMER_cnfg_arr[loop_index].COM_mode)
						{
							case INVERTING:
							{
								COM_1A_PWM_INVERTED();
								break;
							}

							case NON_INVERTING:
							{
								COM_1A_PWM_NON_INVERTED();
								break;
							}

							default:
							{
								TIMER_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
								STATE=NAK;
								break;
							}

						}

						break;
					}

					default:
					{
						TIMER_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
						STATE=NAK;
						break;
					}
				}
				/**************************** END OF PWM MODE ****************************/


				/**************************** INTERRUPT MODE ****************************/
				switch (TIMER_cnfg_arr[loop_index].interrupt)
				{

					case INTERRUPT:
					{
						ENABLE_GLOBAL_INTERRUPT;      //Enable_Global_Interrupt
						switch(TIMER_cnfg_arr[loop_index].WGM_mode)		//check what's WGM
						{
							case NORMAL:
							{
								ENABLE_TOIE1;
								break;
							}

							case CTC_MODE:
							{
								ENABLE_OCIE1A;
								break;
							}

							default:
							{
								TIMER_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
								STATE=NAK;
								break;
							}
						}
						break;
					}

					case NO_INTERRUPT:
					{
						REG_TIMSK &= ~(1<<TOIE1) ;
						REG_TIMSK &= ~(1<<TICIE1) ;
						REG_TIMSK &= ~(1<<TOV1) ;
						break;
					}

					default:
					{
						TIMER_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
						STATE=NAK;
						break;
					}
					break;
				}

				/*********** END OF INTERRUPT MODE *********/

				/*********** ICU MODE *********/
				switch (TIMER_cnfg_arr[loop_index].ICU)
				{
					case ICU_USED:
					{
						if(TIMER_cnfg_arr[loop_index].interrupt == 1 )
						{
							ENABLE_TICIE1;
						}

						break;
					}

					case NO_ICU_USED:
					{
						break;
					}

					default:
					{
						TIMER_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
						STATE=NAK;
						break;
					}
					/*********** END OF ICU MODE *********/
				}
				break;
				/**************************** END OF TIMER 1 ****************************/
			}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

			/**************************** START OF TIMER 2 ****************************/
		case TIMER2:
			{

			REG_TCNT2 = 0; //timer initial value
			switch (TIMER_cnfg_arr[loop_index].WGM_mode)
			{

				/**************************** WGM MODE ****************************/

				case NORMAL_MODE:
				{
					Flag_mode[loop_index] = NON_PWM_MODE;
					/*********** NORMAL_MODE *********/
					REG_TCCR2 &=  ~ ( (1u<<WGM21) | (1u<<WGM20) );	// NORMAL_MODE WGM21=0 & WGM20=0
					REG_TCCR2 = (1<<FOC2); //Non PWM mode
					// END OF NORMAL_MODE
				}


				case CTC_MODE:
				{

					Flag_mode[loop_index] = NON_PWM_MODE;
					/*********** CTC MODE *********/
					REG_TCCR2 |= (1u<<WGM21) ; // CTC WGM21=1
					REG_TCCR2 &= ( ~ (1u<<WGM20) );  // CTC  WGM20=0
					//REG_TCCR2 = (1<<FOC2); //Non PWM mode
					// END OF CTC_MODE
					break;
				}

				case FAST_PWM_MODE:
				{
					Flag_mode[loop_index] = PWM_MODE;
					/*********** FAST PWM MODE *********/
					REG_TCCR2 |= ((1u << WGM21) | (1u << WGM20)); // FAST PWM MODE WGM21=1 & WGM20=1
					DDRD |= (1u<<PIN7); // OC2 PIN OUTPUT
				break;
				}

				case PHASE_CORRECT_MODE:
				{
					Flag_mode[loop_index] = PWM_MODE;
					/*********** PHASE CORRECT MODE *********/
					REG_TCCR0 &= (~(1u << WGM21)); // PHASE CORRECT MODE  WGM21=0
					REG_TCCR0 |= (1u << WGM20);  // PHASE CORRECT MODE  WGM20=1
					DDRB |= (1u<<PIN7);  // OC2 PIN OUTPUT
					// END OF FAST_PWM_MODE
					break;
				}

				default:
				{
					TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
					STATE=NAK;
					break;
				}

				//missing case Phase_Correct_PWM_Mode

				/**************************** END OF WGM MODE ****************************/
			}

				switch(Flag_mode[TIMER2])
				{
					case NON_PWM_MODE:
					{
						switch (TIMER_cnfg_arr[loop_index].COM_mode)
						{
							/*********** COM_MODE *********/

							// NORMAL_OPERATION_COM_MODE
							case NORMAL_OPERATION:
							{
								REG_TCCR2 &=  ~ ( (1u<<COM21) | (1u<<COM20) );	// NORMAL COM21=0 & COM20=0
								break;
							}

							// TOGGLE
							case TOGGLE_OPERATION:
							{
								REG_TCCR2 &= ( ~(1u<<COM21) ); // TOGGLE COM21=0
								REG_TCCR2 |=  (1u<<COM20);  // TOGGLE  COM20=1
								break;
							}
							// CLEAR
							case CLEAR_OPERATION:
							{
								REG_TCCR2 |= (1u<<COM21) ; // CLEAR COM21=1
								REG_TCCR2 &= ( ~ (1u<<COM20) );  // CLEAR  COM20=0
								break;
							}
							// SET
							case SET_OPERATION:
							{
								REG_TCCR2 |= ( (1u<<COM21) | (1u<<COM20) ) ; // SET COM21=1 & COM20=1
								break;
							}
							// INCORRECT INPUT IN  COM MODE
							default: {

								TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
								STATE = NAK;
								break;
							}
							// END OF COM_MODE
						}

						break;
					}

					case PWM_MODE:
					{
						switch (TIMER_cnfg_arr[loop_index].COM_mode)
						{

							/*********** COM_MODE *********/

							// INVERTING
							case INVERTING:
							{
								REG_TCCR2 |= (1u << COM21); // INVERTING COM21=1
								REG_TCCR2 &= (~(1u << COM20));  // INVERTING COM20=0
								break;
							}
							// NON_INVERTING
							case SET:
							{
								REG_TCCR2 |= ((1u << COM21) | (1u << COM20)); // NON_INVERTING COM21=1 & COM20=1
								break;
							}
							// INCORRECT INPUT IN COM MODE
							default:
							{
								TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
								STATE = NAK;
								break;
							}
							// END OF COM_MODE
						}

						break;
					}

					default:
					{
						TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
						STATE = NAK;
						break;
					}
				}



				switch (TIMER_cnfg_arr[loop_index].interrupt)
				{

				/*********** INTERRUPT MODE *********/
				// INTERRUPT
				case INTERRUPT:
				{
					SREG |= (1u<< 7 ) ; // ENABLE GLOBAL INTERRUPT
					switch (TIMER_cnfg_arr[loop_index].WGM_mode) {

					case NORMAL_MODE:
					{
						REG_TIMSK |= (1u<<TOIE2); //Overflow Interrupt Enable
						break;

					}
					case CTC_MODE:
					{
						REG_TIMSK |= (1u<<OCIE2); // Output Compare Match Interrupt Enable
						break;
					}

					// INCORRECT INPUT IN  WGM MODE
					default:
					{
						TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
						STATE = NAK;
						break;
					}

					}
					// END OF WGM MODE
					break;
					// END OF INTERRUPT
				}

				// NO INTERRUPT
				case NO_INTERRUPT:
				 {
					REG_TIMSK &= (~ ( (1u<<OCIE2) |(1u<<TOIE2) ) );  //Overflow Interrupt & Output Compare Match Interrupt disable
					break;
				 }

				default:
				{

					TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
					STATE = NAK;
					break;
				}


				/*********** END OF INTERRUPT MODE *********/
				}


				switch (TIMER_cnfg_arr[loop_index].ICU)
				{
				/*********** ICU MODE *********/
				case NA:
					break;
				default:
				{
					TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
					STATE = NAK;
					break;
				}
				/*********** END OF ICU MODE *********/
				}

				break;

				/**************************** END OF TIMER 2 ****************************/
			}

			// INCORRECT INPUT IN TIMER
			default:
			{
				TIMER_cnfg_arr[loop_index].IS_init = NOT_INITIALIZED;
				STATE = NAK;
				break;
			}

			/**************************** END OF SWITCH TIMERS  ****************************/
			}

			/**************************** END OF TIMER_cnfg_arr  ****************************/
}

	//END OF ELSE
	}

return STATE;
}


ACK Time_Delay ( TIMER_t TIMER_Select , double Required_Delay , Delay_unit_t Delay_unit )
{
    ACK STATE =AK;

	if (TIMER_cnfg_arr[TIMER_Select].IS_init == INITIALIZED)
	{
		PORTA |= (1<<PA0);
			Calculate_OCR_Value (TIMER_Select,  Required_Delay , Delay_unit);
			Controlling_Flag=1;
	}

	else
	{
		STATE = NAK;
		return STATE;
	}

//--------------------------------------------------------------------------------------------------------------------------//

	if( ( (TIMER_cnfg_arr[TIMER_Select].timer == TIMER0) || (TIMER_cnfg_arr[TIMER_Select].timer == TIMER2) )  )
	{
		if( ( (OCR_Value[TIMER_Select]) > 255 ) )
		{
			TIMER_cnfg_arr[TIMER_Select].WGM_mode = NORMAL_MODE;		//Force the WGM_mode to be configured as NORMAL_MODE

			OVF_Counter_Loop = (uint16) OCR_Value[TIMER_Select]/255;

			// we can use the interrupt on Overflow Flag by the number of OVF_Counter_loop till is finished
			if(TIMER_cnfg_arr[TIMER_Select].interrupt == INTERRUPT)
			{
                STATE=Enable_Timer_Interrupt(TIMER_Select);
				STATE = TIMER_Start(TIMER_Select );

			}

			// we can use the polling on Overflow Flag till the OVF_Counter_loop is finished
			else if(TIMER_cnfg_arr[TIMER_Select].interrupt == NO_INTERRUPT)
			{
				STATE = TIMER_Start(TIMER_Select );
				STATE = Polling_Delay (TIMER_Select );
			}

			else
			{
				STATE =NAK;
			}

			return STATE ;
		}

	  }


	else if ( (TIMER_cnfg_arr[TIMER_Select].timer == TIMER1)  && (OCR_Value[TIMER_Select]> 65535) )
	{
			TIMER_cnfg_arr[TIMER_Select].WGM_mode = NORMAL_MODE;		//Force the WGM_mode to be configured as NORMAL_MODE

			OVF_Counter_Loop = (uint16) OCR_Value[TIMER_Select]/65535;

			// we can use the interrupt on Overflow Flag by the number of OVF_Counter_loop till is finished
			if(TIMER_cnfg_arr[TIMER_Select].interrupt == INTERRUPT)
			{
                STATE=Enable_Timer_Interrupt(TIMER_Select);
				STATE = TIMER_Start(TIMER_Select );
			}

			// we can use the Poll on Overflow Flag by the number of OVF_Counter_loop till is finished
			else if(TIMER_cnfg_arr[TIMER_Select].interrupt == NO_INTERRUPT)
			{
				STATE = TIMER_Start(TIMER_Select );
				STATE = Polling_Delay (TIMER_Select );
			}

			else
			{
				STATE=NAK;
			}

			return STATE ;
	}

//----------------------- OCR_value doesn't pass the Top_Value of the Timer -----------------------------//

	if ((TIMER_cnfg_arr[TIMER_Select].WGM_mode == CTC_MODE))
	{
		STATE= Update_Timer_OCR_Register(TIMER_Select);
	}


	else if ( (TIMER_cnfg_arr[TIMER_Select].WGM_mode == NORMAL_MODE) )

	{
		PORTA |= (1<<PA3);
		STATE= Update_Timer_TCNT_Register(TIMER_Select,Required_Delay,Delay_unit);
	}

    else
    {
            // Should not be here
        STATE=NAK;
        return STATE;
    }

	STATE = TIMER_Start(TIMER_Select );

	if( (TIMER_cnfg_arr[TIMER_Select].COM_mode == NORMAL_OPERATION) )
	{

		STATE = Polling_Delay (TIMER_Select);
	}

    return STATE;
}


void Calculate_OCR_Value (TIMER_t TIMER_Select, double Required_Delay , Delay_unit_t Delay_unit)	//Private Function
{
	double Frequency_of_Timer[]={0,0,0};

	Frequency_of_Timer[TIMER_Select] =   (F_CPU / Prescalar_Factor[TIMER_cnfg_arr[TIMER_Select].prescalar]) ;
	OCR_Value[TIMER_Select] =  Required_Delay * Frequency_of_Timer[TIMER_Select] ;
	
	PORTA |= (1<<PA1);
	//PORTC = OCR_Value[TIMER_Select] ;
	
	switch (Delay_unit)
	{
		case SECOND:
		{
			//OCR_Value[TIMER_Select];
			break;
		}

		case MILLISECOND:
		{
			
			OCR_Value[TIMER_Select] = OCR_Value[TIMER_Select]/1000;
			break;
		}

		case MICROSECOND:
		{

			//PORTC = (uint8)(Frequency_of_Timer[TIMER_Select]/1000000);
			OCR_Value[TIMER_Select] = OCR_Value[TIMER_Select]/1000000;	//six zeros
			//OCR_Value[TIMER_Select] = 10;
			if(OCR_Value[TIMER_Select] == 10)
			{PORTA |= (1<<PA2);}
			PORTC =OCR_Value[TIMER_Select];
			break;
		}

		default:
		{
			break;
		}

	}


}




//this below function can be called solely in interrupt function or another function
ACK Update_Timer_TCNT_Register (TIMER_t TIMER_Select, double Required_Delay , Delay_unit_t Delay_unit )
{
	ACK STATE = AK;

	//-----------This Controlling Flag is used if the function is called from time delay so don't recalculate the OCR_Value again to avoid redundancy else if it's called from another function calculate the OCR_value-----------/
	if(Controlling_Flag == 0)
	{
		Calculate_OCR_Value(TIMER_Select,Required_Delay , Delay_unit);
	}

	if( (TIMER_cnfg_arr[TIMER_Select].WGM_mode == NORMAL_MODE)  && (TIMER_cnfg_arr[TIMER_Select].IS_init == INITIALIZED))
	{
		 switch(TIMER_cnfg_arr[TIMER_Select].timer)
		 {
			 case TIMER0:
			 {
					 PORTA |= (1<<PA6);
				 REG_TCNT0=0;	//CLEAR_TIMER0_TCNT_Register
				 Preloaded_Value[TIMER_Select] = 255 - (OCR_Value[TIMER_Select]);
				 REG_TCNT0 = (uint8) Preloaded_Value[TIMER_Select];	
					PORTD=Preloaded_Value[TIMER_Select];	
				 break;
			 }

			 case TIMER1:
			 {
				 TCNT1 =0;	//CLEAR_TIMER1_TCNT_Register
				 Preloaded_Value[TIMER_Select] = 65535 -  (OCR_Value[TIMER_Select])  ;
				 //REG_TCNT1H =  Preloaded_Value/256;
				 //REG_TCNT1L =  Preloaded_Value%256;
				 TCNT1 = (uint16) Preloaded_Value[TIMER_Select];
	
				 break;
			 }

			 case TIMER2:
			 {
				REG_TCNT2=0;	//CLEAR_TIMER2_TCNT_Register
				 Preloaded_Value[TIMER_Select] = 255 - (OCR_Value[TIMER_Select])  ;
				 REG_TCNT2 = (uint8) Preloaded_Value[TIMER_Select];
				 break;
			 }

			 default:
			 {
				 // Should not be here
				 STATE=NAK;
				 break;
			 }
		 }
	}

	else
	{
		STATE=NAK;
	}

	Controlling_Flag=0;

	return STATE;
}


ACK Polling_Delay (TIMER_t TIMER_Select) //Private Function
 {
	 uint8 i=0;
	 ACK STATE= AK;

	 if ( (TIMER_cnfg_arr[TIMER_Select].WGM_mode == NORMAL_MODE)  && (TIMER_cnfg_arr[TIMER_Select].IS_init == INITIALIZED))
	 {

		 switch(TIMER_Select)
		 {
			 case TIMER0:
			 {
				for(i=0; i<OVF_Counter_Loop;i++)
				{
					 PORTA |= (1<<PA5);
					REG_TCNT0= (uint8) Preloaded_Value[TIMER_Select];
					while ( (REG_TIFR & (1<<TOV0) ) == 0 );
					REG_TIFR |= (1<<TOV0);
				}
				 break;
			 }

			 case TIMER1:
			 {
				 for(i=OVF_Counter_Loop; i>0;i--)
				 {
					 TCNT1= (uint16) Preloaded_Value[TIMER_Select];
					 while ( (REG_TIFR & (1<<TOV1) ) == 0 );
					 { REG_TIFR |= 1<<TOV1 ;}

				 }
				 break;
			 }

			 case TIMER2:
			 {
				 for(i=OVF_Counter_Loop; i>0;i--)
				 {
					 REG_TCNT2= (uint8) Preloaded_Value[TIMER_Select];
					 while ( (REG_TIFR & (1<<TOV2) ) == 0 );
					 { REG_TIFR |= 1<<TOV2 ;}
				 }

				 break;
			 }


			 default:
			 {
				 TIMER_cnfg_arr[TIMER_Select].IS_init = NOT_INITIALIZED;
				 STATE=NAK;
				 break;
			 }

		 }

	 }

	 else if ( (TIMER_cnfg_arr[TIMER_Select].WGM_mode == CTC_MODE)  && (TIMER_cnfg_arr[TIMER_Select].IS_init == INITIALIZED))
	 {
		 switch(TIMER_Select)
		 {
			 case TIMER0:
			 {
				 while( (REG_TIFR & (1<<OCF0)) == 0);
				  REG_TIFR |= 1<<OCF0 ;		//Clear the OCF0 By Writing Logic High (one) in TIRF

				 break;
			 }

			 case TIMER1:
			 {
				 while( (REG_TIFR & (1<<OCF1A)) == 0);
				  REG_TIFR |= 1<<OCF1A ;	//Clear the OCF1A By Writing Logic High (one) in TIRF
				 break;
			 }

			 case TIMER2:
			 {
				 while( (REG_TIFR & (1<<OCF2)) == 0);
				 REG_TIFR |= 1<<OCF2 ;	//Clear the OCF2 By Writing Logic High (one) in TIRF

				 break;
			 }


			 default:
			 {
				 STATE=NAK;
				 break;
			 }
		 }
	 }

	 else
	 {
		 STATE =NAK;
	 }

	
	 	if( (TIMER_cnfg_arr[TIMER_Select].COM_mode == NORMAL_OPERATION) )
	 	{

		 	STATE = Polling_Delay (TIMER_Select);
	 	}
 return STATE;
 }


ACK TIMER_Start ( TIMER_t TIMER_Select )
 {
	 ACK STATE = AK ;

		 if ( (TIMER_cnfg_arr[TIMER_Select].timer == TIMER_Select) && (TIMER_cnfg_arr[TIMER_Select].IS_init==INITIALIZED) )
		 {

			 switch (TIMER_Select)
			 {
				 case TIMER0:
				 {
					 REG_TCCR0 &= (0b11111000);		//TIMER0_CLEAR_PRESCALAR_BITS;

					 switch(TIMER_cnfg_arr[TIMER_Select].prescalar)
					 {
						 case PRESCALER0:
						 {
							 REG_TCCR0 |= (0b00000001);
							 break;
						 }

						 case PRESCALER8:
						 {
							 PORTA |= (1<<PA4);
							 REG_TCCR0 |= (0b00000010);
							 break;
						 }

						 case PRESCALER64:
						 {
							 REG_TCCR0 |= (0b00000011);
							 break;
						 }

						 case PRESCALER256:
						 {
							 REG_TCCR0 |= (0b00000100);
							 break;
						 }

						 case PRESCALER1024:
						 {

							 REG_TCCR0 |= (0b00000101);
							 break;
						 }

						 case EXTERNAL_CLK_RISING:
						 {
							 REG_TCCR0 |= (0b00000110);
							 break;
						 }

						 case EXTERNAL_CLK_FALLING:
						 {
							 REG_TCCR0 |= (0b00000111);
							 break;
						 }

						 default:
						 {
							 TIMER_cnfg_arr[TIMER_Select].IS_init=NOT_INITIALIZED;
							 STATE=NAK;
							 break;
						 }
					 }

					 break;
				 }



				 case TIMER1:
				 {

					 TIMER1_CLEAR_PRESCALAR_BITS;

					 switch(TIMER_cnfg_arr[TIMER_Select].prescalar)
					 {

						 case PRESCALER0:
						 {
							 TIMER1_NO_PRESCALAR();
							 break;
						 }

						 case PRESCALER8:
						 {
							 TIMER1_PRESCALAR_8();
							 break;
						 }

						 case PRESCALER64:
						 {
							 TIMER1_PRESCALAR_64();
							 break;
						 }

						 case PRESCALER256:
						 {
							 TIMER1_PRESCALAR_256();
							 break;
						 }

						 case PRESCALER1024:
						 {

							 TIMER1_PRESCALAR_1024();
							 break;
						 }

						 case EXTERNAL_CLK_RISING:
						 {
							 EXT_TIMER_RISING();
							 break;
						 }

						 case EXTERNAL_CLK_FALLING:
						 {
							 EXT_TIMER_FALLING();
							 break;
						 }

						 default:
						 {
							 TIMER_cnfg_arr[TIMER_Select].IS_init=NOT_INITIALIZED;
							 STATE=NAK;
							 break;
						 }

					 }
					 break;
				 }



				 case TIMER2:
				 {
					 REG_TCCR2 &= (0b11111000);	//TIMER2_CLEAR_PRESCALAR_BITS;

					 switch(TIMER_cnfg_arr[TIMER_Select].prescalar)
					 {
						 case PRESCALER0:
						 {
							 REG_TCCR2 |= (0b00000001);
							 break;
						 }

						 case PRESCALER8:
						 {
							 REG_TCCR2 |= (0b00000010);
							 break;
						 }

						 case PRESCALER32:
						 {
							 REG_TCCR2 |= (0b00000011);
							 break;
						 }

						 case PRESCALER64:
						 {
							 REG_TCCR2 |= (0b00000100);
							 break;
						 }

						 case PRESCALER128:
						 {
							 REG_TCCR2 |= (0b00000101);
							 break;
						 }

						 case PRESCALER256:
						 {
							 REG_TCCR2 |= (0b00000110);
							 break;
						 }

						 case PRESCALER1024:
						 {
							 REG_TCCR2 |= (0b00000111);
							 break;
						 }

						 default:
						 {
							 TIMER_cnfg_arr[TIMER_Select].IS_init=NOT_INITIALIZED;
							 STATE=NAK;
							 break;
						 }
					 }
					 break;
				 }


				 default:
				 {
					 TIMER_cnfg_arr[TIMER_Select].IS_init=NOT_INITIALIZED;
					 STATE=NAK;
					 break;
				 }

			 }

		 }

		else
		{
			STATE=NAK;
		}


	 return
	 STATE;
 }


/*ACK PWM_generate( TIMER_t TIMER_Select , float Required_duty_cycle , uint8 type )
 {

	 ACK STATE = AK ;

	 uint16 Top=0;
	 if((TIMER_cnfg_arr[TIMER_Select].timer == TIMER_Select) && (TIMER_cnfg_arr[TIMER_Select].IS_init==INITIALIZED) )
	 {
		 switch(TIMER_Select)
		 {
			 case TIMER0:
			 {
				 OCR0 = (255 * (float)(Required_duty_cycle/100));
				 break;
			 }

			 case TIMER1:
			 {
				 Top=65535;
				 OCR1A= (Top*(float)(Required_duty_cycle/100));
				 break;
			 }

			 case TIMER2:
			 {
				 Top=255;
				 REG_OCR2 = (Top*(float)(Required_duty_cycle/100));
				 break;
			 }

			 default:
			 {
				 // Should not be here
				 STATE=NAK;
				 break;
			 }
		 }

		 switch(Flag_mode[TIMER_Select])
		 {
			 case NON_PWM_MODE:
			 {
				 if (TIMER_cnfg_arr[TIMER_Select].interrupt==INTERRUPT)
				 {
					 STATE= interrupt_PWM (TIMER_Select);

					 if (type == INVERTING)
					 {
						 PWM_Value=0;
						 DIO_write(PWM_PORT,PWM_PIN,PWM_Value);
					 }
					 else if(type == NON_INVERTING)
					 {
						 PWM_Value=1;
						 DIO_write(PWM_PORT,PWM_PIN,PWM_Value);
					 }
					 break;
				 }

				 else
				 {
					 STATE=NAK;
					 break;
				 }

			 }

			 case PWM_MODE:
			 {
				 break;
			 }

			 default:
			 {
				 STATE=NAK;
				 break;
			 }
		 }

	 }

	 else
	 {
		 STATE=NAK;
	 }

	 TIMER_Start(TIMER_Select);
	 return STATE;
 }

*/
ACK Update_Timer_OCR_Register (TIMER_t TIMER_Select)	//Private Function
{
	ACK STATE = AK;
	switch(TIMER_Select)
	{
		case TIMER0:
		{
			
			OCR0 = (uint8) OCR_Value[TIMER_Select];
			break;
		}

		case TIMER1:
		{
			//REG_OCR1AH =  OCR_Value/256;
			//REG_OCR1AL =  OCR_Value%256;
			OCR1A = (uint16) OCR_Value[TIMER_Select];
			break;
		}

		case TIMER2:
		{
			REG_OCR2 = (uint8) OCR_Value[TIMER_Select];
			break;
		}

		default:
		{
			// Should not be here
			STATE = NAK;
			break;
		}
	}

	return STATE ;
}


/*ACK interrupt_PWM (TIMER_t TIMER_Select)  //Private Function
 {
	 ACK STATE= AK;
	 switch(TIMER_Select)
	 {
		 case TIMER0:
		 {

			 ENABLE_GLOBAL_INTERRUPT;
			 ENABLE_OVERFLOW_INTERRUPT_TIMER0;
			 REG_TIMSK |= (1<<OCIE0);
			 break;
		 }
		 case TIMER2:
		 {
			 ENABLE_GLOBAL_INTERRUPT;
			 ENABLE_OVERFLOW_INTERRUPT_TIMER2;
			 REG_TIMSK |= (1<<OCIE2);
			 break;
		 }
		 case TIMER1:
		 {
			 ENABLE_GLOBAL_INTERRUPT;
			 ENABLE_OVERFLOW_INTERRUPT_TIMER1;
			 REG_TIMSK |= (1<<OCIE1A);
			 break;
		 }
		 default:
		 {
			 STATE = NAK;
			 break;
		 }

	 }
	 return STATE;
 }

*/
ACK interrupt_time_delay (TIMER_t TIMER_Select)
 {
	 ACK STATE =AK;
	 static uint8 Counter=0;

	 if ((TIMER_cnfg_arr[TIMER_Select].IS_init == INITIALIZED) && (TIMER_cnfg_arr[TIMER_Select].interrupt == INTERRUPT))
	 {
		 Counter++;

		 if(Counter == OVF_Counter_Loop)
		 {
			 Running_Flag[TIMER_Select]=1;
		 }
	 }

	 else
	 {
		 STATE = NAK;
	 }

	 return STATE;
 }


ACK TIMER_Stop (TIMER_t TIMER_Select)
{
	ACK STATE=AK;

	if ( (TIMER_cnfg_arr[TIMER_Select].timer == TIMER_Select) && (TIMER_cnfg_arr[TIMER_Select].IS_init==INITIALIZED) )
	{
		switch (TIMER_Select)
		{
			case TIMER0:
			{
				REG_TCCR0 &= (0b11111000);	//TIMER0_CLEAR_PRESCALAR_BITS;
				break;
			}

			case TIMER1:
			{
				TIMER1_CLEAR_PRESCALAR_BITS;
				break;
			}


			case TIMER2:
			{
				REG_TCCR2 &= (0b11111000);	//TIMER2_CLEAR_PRESCALAR_BITS;
				break;
			}

			default:
			{
				STATE=NAK;
				break;
			}
		}
	}

	else
	{
		STATE =NAK;
	}

	return STATE;
}

ACK Enable_Timer_Interrupt (TIMER_t TIMER_Select)
{
	ACK STATE = AK;
	if( (TIMER_cnfg_arr[TIMER_Select].WGM_mode == NORMAL_MODE) && (TIMER_cnfg_arr[TIMER_Select].IS_init ==  INITIALIZED)&& (TIMER_cnfg_arr[TIMER_Select].interrupt == INTERRUPT) )
	{
		ENABLE_GLOBAL_INTERRUPT;

		switch(TIMER_Select)
		{
			case TIMER0:
			{
				ENABLE_OVERFLOW_INTERRUPT_TIMER0;
				break;
			}
			case TIMER1:
			{
				ENABLE_OVERFLOW_INTERRUPT_TIMER1;
				break;
			}
			case TIMER2:
			{
				ENABLE_OVERFLOW_INTERRUPT_TIMER2;
				break;
			}
			default:
			{
				STATE = NAK;
				break;
			}
		}
	}

	else if( (TIMER_cnfg_arr[TIMER_Select].WGM_mode ==CTC_MODE) && (TIMER_cnfg_arr[TIMER_Select].IS_init ==  INITIALIZED)&& (TIMER_cnfg_arr[TIMER_Select].interrupt == INTERRUPT) )
	{
		ENABLE_GLOBAL_INTERRUPT;

		switch(TIMER_Select)
		{
			case TIMER0:
			{
				ENABLE_OUTPUT_COMPARE_INTERRUPT_TIMER0;
				break;
			}

			case TIMER1:
			{
				ENABLE_OUTPUT_COMPARE_INTERRUPT_TIMER1;
				break;
			}

			case TIMER2:
			{
				ENABLE_OUTPUT_COMPARE_INTERRUPT_TIMER2;
				break;
			}
			default:
			{
				STATE = NAK;
				break;
			}
		}
	}
	return STATE;
}



/*
ISR(TIMER1_OVF_vect)
{

}

ISR(TIMER2_OVF_vect)
{

}

*/

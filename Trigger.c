

#include "Trigger.h"
#include "avr/io.h"

void Trigger_int (void)
{
	DDR_TRIGGER |= (1<<PIN_TRIGGER);
	Trigger_Pulse ();
}

 void Trigger_Pulse (void)
{


	PORT_TRIGGER |= (1<<PIN_TRIGGER);
	timer2_delay_ms(100);
	//Time_Delay(timer2,1,ms);
	PORT_TRIGGER &= ~(1<<PIN_TRIGGER);

}


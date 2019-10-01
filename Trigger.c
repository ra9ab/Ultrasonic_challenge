

#include "Trigger.h"
#include <avr/io.h>


void Trigger_int (void)
{
	DDR_TRIGGER |= (1<<PIN_TRIGGER);
}

void Trigger_Pulse (void)
{
	PORT_TRIGGER |= (1<<PIN_TRIGGER);
	Time_Delay(TIMER1,10,us);
	PORT_TRIGGER &= ~(1<<PIN_TRIGGER);
}

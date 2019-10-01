

#ifndef _Trigger_H

#define _Trigger_H

#include "Timer_Draft.h"

void Trigger_Pulse (void);
void Trigger_int (void);
#define DDR_TRIGGER  (DDRB)
#define PORT_TRIGGER (PORTB)
#define PIN_TRIGGER	 (PB2)


#endif // _Trigger_H

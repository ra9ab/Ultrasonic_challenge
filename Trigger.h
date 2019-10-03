


#ifndef _Trigger_H

#define _Trigger_H

#include "Timer_Draft.h"
void Trigger_Pulse (void);
void Trigger_int (void);

#define DDR_TRIGGER  (DDRD)
#define PORT_TRIGGER (PORTD)
#define PIN_TRIGGER	 (PD6)


#endif // _Trigger_H

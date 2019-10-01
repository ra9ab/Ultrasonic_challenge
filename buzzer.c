#include "buzzer.h"

void buzzer_on()
{
	DIO_set_pin_direction(PORT_B, PIN3,OUTPUT);
	DIO_write_pin_value(PORT_B, PIN3,HIGH);
}

void buzzer_off()
{
	DIO_set_pin_direction(PORT_B, PIN3,OUTPUT);
	DIO_write_pin_value(PORT_B, PIN3,LOW);
}



/* in main().
while (1)
{
	uint8 x = detect_object();
	uint32 obj_detect = count_object(x);
	
}

*/


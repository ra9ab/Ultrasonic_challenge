#include "buzzer.h"
#include "DIO.h"

void buzzer_init(void)
{
	DIO_set_pin_direction(PORT_B, PIN3,OUTPUT);
}
void buzzer_on()
{
	//DIO_set_pin_direction(PORT_B, PIN3,OUTPUT);
	DIO_write_pin_value(PORT_B, PIN3,HIGH);
}

void buzzer_off()
{
	//DIO_set_pin_direction(PORT_B, PIN3,OUTPUT);
	DIO_write_pin_value(PORT_B, PIN3,LOW);
}






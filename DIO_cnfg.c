/*
 * DIO_cnfg.c
 *
 * Created:  23 , 9 , 2019
 *  Author: Peter
 */
#include "DIO.h"
extern DIO_cnfg_pin DIO_cnfg_arr[NUM_OF_PINS];
#define NA 0x55

DIO_cnfg_pin DIO_cnfg_arr [NUM_OF_PINS] =
{

		{PORT_D,PIN6,OUTPUT,LOW,NA}
		/*
	{PORT_C,PIN0,OUTPUT,LOW,NA},
	{PORT_C,PIN1,OUTPUT,LOW,NA},
	{PORT_D,PIN0,OUTPUT,HIGH,NA},
	{PORT_D,PIN1,OUTPUT,HIGH,NA},
	{PORT_D,PIN2,OUTPUT,HIGH,NA},
	{PORT_D,PIN3,OUTPUT,HIGH,NA},
	{PORT_D,PIN4,OUTPUT,HIGH,NA},
	{PORT_D,PIN5,OUTPUT,HIGH,NA},
	,
	{PORT_B,PIN1,OUTPUT,HIGH,NA},
*/
};

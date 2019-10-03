/*
 * icu_cnfg.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: COMPUTER SHOW
 */

#ifndef ICU_CNFG_H_
#define ICU_CNFG_H_

//#include<avr/io.h>
//#include<avr/interrupt.h>
//#include"interrupt_manual.h"
#include"interrupt_manual.h"
#include"registers.h"
#include"stdtypes.h"
#include"macros.h"
#include"pins.h"


#define NULL_PTR    ((void*)0)

typedef enum{
	NO_CLK,F_CPU_CLK,F_CPU_CLK_8,F_CPU_CLK_64,F_CPU_CLK_256,F_CPU_CLK_1024
}ICU_CLOCK;


typedef enum{
	FALLING,RISING
}ICU_EDGE;

typedef struct{
	ICU_EDGE edge;
	ICU_CLOCK clock;
}ICU_CNFG;

extern ICU_CNFG icu_config;

#endif /* ICU_CNFG_H_ */

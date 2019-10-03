/*
 * ICU.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: COMPUTER SHOW
 */

#ifndef ICU_H_
#define ICU_H_
#include"icu_cnfg.h"



void Icu_init(void);
void Icu_setCallBack(void (*a_ptr)(void));
void Icu_edgeSelect(const ICU_EDGE a_edge);
uint16 Icu_getInputCaptureValue(void);
void Icu_clearTimer(void);
void Icu_Deinit(void);



#endif /* ICU_H_ */

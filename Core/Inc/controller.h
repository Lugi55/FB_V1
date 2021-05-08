/*
 * controller.h
 *
 *  Created on: May 8, 2021
 *      Author: Heimgartner
 */

#include "stdint.h"


#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_


//defines
#define R0 10000
#define T0 25
#define B 3900
#define RS 10000
#define MAX_ADC_VALUE 4096



//Prototypes
void controller_setVoltagePWM(float voltage);
void controller_setCurrentMeasurement(uint32_t uint_adcVal);
void controller_setTempMeasurement(uint32_t adcVal);
void controller_printTemp(void);



#endif /* INC_CONTROLLER_H_ */

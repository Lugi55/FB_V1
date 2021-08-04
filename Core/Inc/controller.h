/*
 * controller.h
 *
 *  Created on: May 8, 2021
 *      Author: Heimgartner
 */

#include "stdint.h"
#include "types.h"


#ifndef INC_CONTROLLER_H_
#define INC_CONTROLLER_H_


//defines
#define R0 10000
#define T0 25
#define B 3900
#define RS 10000
#define MAX_ADC_VALUE 4096
#define VOLTAGE_GAIN 1.f



//Prototypes Public
void controller_update(adcval_t ADCValues);
void controller_setVoltagePWM(float voltage);
void controller_print(void);
void controller_setVoltagePWMConsumer(float voltage);
void controller_setReferenceTemp(float temp);


#endif /* INC_CONTROLLER_H_ */

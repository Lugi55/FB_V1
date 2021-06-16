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
#define R0 10000.0f
#define T0 25.0f
#define B 3900.0f
#define RS 10000.0f
#define MAX_ADC_VALUE 4096.0f



//Prototypes Public
void controller_update(adcval_t ADCValues);
void controller_setVoltagePWM(float voltage);
void controller_Controller(void);
void controller_print(void);


#endif /* INC_CONTROLLER_H_ */

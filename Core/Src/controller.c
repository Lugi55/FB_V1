/*
 * controller.c
 *
 *  Created on: May 8, 2021
 *      Author: Heimgartner
 */

#include "controller.h"
#include "hrtim.h"
#include "math.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"


float temp = 0;
float current = 0;






void controller_setVoltagePWM(float voltage){
	uint32_t compare = (54400/12*voltage);
	__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, HRTIM_COMPAREUNIT_1,compare);
}

void controller_setCurrentMeasurement(uint32_t uint_adcVal){

}

void controller_setTempMeasurement(uint32_t adcVal){
	float R;
	R = (float) adcVal/MAX_ADC_VALUE * 3.3;
	R = (float) R0*3.3/R - R0;

	temp = R/R0;
	temp = log(temp);
	temp /= B;
	temp += 1.0 / (T0 + 273.15);
	temp = 1.0 / temp;
	temp -= 273.15;
}

void controller_printTemp(){
	uint8_t buffer[20];
	sprintf(buffer, "%f\n\r", temp);
	HAL_UART_Transmit(&huart1, buffer, strlen(buffer), HAL_MAX_DELAY);
}

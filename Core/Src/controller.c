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
#include "dma.h"
#include "stdbool.h"



float tempHot = 0;
float tempCold = 0;
float operatingPoint = 15;
float hysterese = 1;
char stringBuffer[100];





void controller_setVoltagePWM(float voltage){
	uint32_t compare = (54400/12*voltage);
	__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, HRTIM_COMPAREUNIT_1,compare);
}



void controller_setTempMeasurementCold(uint32_t adcVal){
	float R;
	R = (float) adcVal/MAX_ADC_VALUE * 3.3;
	R = (float) R0*3.3/R - R0;
	tempCold = R/R0;
	tempCold = log(tempCold);
	tempCold /= B;
	tempCold += 1.0 / (T0 + 273.15);
	tempCold = 1.0 / tempCold;
	tempCold -= 273.15;
}

void controller_setTempMeasurementHot(uint32_t adcVal){
	float R;
	R = (float) adcVal/MAX_ADC_VALUE * 3.3;
	R = (float) R0*3.3/R - R0;
	tempHot = R/R0;
	tempHot = log(tempHot);
	tempHot /= B;
	tempHot += 1.0 / (T0 + 273.15);
	tempHot = 1.0 / tempHot;
	tempHot -= 273.15;
}

void controller_printTemp(){
	sprintf(stringBuffer, "Hot:\t%.3f\t\tCold:\t %.3f\n\r", tempHot, tempCold);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)stringBuffer, strlen(stringBuffer));
}





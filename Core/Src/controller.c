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


//Module Variables
// ----------------------------------------------------------------------------------------
float tempHot;
float tempCold;
char stringBuffer[500];
float setVoltage;
float refernece = 15;
uint32_t current;
float voltage;

//static Prototypes
// ----------------------------------------------------------------------------------------
static void updateTempMeasurementCold(uint32_t adcVal);
static void updateTempMeasurementHot(uint32_t adcVal);
static void updateVoltage(uint32_t adcVal);



// Function: controller_Controller
// ----------------------------------------------------------------------------------------
//   input: void
//   return: void
void controller_Controller(){
	float e;
	e = refernece - tempCold;
}

// Function: controller_update
// ----------------------------------------------------------------------------------------
//   input: *ADCValues	Array of ADC Values
//   return: void
void controller_update(adcval_t ADCValues){
	updateTempMeasurementCold(ADCValues.ADC1Val[0]);
	updateTempMeasurementHot(ADCValues.ADC1Val[1]);
	updateVoltage(ADCValues.ADC2Val[0]);
	current = ADCValues.ADC2Val[1];
}

// Function: controller_setPWM
// ----------------------------------------------------------------------------------------
//   input-> voltage:
//   return: void
void controller_setVoltagePWM(float voltage){
	uint32_t compare;
	setVoltage = voltage;
	compare = voltage/12.0f*PERIOD_100;
	__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, HRTIM_COMPAREUNIT_1,compare);
}

// Function: controller_print
// ----------------------------------------------------------------------------------------
//   input: void
//   return: void
void controller_print(){
	char stringBufferTemp[100];
	stringBuffer[0] = '\0';
	sprintf(stringBufferTemp,"\"Time\":%u,",HAL_GetTick());
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\"SetVoltage\":%.3f," ,setVoltage);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\"HotTemp\":%.3f," ,tempHot);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\"ColdTemp\":%.3f,",tempCold);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\"Current\":%u,",current);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\"Voltage\":%.3f",voltage);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\n\r");
	strcat(stringBuffer,stringBufferTemp);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)stringBuffer, strlen(stringBuffer));
}

// Function: updateTempMeasurementHot
// ----------------------------------------------------------------------------------------
//   input: uint32_t adcVal
//   return: void
static void updateTempMeasurementHot(uint32_t adcVal){
	float R;
	R = adcVal/MAX_ADC_VALUE * 3.3f;
	R = R0*3.3f/R - R0;
	tempHot = R/R0;
	tempHot = log(tempHot);
	tempHot /= B;
	tempHot += 1.0f / (T0 + 273.15f);
	tempHot = 1.0f / tempHot;
	tempHot -= 273.15f;
}

// Function: updateTempMeasurementCold
// ----------------------------------------------------------------------------------------
//   input: uint32_t adcVal
//   return: void
static void updateTempMeasurementCold(uint32_t adcVal){
	float R;
	R = adcVal/MAX_ADC_VALUE * 3.3f;
	R = R0*3.3f/R - R0;
	tempCold = R/R0;
	tempCold = log(tempCold);
	tempCold /= B;
	tempCold += 1.0f / (T0 + 273.15f);
	tempCold = 1.0f / tempCold;
	tempCold -= 273.15f;
}


static void updateVoltage(uint32_t adcVal){
	voltage = adcVal/MAX_ADC_VALUE * 3.3f;
	voltage *= 4;
}




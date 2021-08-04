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
float referneceTemp = 20;
float tempHot;
float tempCold;
char stringBuffer[500];
float setVoltage;
float suplyVoltage;
float current;
float voltage;
float PWMConsumerVoltage;



//static Prototypes
// ----------------------------------------------------------------------------------------
static void updateTempMeasurementCold(uint32_t adcVal);
static void updateTempMeasurementHot(uint32_t adcVal);
static void updateVoltage(uint32_t adcVal);
static void updateSuplyVoltage(uint32_t adcVal);
static void updateCurrent(uint32_t adcVal);
static void updateSetVoltagePWM();
static void controller();



// Function: controller_update
// ----------------------------------------------------------------------------------------
//
void controller_update(adcval_t ADCValues){
	updateTempMeasurementCold(ADCValues.ADC1Val[0]);
	updateTempMeasurementHot(ADCValues.ADC1Val[1]);
	updateVoltage(ADCValues.ADC2Val[0]);
	updateCurrent(ADCValues.ADC2Val[1]);
	updateSuplyVoltage(ADCValues.ADC5Val[0]);
	controller();
	updateSetVoltagePWM();
}

// Function: controller_setVoltagePWM
// ----------------------------------------------------------------------------------------
//
void controller_setVoltagePWM(float voltage){
	setVoltage = voltage;
}

// Function: controller_setReferenceTemp
// ----------------------------------------------------------------------------------------
//
void controller_setReferenceTemp(float temp){
	referneceTemp = temp;
}

// Function: controller_setVoltagePWMConsumer
// ----------------------------------------------------------------------------------------
//
void controller_setVoltagePWMConsumer(float voltage){
	PWMConsumerVoltage = voltage;
	uint32_t compare;
	compare = voltage/suplyVoltage*PERIOD_100;
	if(compare < 121){
		compare = 121;
	}
	__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_C, HRTIM_COMPAREUNIT_1,compare);
}

// Function: controller_print
// ----------------------------------------------------------------------------------------
//
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
	sprintf(stringBufferTemp, "\"Current\":%.3f,",current);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\"RefTemp\":%.3f,",referneceTemp);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\"Voltage\":%.3f,",voltage);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\"PWMConsumerVoltage\":%.3f,",PWMConsumerVoltage);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\"SuplyVoltage\":%.3f",suplyVoltage);
	strcat(stringBuffer,stringBufferTemp);
	sprintf(stringBufferTemp, "\n\r");
	strcat(stringBuffer,stringBufferTemp);
	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)stringBuffer, strlen(stringBuffer));
}

// Function: updateTempMeasurementHot
// ----------------------------------------------------------------------------------------
//
static void updateTempMeasurementHot(uint32_t adcVal){
	float R;
	R = 3.3f*adcVal/MAX_ADC_VALUE;
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
//
static void updateTempMeasurementCold(uint32_t adcVal){
	float R;
	R = 3.3f*adcVal/MAX_ADC_VALUE;
	R = R0*3.3f/R - R0;
	tempCold = R/R0;
	tempCold = log(tempCold);
	tempCold /= B;
	tempCold += 1.0f / (T0 + 273.15f);
	tempCold = 1.0f / tempCold;
	tempCold -= 273.15f;
}

// Function: updateVoltage
// ----------------------------------------------------------------------------------------
//
static void updateVoltage(uint32_t adcVal){
	voltage = 3.3f*adcVal/MAX_ADC_VALUE;
	voltage *= 4.0f;
}

// Function: updateSuplyVoltage
// ----------------------------------------------------------------------------------------
//
static void updateSuplyVoltage(uint32_t adcVal){
	suplyVoltage = 3.3f*adcVal/MAX_ADC_VALUE;
	suplyVoltage *= 4.0f;
}

// Function: updateCurrent
// ----------------------------------------------------------------------------------------
//
static void updateCurrent(uint32_t adcVal){
	current = 3.3f*adcVal/MAX_ADC_VALUE;
	current -= 1.65f;
	current /= -20;
	current /= 0.02;
}

static void updateSetVoltagePWM(){
	uint32_t compare;
	compare = VOLTAGE_GAIN*setVoltage/suplyVoltage*PERIOD_100;
	if(compare < 121){
		compare = 121;
	}
	__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, HRTIM_COMPAREUNIT_1,compare);
}



static void controller(){
	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
	static float uk1 = 0;
	static float ek1 = 0;
	float ek = -(referneceTemp - tempCold);
	float uk = uk1 + 0.5642f*ek - 0.55f*ek1;

	if(uk>5){
		uk=5;
	}
	if(uk<0){
		uk = 0;
	}

	uk1 = uk;
	ek1 = ek;
	setVoltage = uk;
}



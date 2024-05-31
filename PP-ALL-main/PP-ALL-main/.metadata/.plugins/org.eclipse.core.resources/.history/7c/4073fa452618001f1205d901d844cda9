/*
 * subroutine.c
 *
 *  Created on: May 4, 2024 -> Screwed on: May 20, 2024
 *      Author: User -> By: your member, Pupreaw
 */


// Include library
#include "main.h"
#include "string.h"

/* 	Function: updateLED
 *  Description: Update status LED at the control panel
 *	Return type : integer
 *		0 : Ok
 *		1 : Error
 *		2 : DMA/Timer Busy
 *		3 : DMA/Timer Timeout
 *
 *	Parameter:
 *		ledVal: Led color to be display, 1 is red, 2 is green, 3 is blue (There are 3 LEDS)
 *		htim: Timer's pointer that control data generation (Timer 3)
 *		timCH: Timer channel that connect to LED (CH2)
 *
 *	Variable:
 *		ledPayload: (unsigned 16-bit integer array) Contain data for sending to led
 *		ledBuff: (unsigned 16-bit integer array) Contain data for sending to led
 *
 *	Constant:
 *		ledOn: (unsigned 16-bit integer array) Pulse that make respect completely LED on.
 *		ledOff: (unsigned 16-bit integer array) Pulse that make respect completely LED off.
 */

uint8_t updateLED(uint8_t* ledVal, TIM_HandleTypeDef* htim, uint32_t timCH){

	const uint16_t ledOn[8] = {72,72,72,72,72,72,72,72};
	const uint16_t ledOff[8] = {34,34,34,34,34,34,34,34};

	// Reset LED
	static uint16_t ledPayload[132];

	for (int i = 0; i < 3; i++){
		uint16_t ledBuff[24];
		switch(ledVal[i]) {

		case 1:
			(void)memcpy(&ledBuff[0], ledOff, sizeof(ledOff));
			(void)memcpy(&ledBuff[8], ledOn, sizeof(ledOn));
			(void)memcpy(&ledBuff[16], ledOff, sizeof(ledOff));

			break;

		case 2:
			(void)memcpy(&ledBuff[0], ledOn, sizeof(ledOn));
			(void)memcpy(&ledBuff[8], ledOff, sizeof(ledOff));
			(void)memcpy(&ledBuff[16], ledOff, sizeof(ledOff));

			break;

		case 3:
			(void)memcpy(&ledBuff[0], ledOff, sizeof(ledOff));
			(void)memcpy(&ledBuff[8], ledOff, sizeof(ledOff));
			(void)memcpy(&ledBuff[16], ledOn, sizeof(ledOn));

			break;

		default:
			(void)memcpy(&ledBuff[0], ledOff, sizeof(ledOff));
			(void)memcpy(&ledBuff[8], ledOff, sizeof(ledOff));
			(void)memcpy(&ledBuff[16], ledOff, sizeof(ledOff));

			break;
		}

		(void)memcpy(&ledPayload[60+(i*24)], ledBuff, sizeof(ledBuff));

	}

	(void)memset(ledPayload, 0, sizeof(uint16_t)*60);

	return HAL_TIM_PWM_Start_DMA(htim, timCH, ledPayload, 132);

}


/* 	Function: retractX
 *  Description: Retract x axis until it is fully retracted or timeout. This function is blocking
 *	Return type : integer
 *		0 : Ok
 *		1 : Timeout
 *
 *	Parameter:
 *		None
 *
 *	Constant:
 *		TimeoutConst: (Unsigned 32-bit integer) blocking timeout if X axis doesn't retract, use to calculate later
 *
 *	Variable:
 *		Timeout: (Unsigned 32-bit integer) blocking timeout if X axis doesn't retract
 *
 *
 *
 *
 *
 */

uint8_t retractX(){

	const uint32_t TimeoutConst = 1000;

	uint32_t Timeout = HAL_GetTick() + TimeoutConst;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

	while(HAL_GetTick() < Timeout){
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_SET){

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

			return 0;
		}
	}


	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

	return 1;


}




/* 	Function: extendX
 *  Description: Extend x axis until it is fully extended or timeout. This function is blocking.
 *	Return type : integer
 *		0 : Ok
 *		1 : Timeout
 *
 *	Parameter:
 *		None
 *
 *	Constant:
 *		TimeoutConst: (Unsigned 32-bit integer) blocking timeout if X axis doesn't extend, use to calculate later.
 *
 *	Variable:
 *		Timeout: (Unsigned 32-bit integer) blocking timeout if X axis doesn't extend.
 *
 *
 *
 *
 *
 */

uint8_t extendX(){

	const uint32_t TimeoutConst = 1000;

	uint32_t Timeout = HAL_GetTick() + TimeoutConst;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

	while(HAL_GetTick() < Timeout){
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == GPIO_PIN_RESET){

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

			return 0;
		}
	}


	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

	return 1;


}





/* 	Function: HomeZ
 * 	Description: Slowly pulled Z down until hit end-stop or Timeout.
 *	Return type : integer
 *		0 : Ok
 *		1 : Timeout
 *
 *	Parameter:
 *
 *	Constant:
 *		TimeoutConst: (Unsigned 32-bit integer) blocking timeout if Z doesn't hit endstop, use to calculate later.
 *		HomingSpeed: (Unsigned 16-bit integer) homing speed in z axis when is going to home
 *
 *	Variable:
 *		Timeout: (Unsigned 32-bit integer) blocking timeout if Z axis doesn't hit endstop.
 *
 *
 */

uint8_t HomeZ(){

	const uint32_t TimeoutConst = 20000;
	const uint32_t HomingSpeed = 20000;

	uint32_t Timeout = HAL_GetTick() + TimeoutConst;

	serviceMotor(HomingSpeed, 0);

	while(HAL_GetTick() < Timeout){
		if(getZStop() == 1){
			serviceMotor(0, 0);
			return 0;
		}

	}

	serviceMotor(0, 0);
	return 1;

}

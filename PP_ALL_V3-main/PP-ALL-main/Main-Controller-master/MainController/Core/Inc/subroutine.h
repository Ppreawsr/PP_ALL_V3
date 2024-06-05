/*
 * subroutine.h
 *
 *  Created on: May 4, 2024
 *      Author: User
 */

#ifndef INC_SUBROUTINE_H_
#define INC_SUBROUTINE_H_

uint8_t updateLED(uint8_t* ledVal, TIM_HandleTypeDef* htim, uint32_t timCH);
uint8_t retractX();
uint8_t extendX();
uint8_t HomeZ();
uint8_t pickBox();
uint8_t placeBox();

#endif /* INC_SUBROUTINE_H_ */

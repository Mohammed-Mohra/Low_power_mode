/*
 * it.c
 *
 *  Created on: Jan 24, 2022
 *      Author: sa
 */
#include "it.h"
#include "main.h"
extern TIM_HandleTypeDef	tim6;
extern UART_HandleTypeDef UART2;
extern EXTI_HandleTypeDef		hexti;

void SysTick_Handler()
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


void EXTI0_IRQHandler()
{
	HAL_EXTI_ClearPending(&hexti,EXTI_TRIGGER_RISING);

	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
}

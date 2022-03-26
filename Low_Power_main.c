/*
 * PLL.main.c
 *
 *  Created on: Jan 26, 2022
 *      Author: sa
 */


/*
 * main.c
 *
 *  Created on: Jan 24, 2022
 *      Author: sa
 */

#include "stm32f4xx_hal.h"
#include <string.h>
#include "main.h"
#include "it.h"

EXTI_HandleTypeDef		hexti;
EXTI_ConfigTypeDef		hexti_config;

void button_config();
void gpio_led_init();
void Error_Handler();
void SystemClockConfig(uint8_t systick_clk_val);
void gpioa_analog_config();

int main(void)
{
	HAL_Init();
	SystemClockConfig(SYS_CLK_50MH);
	SysTick_Handler();
	gpio_led_init();
	TIM6->SR = 0x0000;
	gpioa_analog_config();
	//__WFI();

	button_config();

	//__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();
	//HAL_PWR_EnableSleepOnExit();



/*	if(HAL_TIM_Base_Start_IT(&tim6) != HAL_OK)
		Error_Handler();
*/

	while(1)
	{

		HAL_SuspendTick();
		__WFE();
		HAL_ResumeTick();
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);

	}
	return 0;
}


void button_config()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	hexti_config.Line = EXTI_LINE_0;
	hexti_config.GPIOSel = EXTI_GPIOA;
	hexti_config.Mode = EXTI_MODE_EVENT;
	hexti_config.Trigger = EXTI_TRIGGER_RISING;



	HAL_EXTI_SetConfigLine(&hexti, &hexti_config);

	GPIO_InitTypeDef	led;
	led.Mode = GPIO_MODE_EVT_RISING;
	led.Pin = GPIO_PIN_0;
	led.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &led);

}



void gpioa_analog_config()
{
	GPIO_InitTypeDef	gpio_led;
	gpio_led.Mode = GPIO_MODE_ANALOG;
	gpio_led.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |\
			GPIO_PIN_4 | GPIO_PIN_5 |GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | \
			GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14 | \
			GPIO_PIN_15;
	gpio_led.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &gpio_led);

}


void gpio_led_init()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef	gpio_led;
	gpio_led.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_led.Pin = GPIO_PIN_12;
	gpio_led.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &gpio_led);
}

void SystemClockConfig(uint8_t systick_clk_val)
{
	// Configuring the Clock of the device to HSE
	RCC_ClkInitTypeDef	rcc_clk_init;
	rcc_clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |\
			RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;


	RCC_OscInitTypeDef 	rcc_hse;
	memset(&rcc_hse,0,sizeof(rcc_hse));
	rcc_hse.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	rcc_hse.HSIState = RCC_HSI_ON;
	rcc_hse.HSEState = RCC_HSE_OFF;

	switch(systick_clk_val)
	{
	case SYS_CLK_50MH:
		rcc_hse.PLL.PLLM = 16;
		rcc_hse.PLL.PLLN = 100;
		rcc_hse.PLL.PLLP = RCC_PLLP_DIV2;
		rcc_hse.PLL.PLLSource = RCC_PLLSOURCE_HSI;
		rcc_hse.PLL.PLLState = RCC_PLL_ON;

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_1WS) != HAL_OK)
			Error_Handler();
		break;
	case SYS_CLK_84MH:
		// To enable the microcontroller to work in full capacity u have to enable certain settings in power mode
		// First enabel the clock for

		rcc_hse.PLL.PLLM = 8;
		rcc_hse.PLL.PLLN = 84;
		rcc_hse.PLL.PLLP = RCC_PLLP_DIV2;
		rcc_hse.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		rcc_hse.PLL.PLLState = RCC_PLL_ON;

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_2WS) != HAL_OK)
			Error_Handler();
		break;
	case SYS_CLK_120MH:
		rcc_hse.PLL.PLLM = 8;
		rcc_hse.PLL.PLLN = 120;
		rcc_hse.PLL.PLLP = RCC_PLLP_DIV2;
		rcc_hse.PLL.PLLSource = RCC_PLLSOURCE_HSI;
		rcc_hse.PLL.PLLState = RCC_PLL_ON;

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_4WS) != HAL_OK)
			Error_Handler();
		break;
	}



	HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void Error_Handler()
{
	while(1);
}

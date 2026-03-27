// main.c

#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>

void SystemClockConfig(void);
// void UART2_Init(void);
void Error_handler(void);
void TIMER6_Init(void);
void GPIO_Init(void);
TIM_HandleTypeDef htimer6;


int main(void)
{
	HAL_Init();
	SystemClockConfig();

	GPIO_Init();
	TIMER6_Init();

	//Let's start the timer in polling mode
	// if (HAL_TIM_Base_Start(&htimer6) != HAL_OK)
	// {
	// 	Error_handler();
	// }

	//Let's start the timer in interrupt mode
	if (HAL_OK != HAL_TIM_Base_Start_IT(&htimer6))
	{
		Error_handler();
	}

	// while (1)
	// {
	// 	// loop until the update event flag is set	
	// 	while ( !(TIM6->SR & TIM_SR_UIF) ) ;
	// 	// clear the update event flag

	// 	// HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	// 	// TIM6->SR &= ~TIM_SR_UIF;

	// 	// Toggle example
	// 	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); // Green LED
	// 	TIM6->SR &= ~TIM_SR_UIF;
	// }
	

	while(1) ;

	return 0;
}

void SystemClockConfig(void)
{

}

void Error_handler(void)
{
	while(1) ;
}

void GPIO_Init(void)
{
	// __HAL_RCC_GPIOA_CLK_ENABLE();
	// GPIO_InitTypeDef ledgpio;
	// ledgpio.Pin = GPIO_PIN_5;
	// ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	// ledgpio.Pull = GPIO_NOPULL;

	// HAL_GPIO_Init(GPIOA, &ledgpio);

	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


}

void TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 24; // Assuming APB1 timer clock is 24 MHz
	htimer6.Init.Period = 64000 - 1; // 1 second period

	if (HAL_OK != HAL_TIM_Base_Init(&htimer6))
	{
		Error_handler();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htimer6)
{
	if (htimer6->Instance == TIM6)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); // Toggle Green LED
	}
}


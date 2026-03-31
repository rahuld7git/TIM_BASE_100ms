// main.c

#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>

typedef enum
{
	SYS_CLOCK_FREQ_50_MHZ = 0,
	SYS_CLOCK_FREQ_84_MHZ,
	SYS_CLOCK_FREQ_120_MHZ
} system_clock_freq_t;

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);
void TIMER6_Init(void);
void GPIO_Init(void);
TIM_HandleTypeDef htimer6;
UART_HandleTypeDef huart2;

uint32_t F_Latency = 0;


int main(void)
{
	HAL_Init();
	SystemClockConfig();

	GPIO_Init();
	TIMER2_Init();
	UART2_Init();

	//Let's start the timer in polling mode
	// if (HAL_TIM_Base_Start(&htimer6) != HAL_OK)
	// {
	// 	Error_handler();
	// }

	//Let's start the timer in interrupt mode
	HAL_TIM_Base_Start_IT(&htimer6);
	if (HAL_OK != HAL_TIM_IC_Start_IT(&htimer6, TIM_CHANNEL_1))
	{
		Error_handler();
	}

	while(1)
	{
		uint32_t time_period = 0;
		char msg[100];
		float timer2_count_frequency = 0.0f;
		float timer2_count_resolution = 0.0f;
		float user_signal_time_period = 0.0f;
		float user_signal_frequency = 0.0f;
		if (is_capture_done)
		{

			if (capture2 > capture1)
			{
				time_period = capture2 - capture1;
			}
			else if (capture2 < capture1)
			{
				time_period = (0xFFFFFFFF - capture1) + capture2;
			}
			else
			{
				Error_handler();
			}

			is_capture_done = false;
		}

		timer2_count_frequency = HAL_RCC_GetPCLK1Freq() / (htimer2.Init.Prescaler + 1);
		timer2_count_resolution = 1.0f / timer2_count_frequency;
		user_signal_time_period = time_period * timer2_count_resolution;
		user_signal_frequency = 1.0f / user_signal_time_period;

		sprintf(msg, "Time period = %f seconds, Frequency = %f Hz\r\n", user_signal_time_period, user_signal_frequency);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

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

void SystemClock_Config(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = RCC_PLLP_DIV2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			F_Latency = FLASH_ACR_LATENCY_1WS;
			break;

		case SYS_CLOCK_FREQ_84_MHZ:
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = RCC_PLLP_DIV2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

			F_Latency = FLASH_ACR_LATENCY_3WS;
			break;

		case SYS_CLOCK_FREQ_120_MHZ:
			osc_init.PLL.PLLM = 16;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = RCC_PLLP_DIV2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			F_Latency = FLASH_ACR_LATENCY_3WS;
			break;

		default:
			return;
	}

	if (HAL_OK != HAL_RCC_OscConfig(&osc_init))
	{
		Error_handler();
	}

	if (HAL_OK != HAL_RCC_ClockConfig(&clk_init, F_Latency))
	{
		Error_handler();
	}

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
	TIM_IC_InitTypeDef timer6_input_capture_config;

	timer6_input_capture_config.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	timer6_input_capture_config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	timer6_input_capture_config.ICPrescaler = TIM_ICPSC_DIV1;
	timer6_input_capture_config.ICFilter = 0;

	if (HAL_OK != HAL_TIM_IC_ConfigChannel(&htimer6, &timer6_input_capture_config, TIM_CHANNEL_1))
	{
		Error_handler();
	}

	htimer6.Instance = TIM6;
	htimer6.Init.Period = 50-1;
	htimer6.Init.Prescaler = 9;

	if (HAL_OK != HAL_TIM_IC_Init(&htimer6))
	{
		Error_handler();
	}
}

uint8_t count = 1;
uint32_t capture1 = 0;
uint32_t capture2 = 0;
bool is_capture_done = false;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htimer6)
{
	if(count == 1)
	{
		capture1 = __HAL_TIM_GET_COMPARE(htimer6, TIM_CHANNEL_1);
		count = 2;
	}
	else if(count == 2)
	{
		capture2 = __HAL_TIM_GET_COMPARE(htimer6, TIM_CHANNEL_1);
		count = 1;
		is_capture_done = true;
	}

	if (htimer6->Instance == TIM6)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13); // Toggle Orange LED
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htimer6)
{
	if (htimer6->Instance == TIM6)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); // Toggle Green LED
	}
}

void UART2_Init()
{
	huart2.Instance                     =    USART2;
	huart2.Init.BaudRate                =    115200;
	huart2.Init.WordLength              =    UART_WORDLENGTH_8B;
	huart2.Init.StopBits                =    UART_STOPBITS_1;
	huart2.Init.Parity                  =    UART_PARITY_NONE;
	huart2.Init.HwFlowCtl               =    UART_HWCONTROL_NONE;
	huart2.Init.Mode                    =    UART_MODE_TX_RX;

	if (HAL_OK != HAL_UART_Init(&huart2))
	{
		//There is a problem
		Error_handler();
	}
}


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

// uint8_t convert_to_upper(uint8_t data);


// UART_HandleTypeDef huart2;

// char *user_data = "The application is running\r\n";

// uint8_t data_buffer[100];
// uint32_t count = 0;
// uint8_t recv_data;

// bool reception_complete = false;

int main(void)
{
	HAL_Init();
	SystemClockConfig();

	GPIO_Init();
	TIMER6_Init();

	//Let's start the timer in polling mode
	if (HAL_TIM_Base_Start(&htimer6) != HAL_OK)
	{
		Error_handler();
	}

	while (1)
	{
		// loop until the update event flag is set	
		while ( !(TIM6->SR & TIM_SR_UIF) ) ;
		// clear the update event flag

		// HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		// TIM6->SR &= ~TIM_SR_UIF;

		// Toggle example
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12); // Green LED
		TIM6->SR &= ~TIM_SR_UIF;
	}
	
	// UART2_Init();

	// uint16_t len_of_data = strlen(user_data);
	// HAL_UART_Transmit(&huart2, (uint8_t*)user_data, len_of_data, HAL_MAX_DELAY);

	// while(reception_complete == false)
	// {

	// 	HAL_UART_Receive_IT(&huart2, &recv_data, 1);

	// }

	while(1) ;
	// uint8_t rcv_data;
	// uint8_t data_buffer[100];
	// uint32_t count = 0;

	// while(1)
	// {
	//     HAL_UART_Receive(&huart2, &rcv_data, 1, HAL_MAX_DELAY);

	//     if ((rcv_data != '\r') && (rcv_data != '\n'))
	//     {
	//         if (count < sizeof(data_buffer) - 1U)
	//         {
	//             data_buffer[count++] = convert_to_upper(rcv_data);
	//         }
	//     }
	//     else
	//     {
	//         uint8_t new_line[] = "\r\n";

	//         data_buffer[count] = '\0';
	//         HAL_UART_Transmit(&huart2, data_buffer, count, HAL_MAX_DELAY);
	//         HAL_UART_Transmit(&huart2, new_line, sizeof(new_line) - 1U, HAL_MAX_DELAY);
	//         count = 0;
	//     }
	// }

	return 0;
}

void SystemClockConfig(void)
{

}

// void UART2_Init()
// {
// 	huart2.Instance                     =    USART2;
// 	huart2.Init.BaudRate                =    115200;
// 	huart2.Init.WordLength              =    UART_WORDLENGTH_8B;
// 	huart2.Init.StopBits                =    UART_STOPBITS_1;
// 	huart2.Init.Parity                  =    UART_PARITY_NONE;
// 	huart2.Init.HwFlowCtl               =    UART_HWCONTROL_NONE;
// 	huart2.Init.Mode                    =    UART_MODE_TX_RX;

// 	if (HAL_OK != HAL_UART_Init(&huart2))
// 	{
// 		//There is a problem
// 		Error_handler();
// 	}
// }

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

	if (HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		Error_handler();
	}
}

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if( recv_data == '\r')
// 	{
// 		reception_complete = true;
// 		data_buffer[count++] = '\r';
// 		HAL_UART_Transmit(&huart2, data_buffer, count, HAL_MAX_DELAY);
// 	}
// 	else
// 	{
// 		data_buffer[count++]    = recv_data;
// 	}
// }

// uint8_t convert_to_upper(uint8_t data)
// {
// 	if ((data >= 'a') && (data <= 'z'))
// 	{
// 		data = data - ('a' - 'A');
// 	}
// 	return data;
// }

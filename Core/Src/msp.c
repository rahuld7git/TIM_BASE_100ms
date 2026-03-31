// msp.c
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"


/**
  * @brief  Initialize the MSP.
  * @retval None
  */
void HAL_MspInit(void)
{
  // 1. Set up priority grouping for Cortex-M.
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  // 2. Enable usage fault, memory fault and bus fault exceptions.
  SCB->SHCSR |= 0x7 << 16;

  // 3. Configure priorities for system exceptions.
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_uart;
    // Here we are going to do the low level initialisation of the USART2 peripherals

    // 1. Enable the clock for the USART2 peripheral
    __HAL_RCC_USART2_CLK_ENABLE();  
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 2. Do the pin muxing configurations
    gpio_uart.Pin       =   GPIO_PIN_2; // UART2_TX
    gpio_uart.Mode      =   GPIO_MODE_AF_PP;
    gpio_uart.Pull      =   GPIO_PULLUP;
    gpio_uart.Speed     =   GPIO_SPEED_FREQ_LOW;
    gpio_uart.Alternate =   GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &gpio_uart);

    gpio_uart.Pin       =   GPIO_PIN_3; // UART2_RX
    HAL_GPIO_Init(GPIOA, &gpio_uart);

    // 3. Enable the IRQ and set up the priority (NVIC settings)
    HAL_NVIC_EnableIRQ(USART2_IRQn);
    HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{

	GPIO_InitTypeDef timer2ch1_gpio = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	timer2ch1_gpio.Pin = GPIO_PIN_0;
	timer2ch1_gpio.Mode = GPIO_MODE_AF_PP;
	timer2ch1_gpio.Alternate = GPIO_AF1_TIM2;

	HAL_GPIO_Init(GPIOA, &timer2ch1_gpio);

	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

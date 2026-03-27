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


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer6)
{
    if (htimer6->Instance != TIM6)
    {
        return;
    }

        // Enable the clock for TIM6
        __HAL_RCC_TIM6_CLK_ENABLE();

        // Set up the NVIC for TIM6
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

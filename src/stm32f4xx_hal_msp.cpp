/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "startup_stm32f429.h"
#include "FreeRTOS.h"
#include "task.h"
#include "../lvgl/lvgl.h"

static TIM_HandleTypeDef        htim4;

void sTim4::handler()
{
	static UBaseType_t uxSavedInterruptStatus;
	uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
	HAL_TIM_IRQHandler(&htim4);
	lv_tick_inc(1);
	taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );
}

extern "C"
{
	__attribute__((used)) void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
	  if (htim->Instance == TIM4) {
		HAL_IncTick();
	  }
	}

	__attribute__((used)) void HAL_MspInit(void)
	{
		  __HAL_RCC_SYSCFG_CLK_ENABLE();
		  __HAL_RCC_PWR_CLK_ENABLE();
	}

	__attribute__((used)) HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
	{
	  RCC_ClkInitTypeDef    clkconfig;
	  uint32_t              uwTimclock, uwAPB1Prescaler = 0U;

	  uint32_t              uwPrescalerValue = 0U;
	  uint32_t              pFLatency;
	  HAL_StatusTypeDef     status;

	  /* Enable TIM4 clock */
	  __HAL_RCC_TIM4_CLK_ENABLE();

	  /* Get clock configuration */
	  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

	  /* Get APB1 prescaler */
	  uwAPB1Prescaler = clkconfig.APB1CLKDivider;
	  /* Compute TIM4 clock */
	  if (uwAPB1Prescaler == RCC_HCLK_DIV1)
	  {
		uwTimclock = HAL_RCC_GetPCLK1Freq();
	  }
	  else
	  {
		uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
	  }

	  /* Compute the prescaler value to have TIM4 counter clock equal to 1MHz */
	  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

	  /* Initialize TIM4 */
	  htim4.Instance = TIM4;

	  /* Initialize TIMx peripheral as follow:
	  + Period = [(TIM4CLK/1000) - 1]. to have a (1/1000) s time base.
	  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
	  + ClockDivision = 0
	  + Counter direction = Up
	  */
	  htim4.Init.Period = (1000000U / 1000U) - 1U;
	  htim4.Init.Prescaler = uwPrescalerValue;
	  htim4.Init.ClockDivision = 0;
	  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	  status = HAL_TIM_Base_Init(&htim4);
	  if (status == HAL_OK)
	  {
		/* Start the TIM time Base generation in interrupt mode */
		status = HAL_TIM_Base_Start_IT(&htim4);
		if (status == HAL_OK)
		{
		/* Enable the TIM4 global Interrupt */
			HAL_NVIC_EnableIRQ(TIM4_IRQn);
		  /* Configure the SysTick IRQ priority */
		  if (TickPriority < (1UL << __NVIC_PRIO_BITS))
		  {
			/* Configure the TIM IRQ priority */
			HAL_NVIC_SetPriority(TIM4_IRQn, TickPriority, 0U);
			uwTickPrio = TickPriority;
		  }
		  else
		  {
			status = HAL_ERROR;
		  }
		}
	  }

	 /* Return function status */
	  return status;
	}

	/**
	  * @brief  Suspend Tick increment.
	  * @note   Disable the tick increment by disabling TIM4 update interrupt.
	  * @param  None
	  * @retval None
	  */
	__attribute__((used)) void HAL_SuspendTick(void)
	{
	  /* Disable TIM4 update Interrupt */
	  __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_UPDATE);
	}

	/**
	  * @brief  Resume Tick increment.
	  * @note   Enable the tick increment by Enabling TIM4 update interrupt.
	  * @param  None
	  * @retval None
	  */
	__attribute__((used)) void HAL_ResumeTick(void)
	{
	  /* Enable TIM4 Update interrupt */
	  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
	}

}

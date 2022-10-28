/*
 * RCC.c
 *
 *  Created on: Oct 20, 2022
 *      Author: hp
 */
#include "RCC.h"

void RCC_CONFIG_48MHZ() {
//	if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) /* (1) */
//	{
//		RCC->CFGR &= (uint32_t) (~RCC_CFGR_SW); /* (2) */
//		while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) /* (3) */
//		{
//			/* For robust implementation, add here time-out management */
//		}
//	}
//	RCC->CR &= (uint32_t) (~RCC_CR_PLLON);/* (4) */
//	while ((RCC->CR & RCC_CR_PLLRDY) != 0) /* (5) */
//	{
//		/* For robust implementation, add here time-out management */
//	}
//	RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_PLLMUL)) | (RCC_CFGR_PLLMUL6); /* (6) */
////	RCC->CFGR = RCC_CFGR_PPRE_DIV1;
//	RCC->CR |= RCC_CR_PLLON; /* (7) */
//	while ((RCC->CR & RCC_CR_PLLRDY) == 0) /* (8) */
//	{
//		/* For robust implementation, add here time-out management */
//	}
//	RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL); /* (9) */
//	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) /* (10) */
//	{
//		/* For robust implementation, add here time-out management */
//	}
//
//	SystemCoreClockUpdate();
//	SysTick_Config(SystemCoreClock/1000);

	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
//	    Error_Handler();
//		print("Error in CLK func(1)");
//		USART_WRITE(USART2, "Error in CLK func(1)\n", 21);
		while (1)
			;
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
//	    Error_Handler();
//		print("Error in CLK func(2)");
//		USART_WRITE(USART2, "Error in CLK func(2)\n", 21);
		while (1)
			;
	}
}


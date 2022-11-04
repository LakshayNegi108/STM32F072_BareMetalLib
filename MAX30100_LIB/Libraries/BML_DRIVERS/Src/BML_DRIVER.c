/*
 * BML_GPIO.c
 *
 *  Created on: Feb 18, 2022
 *      Author: hp
 */

#include "BML_DRIVER.h"

uint32_t PINPOS_2B[16] = { //PIN POSITION MAPPER ARRAY FOR SETTING MODDER REGISTER
		(0x00),		// PIN0
				(0x02),		//
				(0x04), 	//
				(0x06), 	//
				(0x08), 	//
				(0x0A), 	//
				(0x0C), 	//
				(0x0E),		// PIN7
				(0x10),				// PIN8
				(0x12), 	//
				(0x14), 	//
				(0x16), 	//
				(0x18), 	//
				(0x1A), 	//
				(0x1C), 	//
				(0x1E), 	// PIN15

		};

uint32_t AF[8] = { (0x00), (0x01), (0x02), (0x03), (0x04), (0x05), (0x06),
		(0x07) };

uint16_t ticks = 0;

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min,
		uint16_t out_max) {
	return (((x - in_min) * (out_max - out_min)) / ((in_max - in_min) + out_min));
}

/**********************Enable clock Function*************************************/
void gpio_clk_en(GPIO_TypeDef *port) {
	if (port == GPIOA) {
		GPIOA_CLK_EN;
	} else if (port == GPIOB) {
		GPIOB_CLK_EN;
	} else if (port == GPIOC) {
		GPIOC_CLK_EN;
	} else if (port == GPIOD) {
		GPIOD_CLK_EN;
	} else {
		GPIOF_CLK_EN;
	}
}
/**********************************************************************************/

/********************GPIO Configuration Functions**********************************/
void gpio_config(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinMode,
		uint32_t pinSpeed, uint32_t pinPUPD, uint32_t PHPL_OD) {

	if (pinMode == OUTPUT_MODE) {
		gpio_clk_en(port);
		gpio_moder(port, pinNumber, pinMode);
		gpio_speed(port, pinNumber, pinSpeed);
		gpio_PUPD(port, pinNumber, pinPUPD);
		gpio_PHPL_OD(port, pinNumber, PHPL_OD);
	} else if (pinMode == INPUT_MODE) {
		gpio_clk_en(port);
		gpio_moder(port, pinNumber, pinMode);
		gpio_PUPD(port, pinNumber, pinPUPD);
	} else if (pinMode == ALT_MODE) {
		gpio_clk_en(port);
		gpio_moder(port, pinNumber, pinMode);
		gpio_PUPD(port, pinNumber, pinPUPD);
		gpio_PHPL_OD(port, pinNumber, PHPL_OD);
	} else if (pinMode == ANALOG_MODE) {
		gpio_clk_en(port);
		gpio_moder(port, pinNumber, pinMode);
		adc_chsel(port, pinNumber);
	}
}

void gpio_moder(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinMode) {	// Config the mode
	port->MODER |= (pinMode << (PINPOS_2B[pinNumber]));	// IN/OP/Alternate/ANALOG
}

void gpio_speed(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinSpeed) {// Config the speed
	port->OSPEEDR |= (pinSpeed << PINPOS_2B[pinNumber]);		// LOW/MED/HIGH
}

void gpio_PUPD(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinPUPD) {// ENABLE/DISABLE
	port->PUPDR |= (pinPUPD << PINPOS_2B[pinNumber]);		// PULL-UP/PULL-DOWN
}
void gpio_PHPL_OD(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t PHPL_OD) {// ENABLE/DISABLE
	port->OTYPER |= (PHPL_OD << pinNumber);	// PUSH-PULL/OPEN DRAIN
}

void gpio_altfn(GPIO_TypeDef *port, uint32_t pinNumber,
		uint32_t alternate_function) {
	if (pinNumber < 8) {
		port->AFR[0] |= (alternate_function << (4 * pinNumber));
	} else {
		port->AFR[1] |= (alternate_function << (4 * (pinNumber - 8)));
	}
}

/************************************************************************************/

/************************GPIO OPERATION FUNCTION*************************************/
void gpio_write(GPIO_TypeDef *port, uint32_t pinNumber, uint8_t state) {
	if (state) {
		port->BSRR = (1 << pinNumber);
	} else {
		port->BSRR = (1 << (pinNumber + 16));
	}
}

void gpio_toggle(GPIO_TypeDef *port, uint32_t pinNumber) {
	port->ODR ^= (1 << pinNumber);
}

uint8_t gpio_read(GPIO_TypeDef *port, uint32_t pinNumber) {
	uint8_t flag;
	flag = (uint8_t) ((port->IDR >> pinNumber) & 0x00000001);
	return flag;
}
/**************************************************************************************/

/*******************************INTERRUPT CONFIGURATION*********************************/
void gpio_IT_config(GPIO_TypeDef *port, uint32_t pinNumber, uint8_t edge) {
	SYSCFG_CLK_EN;
	if (port == GPIOA) {
		switch (pinNumber) {
		case 0:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PA;
			break;
		case 1:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI1_PA;
			break;
		case 2:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI2_PA;
			break;
		case 3:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI3_PA;
			break;
		case 4:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI4_PA;
			break;
		case 5:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI5_PA;
			break;
		case 6:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI6_PA;
			break;
		case 7:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI7_PA;
			break;
		case 8:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI8_PA;
			break;
		case 9:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI9_PA;
			break;
		case 10:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI10_PA;
			break;
		case 11:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI11_PA;
			break;
		case 12:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI12_PA;
			break;
		case 13:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI13_PA;
			break;
		case 14:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI14_PA;
			break;
		case 15:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI15_PA;
			break;
		}	// end of Port A
	} else if (port == GPIOB) {
		switch (pinNumber) {
		case 0:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PB;
			break;
		case 1:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI1_PB;
			break;
		case 2:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI2_PB;
			break;
		case 3:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI3_PB;
			break;
		case 4:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI4_PB;
			break;
		case 5:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI5_PB;
			break;
		case 6:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI6_PB;
			break;
		case 7:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI7_PB;
			break;
		case 8:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI8_PB;
			break;
		case 9:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI9_PB;
			break;
		case 10:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI10_PB;
			break;
		case 11:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI11_PB;
			break;
		case 12:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI12_PB;
			break;
		case 13:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI13_PB;
			break;
		case 14:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI14_PB;
			break;
		case 15:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI15_PB;
			break;
		}	// end of Port B
	} else if (port == GPIOC) {
		switch (pinNumber) {
		case 0:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PC;
			break;
		case 1:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI1_PC;
			break;
		case 2:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI2_PC;
			break;
		case 3:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI3_PC;
			break;
		case 4:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI4_PC;
			break;
		case 5:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI5_PC;
			break;
		case 6:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI6_PC;
			break;
		case 7:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI7_PC;
			break;
		case 8:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI8_PC;
			break;
		case 9:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI9_PC;
			break;
		case 10:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI10_PC;
			break;
		case 11:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI11_PC;
			break;
		case 12:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI12_PC;
			break;
		case 13:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI13_PC;
			break;
		case 14:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI14_PC;
			break;
		case 15:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI15_PC;
			break;
		}	// end of Port C
	} else if (port == GPIOD) {
		switch (pinNumber) {
		case 0:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PD;
			break;
		case 1:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI1_PD;
			break;
		case 2:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI2_PD;
			break;
		case 3:
			SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI3_PD;
			break;
		case 4:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI4_PD;
			break;
		case 5:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI5_PD;
			break;
		case 6:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI6_PD;
			break;
		case 7:
			SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI7_PD;
			break;
		case 8:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI8_PD;
			break;
		case 9:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI9_PD;
			break;
		case 10:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI10_PD;
			break;
		case 11:
			SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI11_PD;
			break;
		case 12:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI12_PD;
			break;
		case 13:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI13_PD;
			break;
		case 14:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI14_PD;
			break;
		case 15:
			SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI15_PD;
			break;
		}	// end of Port D
	}

	if (edge == RISING_EDGE) {
		EXTI->RTSR |= 1 << pinNumber;
	} else if (edge == FALLING_EDGE) {
		EXTI->FTSR |= 1 << pinNumber;
	} else if (edge == RISING_FALLING_EDGE) {
		EXTI->RTSR |= 1 << pinNumber;
		EXTI->FTSR |= 1 << pinNumber;
	}
}

void gpio_IT_EN(uint8_t pinNumber, IRQn_Type irqNumber) {//Interrupt Enable Function
	EXTI->IMR |= 1 << pinNumber;	//enable interrupt in EXTI
	NVIC_EnableIRQ(irqNumber);		//enable interrupt in NVIC
}

void gpio_IT_DI(uint8_t pinNumber, IRQn_Type irqNumber) {//Interrupt Disable Function
	EXTI->IMR &= ~(1 << pinNumber);	//disable interrupt in EXTI
	NVIC_DisableIRQ(irqNumber);
}

void gpio_IT_SW(uint8_t pinNumber) {			//Interrupt Software generate
	EXTI->SWIER |= 1 << pinNumber;				//TODO: Not Done for now
}

uint8_t gpio_IT_CHK(uint8_t pinNumber) {			//Interrupt check function
	if ((EXTI->PR) & (1 << pinNumber)) {				//Check pending register
		return 1;
	} else {
		return 0;
	}
}

void gpio_IT_CLR(uint8_t pinNumber) {				//Interrupt clear function
	EXTI->PR = 1 << pinNumber;		//clear pending register
}

// Also make a GPIO Interrupt Handler function e.g - EXTI4_15_IRQHandler

//*******************************************************************************************//






//************************** TIMER FUNCTIONS ************************************************//

//************************** Basic TIMER FUNCTIONS *******************************//
void timer_config(TIM_TypeDef *tim, uint16_t prescaler_val,
		uint16_t auto_reload_val) {

	if (tim == TIM1) {						// ENable clock for particular timer
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	} else if (tim == TIM3) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	} else if (tim == TIM6) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	} else if (tim == TIM7) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	} else if (tim == TIM14) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
	} else if (tim == TIM15) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
	} else if (tim == TIM16) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
	} else if (tim == TIM17) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
	}

	// Peripheral clock/Desired clk = PSC x ARR = Value

	tim->ARR = auto_reload_val;	// fill auto-reload register with value: auto_reload_val
	tim->PSC = prescaler_val;// fill prescale value register with value: prescaler_val

}

void timer_EN(TIM_TypeDef *tim) {
	tim->CR1 |= TIM_CR1_CEN;		// Enable counter to count the value
}

void timer_DI(TIM_TypeDef *tim) {
	tim->CR1 &= ~TIM_CR1_CEN;		// Disable counter to count the value
}

void timer6_7_CR1(TIM_TypeDef *tim, uint8_t AUTO_RELOAD_PREL_EN,// Function to configure the rest of the timer functions
		uint8_t ONE_PULSE_MODE, uint8_t UPDATE_REQ_SRC, uint8_t UPDATE_DIS) {
	if (AUTO_RELOAD_PREL_EN == 1) {
		tim->CR1 |= TIM_CR1_ARPE;
	}
	if (ONE_PULSE_MODE == 1) {
		tim->CR1 |= TIM_CR1_OPM;
	}
	if (UPDATE_REQ_SRC == 1) {
		tim->CR1 |= TIM_CR1_URS;
	}
	if (UPDATE_DIS == 1) {
		tim->CR1 |= TIM_CR1_UDIS;
	}
}

void timer_EVENT(TIM_TypeDef *tim) {
	tim->EGR |= TIM_EGR_UG;	// Generate software event to reset counter timer
}

uint16_t timer_cnt_val(TIM_TypeDef *tim) {
	uint16_t val = tim->CNT;		// counter register value
	return val;
}

void timer_IT_EN(TIM_TypeDef *tim) {
	tim->DIER |= TIM_DIER_UIE;
}

void ticks_timer(uint8_t clkSpeed) {			// similar to millis
	if (clkSpeed == 8) {
		timer_config(TIM6, 0, 7499);//Initialize timer with configuration to 1 milli second
	} else if (clkSpeed == 48) {
		timer_config(TIM6, 45, 999);//Initialize timer with configuration to 1 milli second
	}
	timer_IT_EN(TIM6);
	NVIC_EnableIRQ(TIM6_IRQn);
	timer_EN(TIM6);
}
uint16_t ticks_val() {
	return ticks;
}
void ticks_reset() {
	ticks = 0;
}
void TIM6_IRQHandler(void) {		// Timer Interrupt helping in delay
	ticks++;
	TIM6->SR &= ~TIM_SR_UIF;
}

void delay_ms(uint32_t ms) {		// blocking mode delay function
//	timer_config(TIM6, 0, 7499);
	timer_config(TIM6, 0, 47899);
//	timer_config(TIM6, 0, 208);
//	timer_config(TIM6, 23, 65534);
	timer_IT_EN(TIM6);
	NVIC_EnableIRQ(TIM6_IRQn);

	timer_EN(TIM6);	// good for turning timer off when not in need to save power
	ticks = 0;
	while (ticks < ms)
		;
	timer_DI(TIM6);
}

//***************************** SYSTICK TIMER FUNCTIONS *************************//
uint32_t sysTicks = 0;

void sysDelayInit() {
	SysTick_Config(SystemCoreClock / 1000);
}

void sysDelay(uint32_t ms) {
	sysTicks = 0;
	while (sysTicks < ms)
		;
}
void sysDelayReset() {
	sysTicks = 0;
}

uint32_t sysDelayVal() {
	return sysTicks;
}

void sysIncrementTicks() {
	sysTicks++;
}

//void SysTick_Handler(void){
//	sysIncrementTicks();
//}

//*******************************************************************************//

//************************** General Purpose TIMER FUNCTIONS ********************//

//*******************************************************************************//

//*******************************| USART FUNCTIONS |*****************************//



//*******************************************************************************//


//*******************************************************************************************//

/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           lib_timer.h
** Last modified Date:  2014-12-05
** Last Version:        V1.01
** Descriptions:        atomic functions to be used by higher sw levels
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"

/**
 * @brief Enables a specified timer and powers on timers 2 and 3 if selected
 * 
 * @param timer_num The timer number to enable (0-3)
 * 
 * @details This function enables one of the four hardware timers (Timer 0-3)
 * by writing 1 to the corresponding Timer Control Register (TCR).
 * For Timer 2 and Timer 3, it also powers them on by setting the appropriate
 * bits in the Power Control for Peripherals Register (PCONP).
 * - Timer 2: Sets bit 22 in PCONP
 * - Timer 3: Sets bit 23 in PCONP
 * 
 * @note If an invalid timer number is provided, the function returns without any action
 */
void enable_timer( uint8_t timer_num )
{
	switch (timer_num)
	{
		case 0:
			LPC_TIM0->TCR = 1;    // Enable Timer 0
			break;
		case 1:
			LPC_TIM1->TCR = 1;    // Enable Timer 1
			break;
		case 2:
			LPC_SC -> PCONP |= (1 << 22);  // Power on Timer 2 by setting bit 22 in PCONP register	
			LPC_TIM2->TCR = 1;    // Enable Timer 2	
			break;
		case 3:
			LPC_SC -> PCONP |= (1 << 23);  // Power on Timer 3 by setting bit 23 in PCONP register
			LPC_TIM3->TCR = 1;    // Enable Timer 3
			break;
		default:
			// Handle invalid timer number if necessary
			break;
	}
	return;
}

/**
 * @brief Disables and powers off specified timer
 *
 * @details This function performs two operations on the specified timer:
 *          1. Disables the timer by clearing its Timer Control Register (TCR)
 *          2. For Timer 2 & 3, also powers off the timer via PCONP register
 *
 * @param timer_num Timer to disable (0-3)
 *                  - 0: Timer 0
 *                  - 1: Timer 1 
 *                  - 2: Timer 2
 *                  - 3: Timer 3
 *
 * @note For Timer 0 & 1, only the TCR is cleared
 * @note For Timer 2 & 3, both TCR is cleared and power is disabled
 * @note If invalid timer number provided (>3), function returns with no action
 *
 * @return void
 */
void disable_timer(uint8_t timer_num)
{
	switch(timer_num)
	{
		case 0:
			LPC_TIM0->TCR = 0;            // Disable Timer 0
			break;
		case 1:
			LPC_TIM1->TCR = 0;            // Disable Timer 1
			break;
		case 2:
			LPC_TIM2->TCR = 0;            // Disable Timer 2
			LPC_SC->PCONP &= ~(1 << 22);  // Power off Timer 2
			break;
		case 3:
			LPC_TIM3->TCR = 0;            // Disable Timer 3 by clearing Timer Control Register
			LPC_SC->PCONP &= ~(1 << 23);  // Power off Timer 3
			break;
		default:
			// Handle invalid timer number if necessary 
			break;
	}
	return;
}

/**
 * @brief Resets the specified timer counter
 *
 * This function resets the Timer Counter and the Prescale Counter for the specified timer.
 * It sets bit 1 of the Timer Control Register (TCR) which triggers a reset.
 * The reset bit is automatically cleared by hardware.
 *
 * @param timer_num The timer number to reset (0-3)
 *                 - 0: Timer 0
 *                 - 1: Timer 1 
 *                 - 2: Timer 2
 *                 - 3: Timer 3
 *
 * @return None
 *
 * @note Invalid timer numbers are silently ignored
 */
void reset_timer(uint8_t timer_num)
{
	uint32_t regVal;

	switch (timer_num)
	{
		case 0:
			regVal = LPC_TIM0->TCR;
			regVal |= 0x02;
			LPC_TIM0->TCR = regVal;
			break;
		case 1:
			regVal = LPC_TIM1->TCR;
			regVal |= 0x02;
			LPC_TIM1->TCR = regVal;
			break;
		case 2:
			regVal = LPC_TIM2->TCR;
			regVal |= 0x02;
			LPC_TIM2->TCR = regVal;
			break;
		case 3:
			regVal = LPC_TIM3->TCR;
			regVal |= 0x02;
			LPC_TIM3->TCR = regVal;
			break;
		default:
			// Handle invalid timer number if necessary
			break;
	}
	return;
}

/**
 * @brief Initializes a timer with specified parameters
 *
 * @param timer_num Timer number to initialize (0-3)
 * @param prescaler Prescaler value for the timer
 * @param irq_priority Interrupt priority level for the timer
 *
 * @return 0 if initialization successful, 1 if invalid timer number
 *
 * @details This function initializes one of the LPC timers (0-3) by:
 *          - Setting the prescaler value
 *          - Enabling the timer interrupt
 *          - Setting the interrupt priority
 */
uint32_t init_timer(uint8_t timer_num, uint32_t prescaler, uint8_t irq_priority)
{
	volatile LPC_TIM_TypeDef* lpc_tim_n;
	volatile IRQn_Type irqn_tim_n;
	
	switch(timer_num) {
		case 0:
			lpc_tim_n = LPC_TIM0;
			irqn_tim_n = TIMER0_IRQn;
			break;
		case 1:
			lpc_tim_n = LPC_TIM1;
			irqn_tim_n = TIMER1_IRQn;
			break;
		case 2:
			lpc_tim_n = LPC_TIM2;
			irqn_tim_n = TIMER2_IRQn;
			break;
		case 3:
			lpc_tim_n = LPC_TIM3;
			irqn_tim_n = TIMER3_IRQn;
			break;
		default:
			return 1;
	}
	

	lpc_tim_n->PR = prescaler;
	NVIC_EnableIRQ(irqn_tim_n);
	NVIC_SetPriority(irqn_tim_n, irq_priority);
	return 0;
}

/**
 * @brief Sets up match register configuration for a specified timer
 *
 * @param timer_num Timer number (0-3) to configure
 * @param match_reg Match register number (0-3) to configure
 * @param match_val Value to load into match register
 * @param interrupt_on_match_flag Enable (1) or disable (0) interrupt on match
 * @param reset_timer_flag Enable (1) or disable (0) timer reset on match
 * @param stop_timer_flag Enable (1) or disable (0) timer stop on match
 *
 * @details This function configures a match register for the specified timer.
 * It sets up the match value and control flags for interrupt, reset and stop
 * functionality when a match occurs.
 *
 * The match control register (MCR) is configured with the following bits:
 * - Bit 0: Interrupt on match
 * - Bit 1: Reset timer on match
 * - Bit 2: Stop timer on match
 *
 * @note Valid timer numbers are 0-3
 * @note Valid match register numbers are 0-3
 * @note Each control flag should be either 0 or 1
 */
void set_match_register(uint8_t timer_num, uint8_t match_reg, uint32_t match_val, 
					   uint8_t interrupt_on_match_flag, uint8_t reset_timer_flag, uint8_t stop_timer_flag)
{
	LPC_TIM_TypeDef* lpc_tim_n;
	uint8_t match_control = 0;
	
	switch(timer_num) {
		case 0:
			lpc_tim_n = LPC_TIM0;
			break;
		case 1:
			lpc_tim_n = LPC_TIM1;
			break;
		case 2:
			lpc_tim_n = LPC_TIM2;
			break;
		case 3:
			lpc_tim_n = LPC_TIM3;
			break;
		default:
			return;
	}
	
//	// Manual factor scaling for registers and match value
//	#ifdef SIMULATTOR
//    match_val = match_val >> SCALING_FACTOR_TIMER;
//	#endif

	// Construct match control value
	match_control = (stop_timer_flag & 0x1) |
				   ((reset_timer_flag & 0x1) << 1) |
				   ((interrupt_on_match_flag & 0x1) << 2);

	switch(match_reg) {
		case 0:
			lpc_tim_n->MR0 = match_val;
			lpc_tim_n->MCR &= ~(7 << (3 * 0));    // Clear existing bits
			lpc_tim_n->MCR |= match_control << (3 * 0);
			break;
		case 1:
			lpc_tim_n->MR1 = match_val;
			lpc_tim_n->MCR &= ~(7 << (3 * 1));
			lpc_tim_n->MCR |= match_control << (3 * 1);
			break;
		case 2:
			lpc_tim_n->MR2 = match_val;
			lpc_tim_n->MCR &= ~(7 << (3 * 2));
			lpc_tim_n->MCR |= match_control << (3 * 2);
			break;
		case 3:
			lpc_tim_n->MR3 = match_val;
			lpc_tim_n->MCR &= ~(7 << (3 * 3));
			lpc_tim_n->MCR |= match_control << (3 * 3);
			break;
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/
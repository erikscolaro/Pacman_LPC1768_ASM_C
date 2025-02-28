#include "button.h"
#include "LPC17xx.h"

/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {
	/* Button 0 initialization (INT0 - P2.10) */
	LPC_PINCON->PINSEL4 &= ~(1 << 21);     /* Clear bit 21 */
	LPC_PINCON->PINSEL4 |= (1 << 20);      /* Set bit 20 - External interrupt 0 */
	LPC_GPIO2->FIODIR   &= ~(1 << 10);     /* PORT2.10 defined as input */

	/* Button 1 initialization (INT1 - P2.11) */
	LPC_PINCON->PINSEL4 &= ~(1 << 23);     /* Clear bit 23 */
	LPC_PINCON->PINSEL4 |= (1 << 22);      /* Set bit 22 - External interrupt 1 */
	LPC_GPIO2->FIODIR   &= ~(1 << 11);     /* PORT2.11 defined as input */
	
	/* Button 2 initialization (INT2 - P2.12) */
	LPC_PINCON->PINSEL4 &= ~(1 << 25);     /* Clear bit 25 */
	LPC_PINCON->PINSEL4 |= (1 << 24);      /* Set bit 24 - External interrupt 2 */
	LPC_GPIO2->FIODIR   &= ~(1 << 12);     /* PORT2.12 defined as input */

	/* Configure all buttons as edge-sensitive*/
	LPC_SC->EXTMODE = 0x7;

	NVIC_EnableIRQ(EINT2_IRQn);            /* enable irq in nvic */
	NVIC_SetPriority(EINT2_IRQn, 1);       /* priority, the lower the better */
	NVIC_EnableIRQ(EINT1_IRQn);            /* enable irq in nvic */
	NVIC_SetPriority(EINT1_IRQn, 2);       
	NVIC_EnableIRQ(EINT0_IRQn);            /* enable irq in nvic */
	NVIC_SetPriority(EINT0_IRQn, 3);       /* decreasing priority from EINT2->0 */
}

/**
 * @brief Initializes a single button with specified parameters
 * @param button_num The button number (0, 1, or 2)
 * @param handler_priority Priority for the interrupt handler (lower number = higher priority)
 * @param trigger_on_edge 1 for edge-triggered, 0 for level-triggered
 */
void single_button_init(uint8_t button_num, uint8_t handler_priority, uint8_t trigger_on_edge) {
	IRQn_Type irqn;

	switch(button_num) {
		case 0:
			irqn = EINT0_IRQn;
			break;
		case 1:
			irqn = EINT1_IRQn;
			break;
		case 2:
			irqn = EINT2_IRQn;
			break;
	}

	LPC_PINCON->PINSEL4 &= ~(1 << (21 + 2*button_num));   
	LPC_PINCON->PINSEL4 |= (1 << (20 + 2*button_num));
	LPC_GPIO2->FIODIR &= ~(1 << (10 + button_num));
	// disable NVIC functions if you are polling the Joystick!
	NVIC_EnableIRQ(irqn);
	NVIC_SetPriority(irqn, handler_priority);
	LPC_SC->EXTMODE &= ~(1 << button_num);
	LPC_SC->EXTMODE |= (trigger_on_edge << button_num);
}

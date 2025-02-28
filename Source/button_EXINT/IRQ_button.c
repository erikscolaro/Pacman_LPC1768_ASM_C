#include "button.h"
#include "LPC17xx.h"
#include "RIT/RIT.h"

volatile KeyStatus keyStatus = {0};

/*
 * Enable these interrupts if you only have to manage buttons 
 * with interrupts + rit withoust polling the joystick 
 */

void EINT0_IRQHandler (void)        // External Interrupt 0
{
  
	keyStatus.key_0=1;  
	NVIC_DisableIRQ(EINT0_IRQn);    // disable Button interrupts
  // Set EINT0 pin as GPIO by zeroing bits 20-21 in PINCON->PINSEL4
  LPC_PINCON->PINSEL4 &= ~(3 << 20); // Set EINT0 pin as GPIO
	LPC_GPIO2->FIODIR &= ~(1 << 10); // Configura P2.10 come input
  LPC_SC->EXTINT &= (1 << 0);     // clear pending interrupt
		
}

void EINT1_IRQHandler (void)        // External Interrupt 1
{
    LPC_SC->EXTINT &= (1 << 1);     // clear pending interrupt
}

void EINT2_IRQHandler (void)        // External Interrupt 2
{
    LPC_SC->EXTINT &= (1 << 2);     // clear pending interrupt
}
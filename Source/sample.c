/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: 
 *		to control led11 and led 10 through EINT buttons (similarly to project 03_)
 *		to control leds9 to led4 by the timer handler (1 second - circular cycling)
 * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/

                  
#include <stdio.h>
#include "LPC17xx.h"                    /* LPC17xx definitions                */
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "speaker/speaker.h"
#include "GLCD/GLCD.h"
#include "CAN/CAN.h"

#ifdef SIMULATTOR
	extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

/*----------------------------------------------------------------------------
  Main Program
  *----------------------------------------------------------------------------*/
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	init_timer(0, 0, 1);                        // Timer for screen refresh
	init_timer(1, 0, 2);                        // Timer for countdown in game
	init_timer(2, 0, 3); 												// timer for pacman hunting mode	
	init_timer(3, 0, 0); 												// timer for music	
	joystick_init();
	CAN_Init();
	initSpeaker();
	
	init_RIT(MS_TO_CC(100000000,50)); 
	enable_RIT(); 
	LCD_Initialization();
	gameWaiting();
	
	single_button_init(0,0,1);           // Initialize button 0 IRQ
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= 0xFFFFFFFFD;						
		
	while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
	}

}
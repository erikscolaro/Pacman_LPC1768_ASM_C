/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "button.h"
#include "joystick/joystick.h" 
#include "GLCD/GLCD.h"
#include "characters/player.h"
#include "button_EXINT/button.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{			
	/*************************KEY0***************************/
	if(keyStatus.key_0 !=0){
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
			keyStatus.key_0++;
			switch(keyStatus.key_0){
				case 3:
																	switch (getStatus()) {
																			case WAITING:
																					updateStatus(PLAYING);
																					break;
																			case PLAYING:
																					updateStatus(PAUSED);
																					break;
																			case PAUSED:
																					updateStatus(RESUMED);
																					break;
																			case OVER:
																					updateStatus(WAITING);
																					break;
																			case WON:
																					updateStatus(WAITING);
																					break;
																			default:
																					break;
																		}
					break;
				default:
					break;
			}
		}	else {	/* button released */
			keyStatus.key_0=0;			
			//NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
			//LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
			single_button_init(0,0,1);
		}
	} // end INT0

	/*************************JOYSTICK***************************/

	/****UP****/
	if ((LPC_GPIO1->FIOPIN & (1<<29)) == 0) {
			/* Joystick Up Pressed */
			joystick.up++;
			switch (joystick.up) {
					case 1:
							/* first pression, debouncing not needed because done in hw*/
							setDirection(&pacman, UP);
							break;
					default:
							break;
			}
	} else {
			/* Joystick up released */
			joystick.up = 0;
	}

	/****RIGHT****/
	if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
			/* Joystick Right Pressed */
			joystick.right++;
			switch (joystick.right) {
					case 1:
							/* first pression, debouncing not needed because done in hw*/
							setDirection(&pacman, RIGHT);
							break;
					default:
							break;
			}
	} else {
			/* Joystick right released */
			joystick.right = 0;
	}

	/****DOWN****/
	if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
			/* Joystick Down Pressed */
			joystick.down++;
			switch (joystick.down) {
					case 1:
							/* first pression, debouncing not needed because done in hw*/
							setDirection(&pacman, DOWN);
							break;
					default:
							break;
			}
	} else {
			/* Joystick down released */
			joystick.down = 0;
	}

	/****LEFT****/
	if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {
			/* Joystick Left Pressed */
			joystick.left++;
			switch (joystick.left) {
					case 1:
							/* first pression, debouncing not needed because done in hw*/
							setDirection(&pacman, LEFT);
							break;
					default:
							break;
			}
	} else {
			/* Joystick left released */
			joystick.left = 0;
	}

	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

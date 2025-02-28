/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           Joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"

volatile JoystickState joystick = {0};

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
    // Joystick Select (P1.25)
    LPC_PINCON->PINSEL3 &= ~(3<<18);   // Set P1.25 as GPIO (bits 18-19)
    LPC_GPIO1->FIODIR   &= ~(1<<25);   // P1.25 as input

    // Joystick Left (P1.26)
    LPC_PINCON->PINSEL3 &= ~(3<<20);   // Set P1.26 as GPIO (bits 20-21)
    LPC_GPIO1->FIODIR   &= ~(1<<26);   // P1.26 as input

    // Joystick Down (P1.27)
    LPC_PINCON->PINSEL3 &= ~(3<<22);   // Set P1.27 as GPIO (bits 22-23)
    LPC_GPIO1->FIODIR   &= ~(1<<27);   // P1.27 as input

    // Joystick Right (P1.28)
    LPC_PINCON->PINSEL3 &= ~(3<<24);   // Set P1.28 as GPIO (bits 24-25)
    LPC_GPIO1->FIODIR   &= ~(1<<28);   // P1.28 as input

    // Joystick Up (P1.29)
    LPC_PINCON->PINSEL3 &= ~(3<<26);   // Set P1.29 as GPIO (bits 26-27)
    LPC_GPIO1->FIODIR   &= ~(1<<29);   // P1.29 as input
}



/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "adc.h"
#include "GLCD/GLCD.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

volatile unsigned short AD_current;   
volatile unsigned short AD_last = 0xFF;     /* Last converted value               */

void ADC_IRQHandler(void) {
  	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){
		setSeed(AD_current);
  }	
	NVIC_DisableIRQ(ADC_IRQn);             /* enable ADC Interrupt               */
}

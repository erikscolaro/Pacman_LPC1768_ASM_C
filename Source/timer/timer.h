/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           timer.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_timer, funct_timer, IRQ_timer .c files
** Correlated files:    lib_timer.c, funct_timer.c, IRQ_timer.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

/* Utility macro that converts milliseconds to timer counter counts based on frequency */
#define MS_TO_CC(freq_hz, time_ms) ((freq_hz/1000) * time_ms) 
/* Converts PWM duty cycle percent to clock cycles for match/compare register */
#define PWM_TO_CC(period_cc, duty_cycle_percent) ((period_cc * duty_cycle_percent) / 100)

/* init_timer.c */
uint32_t init_timer(uint8_t timer_num, uint32_t prescaler, uint8_t irq_priority);
extern void set_match_register(uint8_t timer_num, uint8_t match_reg, uint32_t match_val, uint8_t stop_timer_flag,  uint8_t reset_timer_flag, uint8_t interrupt_on_match_flag);
extern void enable_timer(uint8_t timer_num);
extern void disable_timer( uint8_t timer_num );
extern void reset_timer( uint8_t timer_num );
/* IRQ_timer.c */
extern void TIMER0_IRQHandler (void);
extern void TIMER1_IRQHandler (void);
extern void TIMER2_IRQHandler (void);
extern void TIMER3_IRQHandler (void);


#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/

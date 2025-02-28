#ifndef __BUTTON_H
#define __BUTTON_H

#include <stdint.h>

//void BUTTON_init(void);

typedef struct {
    volatile int key_0;
    volatile int key_1;
    volatile int key_2;
} KeyStatus;

extern volatile KeyStatus keyStatus;

void EINT0_IRQHandler(void);
void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);

void single_button_init(uint8_t button_num, uint8_t handler_priority, uint8_t trigger_on_edge);

#endif /* end __BUTTON_H */
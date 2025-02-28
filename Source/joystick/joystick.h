/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           Joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_joystick, funct_joystick .c files
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/
#ifndef JOYSTICK_H
#define JOYSTICK_H
typedef struct {
    volatile int sel;
    volatile int up;
    volatile int right;
    volatile int down;
    volatile int left;
} JoystickState;

extern volatile JoystickState joystick;

/* lib_joystick */
void joystick_init(void);

#endif /* JOYSTICK_H */
#ifndef JOYSTICKCONTROL_H
#define JOYSTICKCONTROL_H

typedef enum {LEFT, RIGHT, UP, DOWN, CENTER, INVALID} Joy_Direction;


/*
Export all joystick gpio pins
input: void
output: void;
*/
void initializeJoyStick();

/*
Return the direction of joystick pressed
input: void
output: enum
*/
Joy_Direction getDirections(void);

#endif
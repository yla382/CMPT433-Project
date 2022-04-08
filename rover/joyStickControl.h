#ifndef JOYSTICKCONTROL_H
#define JOYSTICKCONTROL_H


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
void getDirections(void);

#endif
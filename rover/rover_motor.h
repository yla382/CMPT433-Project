#ifndef ROVER_MOTOR_H
#define ROVER_MOTOR_H

void initGpioMotor(void);

void turnAllMotors(void);
void turnOffMotors(void);
void rotateLeftMotors(void);
void rotateRightMotors(void);

void sleep_ms(unsigned int delayMs);

#endif
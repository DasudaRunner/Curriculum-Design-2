#ifndef __MOTOR_H_
#define __MOTOR_H_

#define uchar unsigned char
#define uint unsigned int

extern uchar pwm[2];
extern char MotorOut;

#define SPEED_MAX 80

void go();
void left();
void stop();
char Amplitude(uchar dat,uchar limit);
char speedControl(uchar speed,uchar target);
#endif
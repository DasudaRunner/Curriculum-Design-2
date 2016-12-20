#ifndef __PACKAGE_H_
#define __PACKAGE_H_

#include "reg51.h"

#define uchar unsigned char
#define uint unsigned int

#define SEND_MAX 15 //每次发送最大帧长度

extern uchar send_flag;
extern uchar receive_ok;
extern uchar receive_data[SEND_MAX];
extern uchar ConfigSpeed;

char InPackage(unsigned char *usart_data,unsigned char ID);
unsigned char DePackage(void);
char check_data(void);
#endif
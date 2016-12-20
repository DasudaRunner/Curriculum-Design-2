#include "encoder.h"


uint PreSpeed[4]={0x30,0x30,0x30,'\0'};

void GetSpeed(void)
{
	uint sp;
	sp=speed_tem;
	
	//不用在这里处理，中断中尽量减少运算
//	PreSpeed[0]=speed_tem/20;
//	PreSpeed[1]=speed_tem%20;
	

}
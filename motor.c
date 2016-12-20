/*
			地盘电机控制
			驱动方式：PWM
			
			PID部分
			PD控制 积分控制不明显，静态误差小，影响因素决定
			
			2016.11.30 删除PID调节，效果差，原因：速度反馈不可靠
			
			MADE BY 王海波
*/

#include "motor.h"

uchar pwm[2]={10,1};

char MotorOut=0;
//pwm[0]:表示正反转	    0:正转   1:反转	    

float Kp=5,Ki=0,Kd=1;

char Amplitude(uchar dat,uchar limit)
{
	if(dat>=limit)
	{
		return limit;
	}else if(dat <= (-limit))
	{
		return (-limit);
	}else
	{
		return dat;
	}
}


void go()
{
	pwm[0]=MotorOut;
	pwm[1]=1;
}
void left()
{
	pwm[0]=MotorOut;
	pwm[1]=0;
}
void stop()
{
	pwm[0]=0;
}

char speedControl(uchar speed,uchar target)
{
	int p_part,i_part,d_part,out;
	static char last_Bias=0;
	
	char motor_out,Bias,Dev_Bias;
	
	Bias = target-speed;	
	Dev_Bias = Bias-last_Bias;
	//PID计算
	p_part = Bias*Kp/10;	
	//i_part +=Bias*Ki;
	d_part =Dev_Bias*Kd;
	
	last_Bias = Bias;
	
	out = p_part+d_part;

	if(out>=SPEED_MAX)
	{
		out = SPEED_MAX;
	}else if(out<=(-SPEED_MAX))
	{
		out = -SPEED_MAX;
	}
	//输出值整定
	out=out/6;
//	motor_out=out;
	return out;
}





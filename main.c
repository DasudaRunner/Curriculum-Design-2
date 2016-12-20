//板1：人机交互板
//板2：地盘控制板
#include "reg51.h"
#include "package.h"
#include "motor.h"
/*
2016.11.08 程序框架
2016.11.15 完成协议的封装
2016.11.16 完成板1部分细节功能
2016.11.19 完成接收地盘速度信息功能
2016.11.20 完成矩阵键盘部分
2016.11.22 完成细节 LED及BUZZER
2016.11.23 完成速度设定功能
2016.11.25-2016.11.30 两板联调 PID调节

测速接INT0
按键接INT1
*/
#define uchar unsigned char
#define uint unsigned int

sbit buzzer=P2^5;
sbit red_led=P2^6;
sbit green_led=P2^7;
//对管接口
sbit Dec0=P1^4;
sbit Dec1=P1^5;
sbit Dec2=P1^6;
sbit Dec3=P1^7;
//电机使能口
sbit IN1=P0^0;
sbit IN2=P0^1;
sbit IN3=P0^2;
sbit IN4=P0^3;

sbit IN5=P0^4;
sbit IN6=P0^5;
sbit IN7=P0^6;
sbit IN8=P0^7;


uint dec_cnt=0;
uchar upFlag=0,downFlag=0,can_go=1;
uchar speed_tem,speed_read=0;
uchar now_speed[3]={0x00,0x30,0x30};
//滤波器
uchar SpeedFilter[5]={0,0,0,0,0};
void delay_1(int time)
{

	int a,b;
	for(a=time;a>0;a--)
	{
		for(b=125;b>0;b--);
	}

}

uchar toggleBit(uchar dat)
{
	if(dat==1)
	{
		return 0;
	}else if(dat==0)
	{
		return 1;
	}
}

void INT_init()
{
	IT0=1;
	IT1=1;
	EX0=1;
	EX1=1;
	EA=1;
}
void TimerConfig()
{
	TMOD = 0x21;
		
	TH0 = (65536 - 50000)/256;
	TL0 = (65536 - 50000)%256;
	
	TR0=1;
	ET0=1;
	EA=1;
}
void UsartConfiguration()
{
	SCON=0X50;			//串口工作方式2
	TMOD=0X21;			
	PCON=0X80;			
	TH1=0XF3;				//baud 4800
	TL1=0XF3;
	ES=1;						
	EA=1;						
	TR1=1;					
}
void main(void)
{
	INT_init();
	TimerConfig();
	UsartConfiguration();
	while(1)
	{				
		if(receive_ok)
		{
			receive_ok=0;
			if(check_data())
			{
				switch(receive_data[1])
				{
					case 0x01:{
						ConfigSpeed=receive_data[4]*10+receive_data[5];	
						MotorOut=ConfigSpeed;
						break;}
				}
			}
		}
		
		#if 1
		if(speed_read)
		{
			speed_read=0;

			now_speed[1]=now_speed[0]%100/10;
			now_speed[2]=now_speed[0]%10;
			now_speed[0]=now_speed[0]/100;

			InPackage(now_speed,0x02);
		}
		#endif
	
		#if 1
		//--------------------------
		if((upFlag==0)&&(downFlag==0))//主线
		{
			if((Dec0==1)||(Dec1==1))
			{
				go();
			}else
			{
				left();
			}
		}
		//装货区
		if((Dec2==1)&&(can_go==1))//中线
		{
			stop();
			upFlag=1;
		}
		//卸货区
		if((Dec3==1)&&(can_go==0))//右线
		{
			stop();
			downFlag=1;
		}
		#endif	
		//-------------------------------	
	}	
}
//装货、卸货按键
void int1_SERVICE() interrupt 2
{
	if((upFlag==1)||(downFlag==1))//只有在卸货区或者装货区才能触发，防止运货过程中误触
	{
		can_go=toggleBit(can_go);
		upFlag=0;
		downFlag=0;
	}
}
//读取光电门脉冲
void int0_SERVICE() interrupt 0
{
	speed_tem++;
}

void Timer0_SERVICE()	interrupt 1 using 3
{	
	static uint motor_cnt,encoder_cnt,speed_read_cnt,FilterCnt=0,add=0,pwm_cnt;
	uchar i;
	TH0 = (65536 - 5000)/256; //50ms
	TL0 = (65536 - 5000)%256;

	motor_cnt++;
	encoder_cnt++;
	speed_read_cnt++;
	//滑动滤波
	//----------------------------------------------//
	if(speed_read_cnt>100)
	{
		SpeedFilter[FilterCnt]=speed_tem;
		speed_tem=0;
		speed_read_cnt=0;
		if(FilterCnt>=4)
		{
			FilterCnt=0;
		}else
		{
			FilterCnt++;
		}
	}
	//-----------------------------------------------//
	if(encoder_cnt>100)
	{
		for(i=0;i<5;i++)
		{
			add+=SpeedFilter[i];
		}
		now_speed[0]=add*2/5;
		encoder_cnt=0;
		speed_read=1;
		add=0;
		
		MotorOut += speedControl(now_speed[0],ConfigSpeed);
		if(MotorOut>=20)
		{
			MotorOut=20;
		}
		if(MotorOut<=0)
		{
			MotorOut=0;
		}
	}	

		//MotorOut=Amplitude(MotorOut,20);
	
	/*
	     以下PWM程序
	*/
	if(pwm[0]==0)
	{
		IN1=0;
		IN2=0;
		IN3=0;
		IN4=0;	
		IN5=0;
		IN6=0;		
		IN7=0;
		IN8=0;
	}else
	{
		//1	 2
		if(motor_cnt>pwm[0])
		{
			IN1=0;
			IN2=0;		
			IN5=0;
			IN6=0;
		}else
		{
			IN1=1;
			IN2=0;
			IN5=1;
			IN6=0;
		}
		
		switch(pwm[1])//0  反转
		{
		case 0:{
			 if(motor_cnt>pwm[0])
			{
				IN3=0;
				IN4=0;	
				IN7=0;
				IN8=0;
			}else
			{
				IN3=0;
				IN4=1;			
				IN7=0;
				IN8=1;
			}
			break;}
		case 1:{		 
			 if(motor_cnt>pwm[0])
			{
				IN3=0;
				IN4=0;	
				IN7=0;
				IN8=0;
			}else
			{
				IN3=1;
				IN4=0;			
				IN7=1;
				IN8=0;
			}
			break;}
		}

	}	

	// motor_cnt 清零
	if(motor_cnt>=20)
	{
		motor_cnt=0;
	}
}
void Usart_SERVICE() interrupt 4
{
	if(RI)
	{
		RI = 0;
		DePackage();
	}else if (TI)
	{
		TI = 0;
		send_flag=0;
	}
					
}
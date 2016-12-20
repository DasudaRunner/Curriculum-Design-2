/*
																51�������ݷ���
															���ܣ���װ֡+���֡
															���ڣ�2016.11.25 
																MADE BY ������
*/
#include "package.h"
uchar send_flag;
uchar time_out;

uchar receive_data[SEND_MAX];
uchar receive_ok;

uchar ConfigSpeed=20;
//��������
char InPackage(unsigned char *usart_data,unsigned char ID)
{
	uchar send_data[SEND_MAX],res=0,i,length,head_check;
	static uchar cnt=0;
	
	switch(ID)
	{
		case 0x01:length=6;head_check=0x0b;break;//�ٶ��趨
		case 0x02:length=7;head_check=0x0c;break;//
	}
	
	send_data[0]=0x0a;
	send_data[1]=ID;//����ID
	send_data[2]=head_check;//֡ͷУ��λ
	send_data[3]=length-4;//����֡����
	for(i=0;i<(length-4);i++)
	{
		send_data[4+i]=usart_data[i];
		res+=usart_data[i];
	}	
	send_data[length]=res;//����֡У��λ
	
	cnt=0;
	while(cnt<=length)
	{
		if(!send_flag)
		{
			time_out=0;
			send_flag=1;
			SBUF=send_data[cnt];			
			cnt++;
		}
		time_out++;
		if(time_out>=1000)  //��ʱ����
		{
			time_out=0;
			return (-1);
		}
	}
	return 1;
}
//�������
unsigned char DePackage(void)
{
	static uchar count=0;
	uchar i,res=0;
	receive_data[count]=SBUF;
	if(receive_data[0]==0x0a)
	{
		if(count>=3)
		{
			if(count>=(receive_data[3]+4))
			{
				count=0;
				receive_ok=1;
				return 1;
			}
		}
		count++;
	}
	
}
//У������
char check_data(void)
{
	uchar res,i;
	if(receive_data[2]==(receive_data[0]+receive_data[1]))
	{
		for(i=0;i<receive_data[3];i++)
		{
			res+=receive_data[i+4];
		}
		if(res==receive_data[4+receive_data[3]])
		{
			return 1;
		}else
		{
			return (-1);
		}
	}else
	{
		return (-1);
	}	
	
}







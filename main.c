#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int  uint16;

extern void LcdInit();
extern void ConfigUart(uint16 baud);
extern void E2pReadDat(uint8 addr,uint8 *dat,uint8 len);
extern UartDriver();

void ConfigTimer0(uint8 ms);

uint8 T0RH,T0RL;
uint8 pdata lcdStr1[17];
uint8 pdata lcdStr2[17];


void main()
{
	LcdInit();
	ConfigUart(2400);
	ConfigTimer0(1);
	EA=1;
	E2pReadDat(0x05,lcdStr1,16);
	E2pReadDat(0x80,lcdStr2,16);
	LcdShowText(0,0,lcdStr1,16);
	LcdShowText(0,1,lcdStr2,16);
	while(1)
	{
		UartDriver();
	}
}

bit cmpStr(uint8 *str1,uint8 *str2,uint8 len)
{
	while(len--)
	{
		if(*str1!=*str2)
			return 0;
		else
		{
			str1++;str2++;
		}
	}
	return 1;
}


void UartAction(uint8 *str,uint8 len)
{
	uint8 code cmdStr1[]="showStr1 ";
	uint8 code cmdStr2[]="showStr2 ";
	uint8 code cmdFbk[]="bad command!";
	uint8 *allCmd[]={cmdStr1,cmdStr2};
	for(i=0;i<sizeof(allCmd);i++)
	{
		if(cmpStr(allCmd[i],str,sizeof(cmdStr1)-1))
		break;
	}
	switch (i)
	{
		case 0:
		case 1:
				if(len>16)
					len=16;
				while(len--)
				{
					if(i==0)
					{
						*lcdStr1=*(str+sizeof(cmdStr1)-1)
						lcdStr1++;
						str++;
					}
					if(i==1)
					{
					*lcdStr2=*(str+sizeof(cmdStr2)-1)
						lcdStr2++;
						str++;
					}
				}
				for(j=0;j<16-len;j++)
				{
					if(i==0)
					{
						*lcdStr1++=' ';
					}
					else
						*lcdStr2++=' ';
				}
				if(i==0)
				{
					lcdStr1-=16;
					UartSendStr(lcdStr1,len);
					LcdShowText(0,0,lcdStr1,16);
					E2pWriteDat(0x05,lcdStr1,16);
				}
				else if(i==1)
				{
					lcdStr2-=16;
					UartSendStr(lcdStr2,len);
					LcdShowText(0,1,lcdStr2,16);
					E2pWriteDat(0x80,lcdStr2,16);
				}
		default:UartSendStr(cmdFbk,sizeof(cmdFbk);break;
	}
}


void ConfigTimer0(uint8 ms)
{
	uint16 load;
	load=65536-((SYS_OSC/SYS_T)/1000)*ms;
	TMOD&=0xf0;
	TMOD|=0x01;
	T0RH=load>>8;
	T0RL=load;
	TH0=T0RH;
	TL0=T0RL;
	TR0=1;
	ET0=1;
}



void Timer0Interrupt() imterrupt 1
{
	TH0=T0RH;
	TL0=T0RL;
	UartCheckEnd();
}
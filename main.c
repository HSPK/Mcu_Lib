#include <reg52.h>
#define SYS_OSC 12000000
#define SYS_T 12
typedef unsigned char uint8;
typedef unsigned int  uint16;

void ConfigTimer0(uint8 ms);
void LcdShowInit();
extern void LcdInit();
extern void ConfigUart(uint16 baud);
extern void E2pReadDat(uint8 addr,uint8 *dat,uint8 len);
extern void E2pWriteDat(uint8 addr,uint8 *dat,uint8 len);
extern void LcdShowText(uint8 x,uint8 y,uint8 *dat,uint8 len);
extern void UartDriver();
extern void UartSendStr(unsigned char *str,unsigned char len);
extern void UartCheckEnd(unsigned char ms);


uint8 T0RH,T0RL;



void main()
{
	LcdInit();
	ConfigUart(2400);
	ConfigTimer0(1);
	EA=1;
	LcdShowInit();
	while(1)
	{
		UartDriver();
	}
}

void LcdShowInit()
{
	uint8 buf[16];
	E2pReadDat(0x20,buf,16);
	LcdShowText(0,0,buf,16);
	E2pReadDat(0x80,buf,16);
	LcdShowText(0,1,buf,16);
}
bit CmpStr(uint8 *str1,uint8 *str2,uint8 len)
{
	while(len--)
	{
		if(*str1++!=*str2++)
			return 0;
	}
	return 1;
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


void StrToken(uint8 *src,uint8 *aim,uint8 start,uint8 len)
{
	uint8 i;
	for(i=0;i<len;i++)
	{
		if(*(src+i+start)!='\0')
			*(aim+i)=*(src+i+start);
		else
			break;
	}
	for(;i<len;i++)
	{
		*(aim+i)=' ';
	}
}


void UartAction(uint8 *str,uint8 len)
{
	uint8 code cmdStr1[]="showStr1 ";
	uint8 code cmdStr2[]="showStr2 ";
	uint8 code cmdFbk[]="bad command!\n";
	uint8 i;
	uint8 *allCmd[]={cmdStr1,cmdStr2};
	uint8  buf[17];

	for(i=0;i<sizeof(allCmd);i++)
	{
		if(CmpStr(allCmd[i],str,sizeof(cmdStr1)-1))
		break;
	}
	switch (i)
	{
		case 0:
		{
			*(str+len)='\0';
			StrToken(str,buf,sizeof(cmdStr1)-1,16);
			LcdShowText(0,0,buf,16);
			E2pWriteDat(0x20,buf,16);
			buf[16]='\n';
			UartSendStr(buf,sizeof(buf));
			break;
		}
		case 1:
		{
			*(str+len)='\0';
			StrToken(str,buf,sizeof(cmdStr2)-1,16);
			LcdShowText(0,1,buf,16);
			E2pWriteDat(0x80,buf,16);
			buf[16]='\n';
			UartSendStr(buf,sizeof(buf));
			break;
		}	
		default:UartSendStr(cmdFbk,sizeof(cmdFbk));break;
	}
}



void Timer0Interrupt() interrupt 1
{
	TH0=T0RH;
	TL0=T0RL;
	UartCheckEnd(1);
}
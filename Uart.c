/**

			UART���֡ͨ��
			by WAY
			v0.0.0 20170710

*/


#include <reg52.h>
#define SYS_OSC 12000000
#define SYS_T 12
bit strEndFlag=0;
bit txdFlag=0;
unsigned char pdata rxdBuf[40];
unsigned char cntRxd=0;

extern void UartAction(unsigned char *str,unsigned char len);

void ConfigUart(unsigned int baud)
{
	unsigned char load;
	SCON=0x50;
	PCON=0x00;
	ES=1;
	load=256-(SYS_OSC/SYS_T)/32/baud;
	TMOD&=0x0f;
	TMOD|=0x20;
	TH1=load;	   
	TL1=TH1;
	TR1=1;
	ET1=0;
}

unsigned char UartReadStr(unsigned char *buf,unsigned char len)
{
		unsigned char i;
		if(len>cntRxd)
			len=cntRxd;
		for(i=0;i<len;i++)
		{
			*buf++=rxdBuf[i];
		}
		cntRxd=0;
		return len;
}

void UartCheckEnd(unsigned char ms)
{
	static unsigned char cntBkp=0;
	static unsigned char idleTime=0;
	if(cntRxd>0)
	{
		if(cntBkp!=cntRxd)
		{
			idleTime=0;
			cntBkp=cntRxd;
		}
		else
		{
			if(idleTime<30)
			{
				idleTime+=ms;
				if(idleTime>=30)
					strEndFlag=1;
			}
		}
	}
	else
		cntBkp=0;
}


void UartSendStr(unsigned char *str,unsigned char len)
{
		while(len--)
		{
			txdFlag=0;
			SBUF=*str++;
			while(!txdFlag);
		}
}





void UartDriver()
{
	unsigned char len;
	unsigned char pdata buf[40];
	if(strEndFlag)
	{
		strEndFlag=0;
		len=UartReadStr(buf,sizeof(buf));
		UartAction(buf,len);
	}
}

void UartInterrupt() interrupt 4
{
	if(RI)
	{
		RI=0;
		if(cntRxd<sizeof(rxdBuf))
			*(rxdBuf+cntRxd++)=SBUF;
	}
	if(TI)
	{
		TI=0;
		txdFlag=1;
	}
}
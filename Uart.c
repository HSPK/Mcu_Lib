/**

			UART���֡ͨ��
			by WAY
			v0.0.0 20170710

*/


#include <reg52.h>
#define SYS_OSC 12000000
#define SYS_T 12

extern void UartAction(unsigned char *str,unsigned char len);
bit strEndFlag=0;
bit txdFlag=0;
unsigned char padta rxdBuf[40];
unsigned char cntRxd=0;

void ConfigUart(uint16 baud)
{
	uint8 load;
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

unsigned char UartReadStr(*buf,unsigned char len)
{
		unsigned char i;
		if(len>cntRxd)
			len=cntRxd;
		while(len--)
		{
			*buf=*rxdBuf;
			rxd++;
			buf++;
		}
		cntRxd=0;
		return len;
}

void UartCheckEnd()
{
	unsigned char static cntBkp=0;
	unsigned char static idleTime=0;
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


void UartSendStr(*str,unsigned char len)
{
		while(len--)
		{
			while(!txdFlag);
			txdFlag=0;
			SBUF=*str++;
		}
}

void UartDriver()
{
	unsigned char len;
	unsigned char pdata buf[40];
	if(strEndFalg)
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
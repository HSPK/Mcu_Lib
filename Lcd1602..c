/**

		1602驱动程序
		by WAY
		20170710

*/



#include <reg52.h>
#include <intrins.h>
#define Lcd_DB P0

sbit Lcd_E=P2^6;
sbit Lcd_RS=P2^5;
sbit Lcd_RW=P2^7;

void DelayFourT()
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

void LcdWaitReady()
{
	unsigned char state;
	Lcd_RS=0;
	Lcd_RW=1;
	do{
		Lcd_E=1;
		state=Lcd_DB;
		Lcd_E=0;
	}while(state&0x80);
}


void LcdWriteCmd(unsigned char cmd)
{
	LcdWaitReady();
	Lcd_RS=0;
	Lcd_RW=0;
	Lcd_DB=cmd;
	Lcd_E=1;
	DelayFourT();
	Lcd_E=0;
}


void LcdInit()
{
	Lcd_E=0;
	LcdWriteCmd(0x38);
	LcdWriteCmd(0x06);
	LcdWriteCmd(0x0c);
	LcdWriteCmd(0x01);
}

void setXY(unsigned char x,unsigned char y)
{
	unsigned char addr;
	if(y==0)
		addr=0x00+x;
	else
		addr=0x40+x;
	LcdWriteCmd(addr|0x80);
}


void LcdWriteDat(unsigned char dat)
{
	LcdWaitReady();
	Lcd_RS=1;
	Lcd_RW=0;
	Lcd_DB=dat;
	Lcd_E=1;
	DelayFourT();
	Lcd_E=0;
}


void LcdShowText(unsigned char x,unsigned char y,unsigned char *dat,unsigned char len)
{
	unsigned char i=0;
	setXY(x,y);
	while(len--)
	{
		if(*dat!='\0')
			LcdWriteDat(*dat++);
		else
			break;
	}
}

void LcdAreaClean(unsigned char x,unsigned char y,unsigned char len)
{
	setXY(x,y);
	while(len--)
	{
		LcdWriteDat(' ');
	}
}


void LcdFullClean()
{
	LcdWriteCmd(0x01);
}
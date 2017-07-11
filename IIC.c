/**

			I2cͨ��io��ģ��
			by WAY
			20170710


*/



#include <reg52.h>
#include <intrins.h>
#define DelayFourT() {_nop_();_nop_();_nop_();_nop_();}

sbit I2c_SDA = P;
sbit I2c_SCL = P;


void I2cStart()
{
	I2c_SCL=1;
	I2c_SDA=1;
	DelayFourT();
	I2c_SDA=0;
	DelayFourT();
	I2c_SCL=0;
}

bit I2cWriteByt(unsigned char dat)
{
	unsigned char i;
	bit ack;
	I2c_SCL=0;
	for(i=0x80;i!=0;i>>1)
	{
		DelayFourT();
		if(i&dat==0)
			I2c_SDA=0;
		else
			I2c_SDA=1;
		DelayFourT();
		I2c_SCL=1;
		DelayFourT();
		I2c_SCL=0;
	}
	I2c_SDA=1;
	DelayFourT();
	I2c_SCL=1;
	DelayFourT();
	ack=!I2c_SDA;
	I2c_SCL=0;
	
	return ack;
}



unsigned char I2cReadByt(unsigned char ACK)
{
	unsigned char i,byt;
	for(i=0x80;i!=0;i>>1)
	{
		
		DelayFourT();
		I2c_SCL=1;
		DelayFourT();
		if(I2c_SDA)
		{
			byt|=i;
		}
		else
			I2c|=0x00;
		I2c_SCL=0;
	}
	I2c_SDA=(bit)ACK;
	DelayFourT()
	I2c_SCL=1;
	DelayFourT();
	I2c_SCL=0;
	return byt;
}


void I2cStop()
{
	I2c_SCL=0;
	I2c_SDA=0;
	DelayFourT();
	I2c_SCL=1;
	DelayFourT();
	I2c_SDA=1;
}


bit I2cCheckAck()
{
	bit ack;
	ack=I2c_SDA;
	return ~ack;
}
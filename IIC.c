/**

			I2cͨ��io��ģ��
			by WAY
			20170710


*/



#include <reg52.h>
#include <intrins.h>
#define DelayFourT() {_nop_();_nop_();_nop_();_nop_();}

sbit I2c_SDA = P2^0;
sbit I2c_SCL = P2^1;


void I2cStart()
{	
	I2c_SDA=1;
	I2c_SCL=1;
	DelayFourT();
	I2c_SDA=0;
	DelayFourT();
	I2c_SCL=0;
}

bit I2cWriteByt(unsigned char dat)
{
	unsigned char i;
	bit ack;
	for(i=0x80;i!=0;i>>=1)
	{
		if((i&dat)==0)
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
	ack=I2c_SDA;
	DelayFourT();
	I2c_SCL=0;
	
	return (~ack);
}



unsigned char I2cReadACK()
{
	unsigned char i,byt;
	I2c_SDA=1;
	for(i=0x80;i!=0;i>>=1)
	{
		DelayFourT();
		I2c_SCL=1;
		if(I2c_SDA==1)
		{
			byt|=i;
		}
		else
			byt&=(~i);
		DelayFourT();
		I2c_SCL=0;
	}

	I2c_SDA=0;
	DelayFourT();
	I2c_SCL=1;
	DelayFourT();
	I2c_SCL=0;
	return byt;
}


unsigned char I2cReadNAK()
{
	unsigned char i,byt;
	I2c_SDA=1;
	for(i=0x80;i!=0;i>>=1)
	{
		DelayFourT();
		I2c_SCL=1;
		if(I2c_SDA==1)
		{
			byt|=i;
		}
		else
			byt&=(~i);
		DelayFourT();
		I2c_SCL=0;
	}

	I2c_SDA=1;
	DelayFourT();
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
	DelayFourT();
}
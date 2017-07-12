#include <reg52.h>

extern void I2cStart();
extern void I2cStop();
extern bit I2cWriteByt(unsigned char dat);
extern unsigned char I2cReadACK();
extern unsigned char I2cReadNAK();



void E2pWriteDat(unsigned char addr,unsigned char *dat,unsigned char len)
{
	while(len>0)
	{
		do{
		I2cStart();
		if(I2cWriteByt(0x50<<1))
			break;
		I2cStop();
		}while(1);
		
		I2cWriteByt(addr);
		while(len>0)
		{
			I2cWriteByt(*dat++);
			len--;
			addr++;
			if((addr&0x07)==0)
				break;
		}
		I2cStop();
	}
}

void E2pReadDat(unsigned char addr,unsigned char *dat,unsigned char len)
{
	do{
		I2cStart();
		if(I2cWriteByt(0x50<<1))
			break;
		I2cStop();
	}while(1);
	I2cWriteByt(addr);
	I2cStart();
	I2cWriteByt((0x50<<1)|0x01);
	while(len>1)
	{
		*dat++=I2cReadACK();
		len--;
	}
	*dat=I2cReadNAK();
	I2cStop();
}
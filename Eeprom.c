#include <reg52.h>
extern bit I2cWriteByt(unsigned char dat);
extern void I2cStart();
extern void I2cStop();
extern unsigned char I2cReadByt(unsigned char ACK);
extern bit I2cCheckAck();


void E2pWtiteDat(unsigned char addr,unsigned char *dat,unsigned char len)
{
	I2cStart();
	I2cWriteByt(0x50<<1)
	while(!I2cCheckAck());
	I2cWriteByt(addr);
	while(len--)
	{
		I2cWriteByt(*dat++)
		addr++;
		if(addr&0x07==0)
			while(!I2cCheckAck());
	}
	I2cStop();
}

void E2pReadDat(unsigned char addr,unsigned char *dat,unsigned char len)
{
	I2cStart();
	I2cWriteByt(0x50);
	while(!I2cCheckAck());
	I2cWriteByt(0x50<<1+1);
	I2cStart();
	I2cWriteByt(addr);
	while(len--)
		*dat++=I2cReadByt(len-1);
	I2cStop();
}
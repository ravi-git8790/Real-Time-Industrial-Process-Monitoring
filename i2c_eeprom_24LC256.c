                 /* i2c_eeprom_24LC256.c */

#include "types.h"
#include "i2c.h"
#include "delay.h"

void i2cDevWrite(u8 slaveAddr,u16 buffAddr,u8 data)
{
	i2cStart();
	i2cWrite(slaveAddr);
	i2cAck();
	i2cWrite(buffAddr>>8);
	i2cAck();
	i2cWrite(buffAddr);
	i2cAck();
	i2cWrite(data);
	i2cAck();
	i2cStop();
	delay_ms(10);
}	

u8 i2cDevRead(u8 slaveAddr,u16 buffAddr)
{
	u8 buff=0;
    i2cStart();
	i2cWrite(slaveAddr);
	i2cAck();
	i2cWrite(buffAddr>>8);
	i2cAck();
	i2cWrite(buffAddr);
	i2cAck();
	i2cReStart(); 
	i2cWrite(slaveAddr|1);
	i2cAck();
	buff=i2cRead();
	i2cNoAck();
	i2cStop();
	return buff;
}

#ifndef __I2C_H__
#define __I2C_H__

#include "types.h"
void initI2C(void);
void i2cStart(void);
void i2cReStart(void);
void i2cWrite(u8);
void i2cAck(void);
void i2cNoAck(void);
void i2cStop(void);
u8 i2cRead(void);
void i2cMasterAck(void);
void i2cDevWrite(u8,u16,u8);//random byte write
u8 i2cDevRead(u8,u16);//random byte read

#endif

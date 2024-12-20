                            /* i2cBitBanging.c :w/o abstraction*/
#include <LPC214x.h>									
#include "i2c_defines.h"
#include "delay.h"
#include "types.h"

void i2cStart(void)
{
	IOSET0=1<<SDApin ;
	IOSET0=1<<SCLpin;
	IODIR0|=1<<SDApin|1<<SCLpin; 
	delay_us(1);
	IOCLR0=1<<SDApin; 
	delay_us(1);
}

void i2cWrite(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		IOCLR0=1<<SCLpin;	
		delay_us(1);
		
		if(dat&(0x80>>i)) 
			IOSET0=1<<SDApin; 
		else
			IOCLR0=1<<SDApin; 
		delay_us(1);
		
		IOSET0=1<<SCLpin;
		delay_us(1);
	}
}

void i2cAck(void)
{
	IOCLR0=1<<SCLpin; 
	delay_us(1);
	IOSET0=1<<SDApin; 
	delay_us(1);
	IODIR0&=~(1<<SDApin); 
	delay_us(1);
	IOSET0=1<<SCLpin;    
	delay_us(1);
	//while(IOPIN0&(1<<SDApin));
	delay_us(1);
	IOSET0=1<<SDApin;
	IODIR0|=(1<<SDApin); 
	delay_us(1);
}

void i2cStop(void)
{
	IOCLR0=1<<SCLpin; 
	delay_us(1);
	IOCLR0=1<<SDApin; 
	delay_us(1);
	IOSET0=1<<SCLpin; 
	delay_us(1);
	IOSET0=1<<SDApin; 
	delay_us(1);
}

void i2cReStart(void)
{
	IOCLR0=1<<SCLpin; 
	delay_us(1);
	IOSET0=1<<SDApin|1<<SCLpin; 
	delay_us(1);
	IOCLR0=1<<SDApin; 
	delay_us(1);
}

void i2cNoAck(void)
{
	IOCLR0=1<<SCLpin; 
	delay_us(1);
	IOSET0=1<<SDApin; 
	delay_us(1);
	IOSET0=1<<SCLpin; 
	delay_us(1);
}

u8 i2cRead(void)
{
	u8 i,buff=0;
	IODIR0&=~(1<<SDApin); 
	delay_us(1);
  for(i=0;i<8;i++)
	{
		IOCLR0=1<<SCLpin; 
		delay_us(1);
		IOSET0=1<<SCLpin; 
		delay_us(1); 
		if(IOPIN0&(1<<SDApin))
			buff|=0x80>>i;
		delay_us(1);
	}
	
	IODIR0|=(1<<SDApin); 
	delay_us(1);
  return buff;
}


#include<lpc214x.h>
#include <string.h>

#include "lcd.h"
#include "delay.h"
#include "uart0.h"
#include "gsm.h"
#include "adc_defines.h"
#include "adc.h"
#include "i2c.h"
#include "defines.h"

#define LED 4

extern char buff[125],i,ch,message_receive;

u8 temp_set,cur_temp;
f32 temp;
u8 i_flag=0;

int main()
{
	SETBIT(IODIR0,LED);
	LCD_Init( );
	LCD_str("R.T.I.P.M.A.C.S");
	InitUART0();
	GSM_init();
	Init_ADC();
	
	i2cDevWrite(0xa0,0x0010,35);

	while(1)
	{
		do
		{
		LCD_cmd(0x01);LCD_cmd(0x80);
		LCD_str("C.T: ");
		temp=Read_ADC(CH1);
	    cur_temp=(int)(temp*100);
		U32LCD(cur_temp);
		temp_set = i2cDevRead(0xa0,0x0010);
		LCD_cmd(0xC0);
		LCD_str("S.P: ");
		U32LCD(temp_set);
		delay_ms(1000);
		if(cur_temp>temp_set)
			SETBIT(IOCLR0,LED);//Active LOW LED ON	
		else
			SETBIT(IOSET0,LED);	//Active LOW LED OFF

		if(cur_temp>temp_set+5)
			send_sms("9705160811", "High Alert: Current Temperature value Crossed the SP+5 value. Please Take Care."); 
		}while(message_receive==0);
		message_receive=0;
		//set point modification logic
		LCD_cmd(0x01);LCD_cmd(0x80);
		LCD_str("Message Received");
		delay_ms(1000);
		i=0;memset(buff,'\0',125); 
		UART0_Str("AT+CMGR=1\r\n");
		while(i<3);
		delay_ms(1000);
		if(strstr(buff,"OK"))
		{
			LCD_cmd(0x01);LCD_cmd(0x80);
			LCD_str(buff);
			delay_ms(1000);	 
		}
		else
		{
			LCD_cmd(0x01);LCD_cmd(0x80);
			LCD_str("Message ! Exist");
			delay_ms(1000);	
		}				 
	 }
}



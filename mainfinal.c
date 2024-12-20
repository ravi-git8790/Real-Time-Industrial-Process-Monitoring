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
	char PhNo[15],j;
	SETBIT(IODIR0,LED);
	LCD_Init( );
	LCD_str("R.T.I.P.M.A.C.U.G");
	delay_ms(1000);
	InitUART0();
	GSM_init();
	Init_ADC();
//first time need to update setpoint once into EEPROM - only once	
//	i2cDevWrite(0xa0,0x0010,35); 

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
		//+CMGR: "REC UNREAD","919705160811","","2023/06/09 10:54:17+22"
		//$76#
		while(i<3);
		delay_ms(1000);
		if(strstr(buff,"OK"))
		{
			temp_set = 0;
			LCD_cmd(0x01);LCD_cmd(0x80);
			LCD_str(buff);
			i=0;j=0;memset(PhNo,'\0',15);
			//read the phone mumber
			while(buff[i++] != '1');
			for(;buff[i]!='"';i++)
				PhNo[j++] = buff[i];
			PhNo[j] = '\0';
			LCD_cmd(0x01);LCD_cmd(0x80);
			LCD_str("From:");
			LCD_cmd(0xC0);
			LCD_str(PhNo);
			delay_ms(1000);
			temp_set=0;
			//read the message content for set point	SETPOINT SYNTAX - $SETPOINT# Example: $34#
			while(buff[i++] != '$');
			LCD_cmd(0x01);LCD_cmd(0x80);

			for(;buff[i]!='#';i++)
			{
				temp_set = (temp_set * 10) + ((buff[i]-48));
			}

			LCD_cmd(0x01);LCD_cmd(0x80);
			LCD_str("New S.P:");
			U32LCD(temp_set);
			delay_ms(1000);	 
			if(temp_set<150)
				i2cDevWrite(0xa0,0x0010,temp_set);
			else
			{
				LCD_cmd(0x01);LCD_cmd(0x80);
				LCD_str("Invalid Set Point");
				LCD_cmd(0xC0);
				LCD_str("Range: -55 to 150");
				delay_ms(1000);				
			}

			i=0;memset(buff,'\0',125);
			UART0_Str("AT+CMGD=1\r\n");
			while(i<2);
			delay_ms(500);
			buff[i] = '\0';
			if(( strstr(buff,"OK") || strstr(buff,"ERROR")))
			{
				LCD_cmd(0xC0);
				LCD_str("Message Deleted");
				delay_ms(500);
			}
			else 
			{		
				LCD_cmd(0xC0);
				LCD_str("ERROR");
				delay_ms(500);
				LCD_cmd(0xC0);
				LCD_str("  ");
			}
		}
		else
		{
			LCD_cmd(0x01);LCD_cmd(0x80);
			LCD_str("Message ! Exist");
			delay_ms(1000);	
		}				 
	 }
}



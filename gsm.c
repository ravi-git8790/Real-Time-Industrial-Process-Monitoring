#include <string.h>
#include "uart0.h"
#include "delay.h"
#include "lcd.h"


extern char buff[125],i,ch;

char gsm_cmd[][14] = {"AT\r\n","ATE0\r\n","AT+CMGF=1\r\n","AT+CNMI=1,3\r\n","AT+CMGD=1\r\n"};

void GSM_init(void)
{
	char j;	
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_str("GSM init....");

	for(j=0;j<5;j++)
	{
		i=0;memset(buff,'\0',125);
		UART0_Str(gsm_cmd[j]);
		while(i<2);
		delay_ms(500);
		buff[i] = '\0';
		if((j==4) && ( strstr(buff,"OK") || strstr(buff,"ERROR")))
		{
			LCD_cmd(0xC0);
			LCD_str("OK");
			delay_ms(500);
			LCD_cmd(0xC0);
			LCD_str("  ");
			break;
		}
		else if(strstr(buff,"OK"))
		{		
			LCD_cmd(0xC0);
			LCD_str("OK");
			delay_ms(500);
			LCD_cmd(0xC0);
			LCD_str("  ");
		}
		else
		{
			LCD_cmd(0xC0);
			LCD_str("ERROR");
			delay_ms(500);
			LCD_cmd(0xC0);
			LCD_str("     ");
			break;
		}
	}
	if(j==4)
	{
		LCD_cmd(0x01);LCD_cmd(0x80);
		LCD_str("GSM init. Done");
		delay_ms(1000);
	} 
	else
	{
		LCD_cmd(0x01);LCD_cmd(0x80);
		LCD_str("GSM init. Failed");
		delay_ms(2000);
		LCD_cmd(0x01);LCD_cmd(0x80);
		LCD_str("Check Connection");
		LCD_cmd(0xC0);
		LCD_str("Try Again ...!");
		delay_ms(1000);
	} 
}

void send_sms(char *num,char *msg)
{
	LCD_cmd(0x01);LCD_cmd(0x80);
	LCD_str("Sending sms....");
	delay_ms(500);
	i=0;memset(buff,'\0',125);
	UART0_Str("AT+CMGS=");
	UART0_Tx('"');
	UART0_Str(num);
	UART0_Tx('"');
	UART0_Str("\r\n");
	
	delay_ms(500);
	i=0;
	UART0_Str(msg);
    UART0_Tx(0x1A);
	delay_ms(5000);
	while(i<3);
	buff[i] = '\0';
	if(strstr(buff,"OK"))
	{
		LCD_cmd(0x01);LCD_cmd(0x80);
		LCD_str("Message Sent..");
		delay_ms(500);
	}
	else
	{
		LCD_cmd(0x01);LCD_cmd(0x80);
		LCD_str("Message failed..");
		delay_ms(500);
	}
}

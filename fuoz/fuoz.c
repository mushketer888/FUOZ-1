/*
 * fuoz.c
 *	prowivka ras4itana na tranzistornij kommutator v moem slu4ae eto bil prosto tranzistor darlingtona BDX33C
 *  Atmega8 8MHz
 *	Attiny2313 8MHz
 *  Author: OFPR
 *  Main file
 */ 

#include "config.h"
//#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>



/*void lcd_putstring(char * str) {
  //LED_ON;	
  while (*str) {
    LCDsendChar(*str++);
  }
  LED_OFF;
}*/

/*void Adc_Stat_LCD(void)
{
	//LED_ON;
	char st[3];
	unsigned int tm;
	char line[16] = {'T', 'e', 'm', 'p', ':', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
	tm = Term() / 10;
	itoa(tm, st, 10);
	//memcpy (&string[11], st, 3)
	line[6] = st[0];
	line[7] = st[1];
	line[8] = st[2];
	LCDclr();
	_delay_ms(1);
	LCDGotoXY(0, 0);
	_delay_ms(1);
	lcd_putstring(line);
	LED_OFF;
}*/

int main(void)
{
	fuoz_init(); 
	DDRB |= 1<<0;	
	//ADC_Init();
	//LCDinit();
	//LCDclr();
    while(1)
    {
		LED_OFF;
		//Adc_Stat_LCD();
		//_delay_ms(1000);
		// sjuda mozhno hotj LCD prisoba4itj toljko neljzja zaprewatj prerivanija na dliteljnoe vremja          
    }
}








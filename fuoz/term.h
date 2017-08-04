#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "config.h"
#ifndef TERM_H
#define TERM_H
	//volatile const int temps[] PROGMEM;
	void ADC_Init(void);
	unsigned int ADC_Read(void);
	unsigned int Term(void);
	ISR(ADC_vect);
#endif
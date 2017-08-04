#ifndef FORMIROVATELJ_H
#define FORMIROVATELJ_H

#include <avr/interrupt.h>
#include "config.h"
//===========================
void fuoz_init(void);
ISR(INT0_vect);
ISR(TIMER1_COMPA_vect);
ISR(TIMER1_COMPB_vect);
ISR(TIMER1_OVF_vect);
//===========================

#endif
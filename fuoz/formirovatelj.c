#include <avr/interrupt.h>
#include "config.h"
#define PIN_DDR  DDRB |= 1<<5
#define PIN_H PORTB |= 1<<5
#define PIN_L PORTB &= ~(1<<5)
const int Nakoplenie = 228; // t=((sqrt(E*2/L)*L)/U)/(8*10e-6) gde E energija v dzhouljah (0.1J dlja mowjnih katuwek, 0.05J dlja obi4noj
							// babini), L induktivnostj babini v henri 0.0024H, U naprjazhenie v voljtah 12V
							
const int Gorenie = 100;	//vremja gorenija benzina do stenki cilingra 800us (odin tik tajmera 8us) '650us

//=====================================================================================================================

unsigned int vrema = 0;
unsigned int vremaOld = 0;
unsigned int tmp = 0;
unsigned int zar = 0;

void fuoz_init(void)
{
	//========init============
	#if (__AVR_ATtiny2313__) || (__AVR_ATmega8__)
		#warning Atmega8 or Attiny2313
		cli();
		TIFR |= 1<<OCF1A|1<<OCF1B;
		TIMSK |= 1<<OCIE1A|1<<OCIE1B|1<<TOIE1;//prerivanija tajmera ih tut visit azh 3 wtuki :D
		#if Interrupt_MODE == 0 //int0 rising
			MCUCR |= 1<<ISC00|1<<ISC01; 
			GIMSK = 1<<INT0;//vnewnee prerivanie
		#elif Interrupt_MODE == 1 //int0 falling
			MCUCR = (1<<ISC01) | ~(1<<ISC00);
			GIMSK = 1<<INT0;
		#elif Interrupt_MODE == 2 //int1 rising
			MCUCR |= 1<<ISC10|1<<ISC11;
			GIMSK = 1<<INT1;
		#elif Interrupt_MODE == 3 //int1 falling
			MCUCR = (1<<ISC11)|~(1<<ISC10);
			GIMSK = 1<<INT1;
		#endif
		PIN_DDR;
		sei();
	#elif (__AVR_ATmega328P__)
		#warning Atmega328P
		cli();
		TIFR1 |= 1<<OCF1A|1<<OCF1B;
		TIMSK1 |= 1<<OCIE1A|1<<OCIE1B|1<<TOIE1;
		#if Interrupt_MODE == 0 //int0 rising
			EICRA |= 1<<ISC00|1<<ISC01;
			EIMSK = 1<<INT0;
		#elif Interrupt_MODE == 1 //int0 falling
			EICRA = (1<<ISC01) | ~(1<<ISC00);
			EIMSK = 1<<INT0;
		#elif Interrupt_MODE == 2 //int1 rising
			EICRA |= 1<<ISC10|1<<ISC11;
			EIMSK = 1<<INT1;
		#elif Interrupt_MODE == 3 //int1 falling
			EICRA = (1<<ISC11)|~(1<<ISC10);
			EIMSK = 1<<INT1;
		#endif
		PIN_DDR;
		sei();
	#endif
	//=======================
}

//================================CDI=====================================================
#if Type == 1 


#if Interrupt_MODE == 0 || Interrupt_MODE == 1
ISR(INT0_vect)
#elif Interrupt_MODE == 2 || Interrupt_MODE == 3
ISR(INT1_vect)
#endif


{
	TCCR1B |= 1<<CS10|1<<CS11;//preddelitelj na 64 zapuskaem tajmer esli dat4ik dal signal
	PIN_H;
	vremaOld = vrema;
	vrema = TCNT1;
	TCNT1 = 0;
	if (vrema >= vremaOld) // tormozhenie dvigatelja
	{
		tmp = vrema * 2;
		tmp = tmp - vremaOld;
		tmp = tmp - Gorenie;
		
	} 
	else
	{
		tmp = vremaOld - vrema; // razgon
		tmp = vrema - tmp;
		tmp = tmp - Gorenie;
	}
	tmp = vrema - Gorenie;
	OCR1A = tmp;
	
	
};

ISR(TIMER1_COMPA_vect)
{
	PIN_H;	//ISKRA!
	OCR1B = tmp + 3;// + vremja srabativanija tiristora po datawitu 10us vzjal s zapasom 24us
	
};

ISR(TIMER1_COMPB_vect)
{
	PIN_L; // zapiraem tiristor 4tob daljwe kopitj energiju v kondensatore
};

ISR(TIMER1_OVF_vect)// ostanovka dvigatelja virubaem zazhiganie
{
	PIN_L;
	TCCR1B = 0x00;
};
#else //=================================================TCI================================================================

#if Interrupt_MODE == 0 || Interrupt_MODE == 1
ISR(INT0_vect)
#elif Interrupt_MODE == 2 || Interrupt_MODE == 3
ISR(INT1_vect)
#endif

{
	cli();
	//LED_ON;
	TCCR1B |= 1<<CS10|1<<CS11;//preddelitelj na 64 zapuskaem tajmer esli dat4ik dal signal
	PIN_L;
	vremaOld = vrema;
	vrema = TCNT1;
				
	TCNT1 = 0;
	if (vrema >= vremaOld) // tormozhenie dvigatelja
	{
		tmp = vrema * 2;
		tmp = tmp - vremaOld;
		tmp = tmp - Gorenie;
		
	} 
	else
	{
		tmp = vremaOld - vrema; // razgon
		tmp = vrema - tmp;
		tmp = tmp - Gorenie;
	}
	

	//tmp = vrema - Gorenie; // vremja do to4ki kogda dolhna bitj iskra
	zar = tmp - Nakoplenie; // vremja do to4ki kogda nuzhno na4atj kopitj energiju v katuwke
	OCR1A = zar;
	OCR1B = tmp;
	//TCNT1 = 0;
	sei();
	
	
};

ISR(TIMER1_COMPA_vect)
{
	PIN_H;	// ZARJADKA!
	//LED_ON;
};

ISR(TIMER1_COMPB_vect)
{
	//LED_ON;
	PIN_L; // ISKRA!
};

ISR(TIMER1_OVF_vect) // ostanovka dvigatelja
{
	//LED_ON;
	PIN_L;
	TCCR1B = 0x00;//esli dvigatelj zagluwen neza4em tratit energiju akuma na razogrev katuwki zazhiganija 
};				  //maximaljnoe vremja nahozhdenija katuwki pod tokom primerno 0,5 sekundi
#endif

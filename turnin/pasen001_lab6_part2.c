/*	Author: lab
 *  Partner(s) Name: Peter John Asence
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/Sl7xUhk3law
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
//timer stuff
volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr =0;

void TimerOn(){
	TCCR1B = 0x0B;
	OCR1A = 123;
	TIMSK1 = 0x02;
	TCNT1 = 0;

	_avr_timer_cntcurr = _avr_timer_M;

	SREG |=0x80;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag=1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
//timer stuff


enum LED_states {Start, Light1, Light2, Light3, Light4, press} states;

void tick_LED(){
	unsigned char but;
	but = ~PINA & 0x01;
	//transition actions
	switch(states){
	case Start:
		if (but == 0x01) states = press;
		else  states = Light1;	
		break;
	case Light1:
		if (but == 0x01) states = press;
		else states = Light2;
		break;
	case Light2: 
		if (but == 0x01) states = press;
		else states = Light3;
		break;
	case Light3: 
		if (but == 0x01) states = press;
		else states = Light4;
		break;
	case Light4:
		if (but == 0x01) states = press;
                else states = Light1;
		break;
	case press:
		if (but == 0x01) states = Light1;
		else states = press;
		break;
	default: 
		states = Start;
		break;
	}
	// state actions 
	switch(states){
		case Start:
			break;
		case Light1:
			PORTB = 0x01;
			break;
		case Light2:
			PORTB = 0x02;
			break;
		case Light3:
			PORTB =0x04;
			break;
		case Light4:
			PORTB = 0x02;
			break;
		case press:
			break;
		default:
			break;
		}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF;
	PORTB = 0x00;
	DDRA = 0x00;
	PORTA = 0xFF;
    /* Insert your solution below */
	TimerSet(300);
	TimerOn();
	
	states = Start;

    while (1) {
	tick_LED();
	while(!TimerFlag);
	TimerFlag = 0;
    }
   
    return 1;
}

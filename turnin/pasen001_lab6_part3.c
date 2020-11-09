/*	Author: lab
 *  Partner(s) Name: Peter John Asence
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/wPPYRsg8mIQ
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


enum LED_states {Start, init, inc_p, inc_r, dec_p, dec_r, reset_p,} states;
unsigned char i = 0;
void tick_LED(){
	unsigned char but;
	but = ~PINA & 0x03;
	//transition actions
	switch(states){
	case Start:
		PORTB = 0x07;
		states = init;
		break;
	case init:
		if (but == 0x01) states = inc_p;
		else if(but == 0x02) states = dec_p;
		else if(but == 0x03) states = reset_p;
		else states = init;
		break;
	case inc_p: 
		if (but == 0x00) states = dec_r ;
		else if (but == 0x03) states = reset_p;
		else states = inc_p;
		break;
	case inc_r: 
		if (but == 0x01) states = inc_p;
		else if (but == 0x02) states = dec_p;
	        else if (but == 0x03) states = reset_p;
		else states = init;
		break;
	case dec_p:
		if (but == 0x00) states = dec_r ;
                else if (but == 0x03) states = reset_p;
                else states = dec_p;
                break;
	case dec_r:
		if (but == 0x01) states = inc_p;
                else if (but == 0x02) states = dec_p;
                else if (but == 0x03) states = reset_p;
                else states = init;
                break;
	case reset_p:
		if (but == 0x01) states = inc_p;
                else if (but == 0x02) states = dec_p;
		else states = reset_p;
		break;
	default: 
		states = Start;
		break;
	}
	// state actions 
	switch(states){
		case Start:
			break;
		case init:
			break;
		case inc_p:
                        i++;
			if ( i % 10 == 0 || i == 1){
			    if (PORTB < 9) PORTB = PINB + 1;  
			} 
			break;
		case inc_r:
			i = 0;
			break;
		case dec_p:
			i++;
			if (i % 10 == 0 || i == 1){
			       if(PORTB > 0) PORTB = PINB - 1;
			}
			break;
		case dec_r:
			i = 0;
			break;
		case reset_p:
			PORTB = 0x00;
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
	TimerSet(100);
	TimerOn();
	
	states = Start;

    while (1) {
	tick_LED();
	while(!TimerFlag);
	TimerFlag = 0;
    }
   
    return 1;
}

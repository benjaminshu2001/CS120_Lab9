/*	Author: Benjamin SHu
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Link to Demo: https://www.youtube.com/watch?v=ZCyHEAm2pL8
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

unsigned char threeLEDs;
unsigned char blinkingLED;

enum ThreeLEDs_States {Start, Light1, Light2, Light3} ThreeLEDs_State;
enum BlinkingLED_States {Begin, On, Off} BlinkingLED_State;
enum CombineLEDs_States {Init} CombineLEDs_State;

void ThreeLEDsSM() {

	switch(ThreeLEDs_State) {
		case Start:
			ThreeLEDs_State = Light1;
			break;
		case Light1:
			ThreeLEDs_State = Light2;
			break;
		case Light2:
			ThreeLEDs_State = Light3;
			break;
		case Light3:
			ThreeLEDs_State = Light1;
			break;
		default:
			ThreeLEDs_State = Start;
			break;
	}
	switch(ThreeLEDs_State) {
		case Begin:
			break;
		case Light1:
			threeLEDs = 0x01;
			break;
		case Light2:
			threeLEDs = 0x02;
			break;
		case Light3:
			threeLEDs = 0x04;
			break;
		default:
			break;
	}
}
void BlinkingLEDSM() {
	switch(BlinkingLED_State) {
		case Begin:
			BlinkingLED_State = On;
			break;
		case On:
			BlinkingLED_State = Off;
			break;
		case Off:
			BlinkingLED_State = On;
			break;
		default:
			BlinkingLED_State = Begin;
			break;
	}
	switch(BlinkingLED_State) {
		case Begin:
			break;
		case On:
			blinkingLED = 0x08;
			break;
		case Off:
			blinkingLED = 0x00;
			break;
		default:
			break;
	}
}
void CombineLEDsSM() {
	switch(CombineLEDs_State) {
		case Init:
			PORTB = threeLEDs | (blinkingLED);
			break;
		default:
			break;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	unsigned long threeLEDs_elapsedTime = 300; //obtained from 6.2 from ZyBooks
	unsigned long blinkingLEDs_elapsedTime = 1000;
	const unsigned long timerPeriod = 1;

	TimerSet(timerPeriod);
	TimerOn();
	ThreeLEDs_State = Start;
	BlinkingLED_State = Begin;
	CombineLEDs_State = Init;
    while (1) {
		if(threeLEDs_elapsedTime >= 300) {
			ThreeLEDsSM();
			threeLEDs_elapsedTime = 0;
		}
		if(blinkingLEDs_elapsedTime >= 1000) {
			BlinkingLEDSM();
			blinkingLEDs_elapsedTime = 0;
		}
		CombineLEDsSM();
		while(!TimerFlag);
		TimerFlag = 0;
		threeLEDs_elapsedTime += timerPeriod;
		blinkingLEDs_elapsedTime += timerPeriod;
    }
    return 1;
}

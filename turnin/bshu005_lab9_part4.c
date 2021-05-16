/*	Author: Benjamin SHu
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Link to Demo: https://www.youtube.com/watch?v=DDVOsXNqwSg
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"

unsigned char threeLEDs;
unsigned char blinkingLED;
unsigned char toggle = 0x00;
unsigned char count = 0;
unsigned char frequency = 0;
unsigned char frequencies[] = {1,2,3,4,5,6,7};
enum ThreeLEDs_States {Start, Light1, Light2, Light3} ThreeLEDs_State;
enum BlinkingLED_States {Begin, On, Off} BlinkingLED_State;
enum CombineLEDs_States {Init} CombineLEDs_State;
enum Speaker_States {S_Start, Wait, S_On, Inc, Dec, S_Off} Speaker_State;

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
			PORTB = threeLEDs | blinkingLED | toggle;
			break;
		default:
			break;
	}
}
void SpeakerSM() {
	switch(Speaker_State) {
		case S_Start:
			Speaker_State = S_Off;
			break;
			
		case S_Off:
			if((~PINA & 0x07) == 0x04) {//speaker is on if PA2 is pressed
				Speaker_State = S_On;
			}
			else if((~PINA & 0x07) == 0x01) {
				Speaker_State = Inc;
			}
			else if((~PINA & 0x07) == 0x02) {
				Speaker_State = Dec;
			}
			else {
				Speaker_State = S_Off;
			}
			break;
		case S_On:
			if((~PINA & 0x07) == 0x00) {
				Speaker_State = S_Off;
			}
			else {
				Speaker_State = S_On;
			}
			break;
		case Inc:
			Speaker_State = Wait;
			break;
		case Dec:
			Speaker_State = Wait;
			break;
		case Wait:
			if((~PINA & 0x07) == 0x00) {
				Speaker_State = S_Off;
			}
			else {
				Speaker_State = Wait;
			}
			break;
		default:
			Speaker_State = S_Start;
	}
	switch(Speaker_State) {
		case S_Start:
			break;
		case S_Off:
			toggle = 0x00;
			count = 0;
			break;
		case S_On:
			if(count <= frequencies[frequency]) {
				toggle = 0x40;
			}
			else {
				toggle = 0x00;
				count = 0;
			}
			count++;
			break;
		case Inc:
			if(frequency > 0) {
				frequency--;
			}
			break;
		case Dec:
			if(frequency < 7) {
				frequency++;
			}
			break;
		case Wait:
			break;
		default:
			toggle = 0x00;
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
	Speaker_State = S_Off;
    while (1) {
		if(threeLEDs_elapsedTime >= 300) {
			ThreeLEDsSM();
			threeLEDs_elapsedTime = 0;
		}
		if(blinkingLEDs_elapsedTime >= 1000) {
			BlinkingLEDSM();
			blinkingLEDs_elapsedTime = 0;
		}
		SpeakerSM();
		CombineLEDsSM();
		while(!TimerFlag);
		TimerFlag = 0;
		threeLEDs_elapsedTime += timerPeriod;
		blinkingLEDs_elapsedTime += timerPeriod;
    }
    return 1;
}

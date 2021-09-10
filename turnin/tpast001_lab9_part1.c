/*	Author: Tyler Pastor tpast001@ucr.edu
 *  Partner(s) Name: N/A
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=JDiEcJDuzjs
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
int count;
int elapsedTime;
int elapsedTime_2;

enum SM1_STATES {SM1_SMStart, SM1_blink} SM1_STATE;
void ThreeLEDsSM() {
	switch(SM1_STATE) {
     		case SM1_SMStart:
      			SM1_STATE = SM1_blink;
			break;

		case SM1_blink:
			if(elapsedTime != 1000){
				elapsedTime++;
			}
			else if(elapsedTime == 1000){
				if(count == 0){
					threeLEDs = 0x01;
					count++;
				}
				else if(count == 1){
					threeLEDs = 0x02;
					count++;
				}
				else if(count == 2){
					threeLEDs = 0x04;
					count = 0;
				}
				elapsedTime = 0;
			}

			SM1_STATE = SM1_blink;
			break;

      		default:
         		SM1_STATE = SM1_blink;
         		break;
   	}

   	switch(SM1_STATE) {
      		case SM1_SMStart:
         		break;

		case SM1_blink:
			break;
	}
}

//////////////////////////////////////////////

enum SM2_STATES {SM2_SMStart, SM2_B3} SM2_STATE;
void BlinkingLEDSM() {
        switch(SM2_STATE) {
                case SM2_SMStart:
                        SM2_STATE = SM2_B3;
                        break;

		case SM2_B3:
			if(elapsedTime_2 != 1000){
				elapsedTime_2++;
			}
			else if(elapsedTime_2 == 1000){
				if(blinkingLED == 0x00){
					blinkingLED = 0x08;
				}
				else{
					blinkingLED = 0x00;
				}
				elapsedTime_2 = 0;
			}

			SM2_STATE = SM2_B3;
			break;

                default:
                        SM2_STATE = SM2_B3;
                        break;
        }

        switch(SM2_STATE) {
                case SM2_SMStart:
                        break;

		case SM2_B3:
			break;
        }
}

////////////////////////////////////////////

enum SM3_STATES {SM3_SMStart, SM3_combine} SM3_STATE;
void CombineLEDsSM() {
        switch(SM3_STATE) {
                case SM3_SMStart:
                        SM3_STATE = SM3_combine;
                        break;

		case SM3_combine:
			PORTB = threeLEDs | blinkingLED;
			SM3_STATE = SM3_combine;
			break;

                default:
                        SM3_STATE = SM3_combine;
                        break;
        }

        switch(SM3_STATE) {
                case SM3_SMStart:
                        break;

		case SM3_combine:
			break;
        }
}

///////////////////////////////////////////

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;

	SM1_STATE = SM1_SMStart;
	SM2_STATE = SM2_SMStart;
	SM3_STATE = SM3_SMStart;

	TimerSet(1);
	TimerOn();

   	while (1) {
		ThreeLEDsSM();
		BlinkingLEDSM();
		CombineLEDsSM();
		while(!TimerFlag);
		TimerFlag = 0;	
	}
    	
	return 0;
}

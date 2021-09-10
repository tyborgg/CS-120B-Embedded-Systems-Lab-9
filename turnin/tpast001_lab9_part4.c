/*	Author: Tyler Pastor tpast001@ucr.edu
 *  Partner(s) Name: N/A
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=CaJ6VnstBpY
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char speakerVal = 0x00;
int count;
int elapsedTime;
int elapsedTime_2;
int elapsedTime_3;
int frequency = 1;
int flag;
unsigned char A;

enum SM1_STATES {SM1_SMStart, SM1_blink} SM1_STATE;
void ThreeLEDsSM() {
	switch(SM1_STATE) {
     		case SM1_SMStart:
      			SM1_STATE = SM1_blink;
			break;

		case SM1_blink:
			if(elapsedTime != 300){
				elapsedTime++;
			}
			else if(elapsedTime == 300){
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

enum SM3_STATES {SM3_SMStart, SM3_wait, SM3_on, SM3_off} SM3_STATE;
void speaker() {
        switch(SM3_STATE) {
                case SM3_SMStart:
                        SM3_STATE = SM3_wait;
                        break;

		case SM3_wait:
			if(A == 0x04){
				flag = 1;
				speakerVal = 0x10;
				SM3_STATE = SM3_on;
			}
			else{
				speakerVal = 0x00;
				SM3_STATE = SM3_wait;
			}
			break;

		case SM3_on:
			speakerVal = 0x10;

			if(flag == 1){
				flag = 0;
				SM3_STATE = SM3_off;
			}	
			else{
				if(A == 0x04){
					if(elapsedTime_3 != frequency){
                                        	elapsedTime_3++;
                                        	SM3_STATE = SM3_on;
                                	}
                                	else{
                                        	elapsedTime_3 = 0;
                                        	SM3_STATE = SM3_off;
                                	}	
				}
				else{
					elapsedTime_3 = 0;
					SM3_STATE = SM3_wait;
				}
			}
			break;

		case SM3_off:
			speakerVal = 0x00;

			if(A == 0x04){
				if(elapsedTime_3 != frequency){
					elapsedTime_3++;
					SM3_STATE = SM3_off;
				}
				else{
					elapsedTime_3 = 0;
					SM3_STATE = SM3_on;
				}
			}
			else{
				elapsedTime_3 = 0;
				speakerVal = 0x00;
				SM3_STATE = SM3_wait;
			}
			break;

                default:
                        SM3_STATE = SM3_wait;
                        break;
        }

        switch(SM3_STATE) {
                case SM3_SMStart:
                        break;

		case SM3_wait:
			break;

		case SM3_on:
			break;

		case SM3_off:
			break;
        }
}

////////////////////////////////////////////

enum SM4_STATES {SM4_SMStart, SM4_combine} SM4_STATE;
void CombineLEDsSM() {
        switch(SM4_STATE) {
                case SM4_SMStart:
                        SM4_STATE = SM4_combine;
                        break;

		case SM4_combine:
			PORTB = threeLEDs | blinkingLED | speakerVal;
			SM4_STATE = SM4_combine;
			break;

                default:
                        SM4_STATE = SM4_combine;
                        break;
        }

        switch(SM4_STATE) {
                case SM4_SMStart:
                        break;

		case SM4_combine:
			break;
        }
}

//////////////////////////////////////////

enum SM5_STATES {SM5_SMStart, SM5_toggle, SM5_wait} SM5_STATE;
void toggle() {
        switch(SM5_STATE) {
                case SM5_SMStart:
                        SM5_STATE = SM5_wait;
                        break;

		case SM5_wait:
			if(A == 0x02){
				if(frequency != 6){
					frequency++;
				}
				SM5_STATE = SM5_toggle;
			}
			else if(A == 0x01){
				if(frequency != 0){
					frequency--;
				}
				SM5_STATE = SM5_toggle;
			}
			else{
				SM5_STATE = SM5_wait;
			}
			break;

                case SM5_toggle:
                        if(A == 0x01 || A == 0x02){
                                SM5_STATE = SM5_toggle;
                        }
                        else if(A == 0x00){
                                SM5_STATE = SM5_wait;
			}
                        break;

                default:
                        SM5_STATE = SM5_wait;
                        break;
        }

        switch(SM5_STATE) {
                case SM5_SMStart:
                        break;

                case SM5_toggle:
                        break;

		case SM5_wait:
			break;
        }
}

///////////////////////////////////////////

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	SM1_STATE = SM1_SMStart;
	SM2_STATE = SM2_SMStart;
	SM3_STATE = SM3_SMStart;
	SM4_STATE = SM4_SMStart;
	SM5_STATE = SM5_SMStart;

	TimerSet(1);
	TimerOn();

   	while (1) {
		A = ~PINA;
		
		toggle();
		ThreeLEDsSM();
		BlinkingLEDSM();
		speaker();
		CombineLEDsSM();
		while(!TimerFlag);
		TimerFlag = 0;	
	}
    	
	return 0;
}

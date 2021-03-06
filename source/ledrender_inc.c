#ifndef LEDRENDER_INC_C
#define LEDRENDER_INC_C

#include <avr/io.h>
#include <stdio.h>
#include "fncontrol.h"
#include "ledrender.h"
#include "hardwareinfo.h"
#include "ledconfig.h"
#include "bootmapper.h"

static uint8_t LEDstate = 0;     ///< current state of the LEDs
#define blinkLedCountDelay 900

static uint8_t ledBlinkCount = 0;

/*
 * xPrev : LED의 이전 상태를 저장하고 있다. 이정 상태와 다를 경우에만 깜박이도록.
 */
void getLedBlink(uint8_t xLed, bool xStatus, uint8_t *xPrevState, uint8_t *xCount){
    if ((LEDstate & xLed) && !(*xPrevState & xLed)) { // light up

//		*xPrevState = LEDstate;
    	if(!xStatus){
    	    *xCount =  5;	//on off on off 1
		}else{
		    *xCount =  4;	// off on off 1
		}

	} else if(!(LEDstate & xLed) && (*xPrevState & xLed)){

//		*xPrevState = LEDstate;
		if(!xStatus){
		    *xCount =  3;	// on off 1
		}else{
		    *xCount =  2;	// off 1
		}

	}
}

void blinkIndicateLED(void) {
	static int counter = 0;
	const int countMAX = 100;
	//on off on off
	if(ledBlinkCount > 0){
		counter++;
		if(counter > countMAX){
			if(ledBlinkCount == 5 || ledBlinkCount == 3){
				if(isBeyondFnLedEnabled() == BEYOND_FN_LED_NL){
					turnOnLED(LEDNUM);
				}else{
					turnOnLED(LEDSCROLL);
				}
			}else if(ledBlinkCount == 4 || ledBlinkCount == 2){
				if(isBeyondFnLedEnabled() == BEYOND_FN_LED_NL){
					turnOffLED(LEDNUM);
				}else{
					turnOffLED(LEDSCROLL);
				}
			}else{
				if(isBeyondFnLedEnabled() == BEYOND_FN_LED_NL){
					if(isBeyondFN()){
						turnOnLED(LEDNUM);
					}
				}else if(isBeyondFnLedEnabled() == BEYOND_FN_LED_SL){
					if(isBeyondFN()){
						turnOnLED(LEDSCROLL);
					}
				}else{
					if((getLEDState() & LED_STATE_NUM)){
						turnOnLED(LEDNUM);
					}
				}
			}
			counter = 0;

			ledBlinkCount--;
		}
	}else{
		counter = 0;
	}
}


void blinkBootMapperLED(void) {
//	static int gCounter = 0;
	static int gDelayCounter = 0;
	static uint8_t gLEDState = 1;
//	const int gCountMAX = blinkLedCountDelay; // 200;
	uint8_t gIsOn = 0;

#ifdef ENABLE_BOOTMAPPER
	if(isBootMapper()){
		gIsOn = 1;
	}
#endif

	if(gIsOn == 1){
		++gDelayCounter;
		if(gDelayCounter > blinkLedCountDelay){	//setDelay(blinkLedCountDelay)){
//			++gCounter;
//			if(gCounter > gCountMAX){
//				if(getLEDState() & LED_STATE_CAPS){	// Caps Lock이 켜져 있을때는 커졌다 켜지고;
					if(gLEDState == 1){
						turnOffLED(LEDCAPS);
					}else{
						turnOnLED(LEDCAPS);
//						gDelayCounter = 0;
					}
				/*}else{	// Caps Lock이 꺼져 있을 때는 켜졌다 꺼진다.
					if(gLEDState == 1){
						turnOnLED(LEDCAPS);
					}else{
						turnOffLED(LEDCAPS);
//						gDelayCounter = 0;
					}
				}*/
				gDelayCounter = 0;
				gLEDState ^= 1;
//				gCounter = 0;
//			}
		}
	}else{
//		gCounter = 0;
		gLEDState = 1;
	}
}

void blinkOnce(const int xStayMs){
	if(isBeyondFnLedEnabled() == BEYOND_FN_LED_NL){
		if (isBeyondFN()) { // light up num lock on FN2 toggle
	        turnOffLED(LEDNUM);//PORTLEDS |= (1 << LEDNUM);	//
	    } else {
	        turnOnLED(LEDNUM);//PORTLEDS &= ~(1 << LEDNUM);	//
	    }
	}else{
		if (LEDstate & LED_STATE_NUM) { // light up num lock
	        turnOffLED(LEDNUM);//PORTLEDS &= ~(1 << LEDNUM);	//
	    }else{
	        turnOnLED(LEDNUM);//PORTLEDS &= ~(1 << LEDNUM);	//
	    }
	}
    if (LEDstate & LED_STATE_CAPS) { // light up caps lock
        turnOffLED(LEDCAPS); //PORTLEDS &= ~(1 << LEDCAPS);	//
    } else {
        turnOnLED(LEDCAPS); //PORTLEDS |= (1 << LEDCAPS);	//
    }
	/*
		_delay_ms()에 xStayMs를 인자로 넣으면 hex 파일의 용량이 0x1000가량 증가한다.
		매뉴얼 펑션으로 _delay_ms(1)을 ms 만큼 루프시키도록 만들어서 사용;
	*/
	__delay_ms(xStayMs);

	if(isBeyondFnLedEnabled() == BEYOND_FN_LED_NL){
		if (isBeyondFN()) { // light up num lock on FN2 toggle
	        turnOnLED(LEDNUM);//PORTLEDS |= (1 << LEDNUM);	//
	    } else {
	        turnOffLED(LEDNUM);//PORTLEDS &= ~(1 << LEDNUM);	//
	    }
	}else{
		if (LEDstate & LED_STATE_NUM) { // light up num lock
	        turnOnLED(LEDNUM);//PORTLEDS &= ~(1 << LEDNUM);	//
	    }else{
	        turnOffLED(LEDNUM);//PORTLEDS &= ~(1 << LEDNUM);	//
	    }
	}
    if (LEDstate & LED_STATE_CAPS) { // light up caps lock
        turnOnLED(LEDCAPS); //PORTLEDS |= (1 << LEDCAPS);	//
    } else {
        turnOffLED(LEDCAPS); //PORTLEDS &= ~(1 << LEDCAPS);	//
    }
}

void setLed(uint8_t xLed, bool xBool) {
	if (xBool) {
		if (xLed == LED_STATE_NUM) {
			turnOnLED(LEDNUM);
		} else if (xLed == LED_STATE_CAPS) {
			turnOnLED(LEDCAPS);
		}
#ifdef LEDSCROLL
		else if(xLed == LED_STATE_SCROLL) {
			turnOnLED(LEDSCROLL);
		}
#endif
	} else {
		if (xLed == LED_STATE_NUM) {
			turnOffLED(LEDNUM);
		} else if (xLed == LED_STATE_CAPS) {
			turnOffLED(LEDCAPS);
		}
#ifdef LEDSCROLL
		else if(xLed == LED_STATE_SCROLL) {
			turnOffLED(LEDSCROLL);
		}
#endif
	}
}
#endif

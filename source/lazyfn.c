
#include "lazyfn.h"
#include <avr/eeprom.h>
#include "eeprominfo.h"
// #include "print.h"
#include "macrobuffer.h"
#include "ps2avru_util.h"

// lazy fn
#define CMD_TOGGLE_LAZY_FN 1
#define CMD_EXIT_LAZY_FN 3
#define CMD_BACK_LAZY_FN 6

const char str_select_lazy_fn[] PROGMEM =  "lazy fn";

static bool _isLazyFn;

void printMenuLazyFn(void);
void printContentsLazyFn(void);
void putKeyindexLazyFn(uint8_t xCmd, uint8_t xKeyidx, uint8_t xCol, uint8_t xRow, uint8_t xIsDown);

keymapper_driver_t driverKeymapperLazyFn = {
   	printMenuLazyFn,
    printContentsLazyFn,
    putKeyindexLazyFn
};

static void printLazyFnState(void){
	printStringFromFlash(str_select_lazy_fn);
	printStringFromFlash(str_space);
	printStringFromFlash(str_colon);
	printStringFromFlash(str_space);
	if(isLazyFn()){
		printStringFromFlash(str_on);
	}else{
		printStringFromFlash(str_off);
	}
}

void printMenuLazyFn(void){
	printLazyFnState();
}

void printContentsLazyFn(void){
	printLazyFnState();
	printEnter();
	printString(toString(CMD_TOGGLE_LAZY_FN));
	printStringFromFlash(str_colon);
	printStringFromFlashWithEnter(str_toggle);
	printString(toString(CMD_EXIT_LAZY_FN));
	printStringFromFlash(str_colon);
	printStringFromFlashWithEnter(str_exit);	// exit
	printString(toString(CMD_BACK_LAZY_FN));
	printStringFromFlash(str_colon);
	printStringFromFlashWithEnter(str_back);
}

void putKeyindexLazyFn(uint8_t xCmd, uint8_t xKeyidx, uint8_t xCol, uint8_t xRow, uint8_t xIsDown){
	if(xCmd == CMD_TOGGLE_LAZY_FN){
		setStep(STEP_INPUT_COMMAND);
		toggleLazyFn();
		printLazyFnState();
		printEnter();
	}else if(xCmd == CMD_EXIT_LAZY_FN){
		setStep(STEP_EXIT);
		stopKeyMapping();
	}else if(xCmd == CMD_BACK_LAZY_FN){		
		setStep(STEP_BACK);
	}
}

void initLazyFn(void){
	_isLazyFn = getToggleOption(EEPROM_ENABLED_OPTION, TOGGLE_LAZY_FN);
	addKeymapperDriver(&driverKeymapperLazyFn);
}

bool isLazyFn(void){
	return _isLazyFn;
}

void toggleLazyFn(void){
	
	_isLazyFn ^= true;	
    setToggleOption(EEPROM_ENABLED_OPTION, TOGGLE_LAZY_FN, _isLazyFn);
}

#ifndef KEYMAP_C
#define KEYMAP_C

#include "keymap.h"
#include "print.h"
#include "macrobuffer.h"

#if !(KEYMAP_A87||KEYMAP_THUMB||KEYMAP_MX_MINI)
	#define KEYMAP_A87
#endif


const uint8_t dualActionMaskDown[] = {
    KEY_FN, // FN
    KEY_FN, // FN
    KEY_FN2, // FN2
    KEY_FN2, // FN2
    KEY_RSHIFT,
    KEY_RSHIFT,    
    KEY_LSHIFT,
    KEY_LSHIFT,
    KEY_RALT,
    KEY_RALT,
    KEY_LALT,
    KEY_LALT,
    KEY_RCTRL,
    KEY_RCTRL,
    KEY_LCTRL,
    KEY_LCTRL
};
const uint8_t dualActionMaskUp[] = {
    KEY_HANGLE, // hangle
    KEY_HANJA,	// hanja
    KEY_HANGLE, // hangle
    KEY_HANJA,	// hanja
    KEY_HANGLE,
    KEY_HANJA,	// hanja    
    KEY_HANGLE,
    KEY_HANJA,
    KEY_HANGLE,
    KEY_HANJA,
    KEY_HANGLE,
    KEY_HANJA,
    KEY_HANGLE,
    KEY_HANJA,
    KEY_HANGLE,
    KEY_HANJA
};

uint8_t dualActionKeyIndex = 0;

/*void setActiveDualAction(void){
	_isActiveDualAction = 1;
}*/


static uint8_t _isCanceledDualAction = 0;
uint8_t isCanceledDualAction(void)
{
	return _isCanceledDualAction;
}

static uint8_t _isActiveDualAction = 0;
// 모든키가 release 되었을 때 작동 시켜 준다.
void applyDualAction(void){
	if(!isAllKeyRelease()) return;

	_isActiveDualAction = 0;
	_isCanceledDualAction = 0;

    if(dualActionKeyIndex > 0 && !_isCanceledDualAction){
        // 듀얼액션이 저장되어 있을 때 아무키도 눌리지 않은 리포트가 간다면 액션!
       	uint8_t gActionIdx = dualActionMaskUp[dualActionKeyIndex - (KEY_dualAction + 1)];
        pushM(gActionIdx);
        pushM(gActionIdx);
        dualActionKeyIndex = 0;
    }
}

void setDualAction(uint8_t keyidx){
	if(keyidx > KEY_dualAction && keyidx < KEY_dualAction_end && !_isActiveDualAction){
        dualActionKeyIndex = keyidx;
        _isActiveDualAction = 1;
		// DEBUG_PRINT(("dualActionKeyIndex: %d \n", dualActionKeyIndex));
    }else if(dualActionKeyIndex > 0){
        // 듀얼액션이 저장되어 있을 때 아무 키나 눌리면 액션 중지;    
        // DEBUG_PRINT(("dualActionKeyIndex reset : %d\n", dualActionKeyIndex));    
        //dualActionKeyIndex = 0; 
        _isCanceledDualAction = 1;       
    }
}
// 듀얼액션 취소되었을 때는 다운 키코드를 적용한다.;
uint8_t getDualActionDownKeyIdex(uint8_t xActionIndex){
	if(xActionIndex > KEY_dualAction && xActionIndex < KEY_dualAction_end && isCanceledDualAction()){
        return dualActionMaskDown[xActionIndex - (KEY_dualAction + 1)];        
    }
    return xActionIndex;
}

// for ps/2 interface
const uint8_t PROGMEM keycode_set2[NUM_KEY] =	{ 
	0x00, 0x00, 0xFC, 0x00,
	0x1C, 0x32, 0x21, 0x23, 0x24, 0x2B, 0x34, 0x33, 0x43, 0x3B,	// abcd... 
	0x42, 0x4B, 0x3A, 0x31, 0x44, 0x4D, 0x15, 0x2D, 0x1B, 0x2C,	// klmn...
	0x3C, 0x2A, 0x1D, 0x22, 0x35, 0x1A, 0x16, 0x1E, 0x26, 0x25,	// uvwx...
	0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x45, 0x5A, 0x76, 0x66, 0x0D,	// 5678...
	0x29, 0x4E, 0x55, 0x54, 0x5B, 0x5D, 0x5D, 0x4C, 0x52, 0x0E,	// space-=[...
	0x41, 0x49, 0x4A, 0x58, 0x05, 0x06, 0x04, 0x0C, 0x03, 0x0B,	// ,./caps...
	0x83, 0x0A, 0x01, 0x09, 0x78, 0x07, 0x7C, 0x7E, 0x7C, 0x70,	// F7F8F9F10... ps sl pause INSERT
	0x6C, 0x7D, 0x71, 0x69, 0x7A, 0x74, 0x6B, 0x72, 0x75, 0x77,	// HomePuDel...UP NL
	0x4A, 0x7C, 0x7B, 0x79, 0x5A, 0x69, 0x72, 0x7A, 0x6B, 0x73,	// K/ K* ... K4 K5
	0x74, 0x6C, 0x75, 0x7D, 0x70, 0x71, 0x61, 0x2F,			// K6 K7 ... APPS		// d:101

	0x00,		// KEY_Modifiers
	0x14, 0x12, 0x11, 0x1F, 0x14, 0x59, 0x11, 0x27, 
	0x00,		// KEY_Modifiers_end

	0x00,  //KEY_Multimedia	
	0x37, 0x3F, 0x5E, 0x48, 0x10, 0x3A, 0x38, 0x30, 
	0x28, 0x20, 0x18, 0x4D, 0x15, 0x3B, 0x34, 0x23, 0x32, 0x21, 
	0x50, 0x2B, 0x40, 0x4B, 0x1C, 0x43, 0x22, 0x1D,
	0x00,   //KEY_Multimedia_end                    

    0x00,  //KEY_extend 
    0xF1, 0xF2,
    0x00   //KEY_extend_end                    // 144 keys
};

// usage page 01(Generic Desktop) : KEY_POWER,  KEY_SLEEP,  KEY_WAKE
// usage page 07(keyboard) : KEY_HANJA,   KEY_HANGLE
/* usage page 0C(Consumer Devices) : KEY_WWW_SEARCH, KEY_WWW_HOME, KEY_WWW_BACK, KEY_WWW_FORWARD, KEY_WWW_STOP, KEY_WWW_REFRESH, KEY_WWW_FAVORITE,KEY_EMAIL,  
                                    KEY_NEXT_TRK, KEY_PREV_TRK, KEY_STOP, KEY_PLAY, KEY_MUTE, KEY_VOL_UP, KEY_VOL_DOWN, KEY_MEDIA,  KEY_CALC,   KEY_MYCOM */
//
// windows OS 에서 미지원 키 : KEY_SCREENSAVE,    KEY_REC,    KEY_REWIND,     KEY_MINIMIZE,   KEY_EJECT

const uint16_t PROGMEM keycode_USB_multimedia[] =	{ 
	0x0000, 0x0000, 0x0000, 0x018A, 0x0221, 0x0223, 0x0224, 0x0225,
	0x0226, 0x0227, 0x022A, 0x00B5, 0x00B6, 0x00B7, 0x00CD, 0x00E2, 0x00E9, 0x00EA, 
	0x0183, 0x0192, 0x0194, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
// KEY_APPS까지는 keycode 순서에 맞게 나열되어 있지만, 
// KEY_extend는 배열 순서와는 다른 키코드를 갖는 usage page 07(keyboard)의 키코드를 정의.
const uint16_t PROGMEM keycode_USB_extend[] =   { 
    0x91, 0x90
};


const uint8_t PROGMEM keycode_set2_special[] = 
{ 	KEY_PRNSCR, KEY_PAUSE,
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_makeonly[] = 
{ 	KEY_PAUSE, KEY_HANGLE, KEY_HANJA,
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_make_break[] =
{ 	KEY_POWER, KEY_SLEEP, KEY_WAKE,
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_extend[] =
{	KEY_LGUI, KEY_RCTRL, KEY_RGUI, KEY_RALT, KEY_APPS, KEY_PRNSCR,
	KEY_INSERT, KEY_HOME, KEY_PGUP, KEY_DEL, KEY_END, KEY_PGDN, 
	KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_DOWN, KEY_KP_SLASH, KEY_KP_ENTER,
	KEY_POWER, KEY_SLEEP, KEY_WAKE, KEY_EMAIL, KEY_WWW_SEARCH, KEY_WWW_HOME,
	KEY_WWW_BACK, KEY_WWW_FORWARD, KEY_WWW_STOP, KEY_WWW_REFRESH, KEY_WWW_FAVORITE,
	KEY_NEXT_TRK, KEY_PREV_TRK, KEY_STOP, KEY_PLAY, KEY_MUTE, KEY_VOL_UP, 
	KEY_VOL_DOWN, KEY_MEDIA, KEY_CALC, KEY_MYCOM, KEY_SCREENSAVE, KEY_REC,
	KEY_REWIND, KEY_MINIMIZE, KEY_EJECT, 
	KEY_NONE };

const uint8_t PROGMEM keycode_set2_proc_shift[] = 
{
	KEY_INSERT, KEY_DEL, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN, KEY_LGUI, KEY_RGUI, KEY_APPS,
 	KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_KP_SLASH,
	KEY_NONE };


const uint8_t PROGMEM keymap_code[3][17][8] =  {

	#ifdef KEYMAP_A87
	// a.87 ps2avr, a.87U, mx-miniU, a.87U ex
	{
		// set 1 : normal
		// COL0			COL1		COL2		COL3		COL4		COL5		COL6		COL7
		{ KEY_PAUSE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RCTRL,	KEY_NONE,	KEY_LCTRL,	KEY_F5 },	// ROW0
		{ KEY_Q,		KEY_TAB,	KEY_A,		KEY_ESC,	KEY_Z,		KEY_NONE,	KEY_HASH,	KEY_1 },	// ROW1
		{ KEY_W,		KEY_CAPS,	KEY_S,		KEY_NONE,	KEY_X,		KEY_NONE,	KEY_F1, 	KEY_2 },	// ROW2
		{ KEY_E,		KEY_F3, 	KEY_D,		KEY_F4, 	KEY_C,		KEY_NONE,	KEY_F2, 	KEY_3 },	// ROW3
		{ KEY_R,		KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_5,		KEY_4 },	// ROW4
		{ KEY_U,		KEY_Y,		KEY_J,		KEY_H,		KEY_M,		KEY_N,		KEY_6,		KEY_7 },	// ROW5
		{ KEY_I,		KEY_RBR,	KEY_K,		KEY_F6, 	KEY_COMMA,	KEY_NONE,	KEY_EQUAL,	KEY_8 },	// ROW6
		{ KEY_O,		KEY_F7, 	KEY_L,		KEY_NONE,	KEY_DOT,	KEY_FN,		KEY_F8, 	KEY_9 },	// ROW7
		{ KEY_P,		KEY_LBR,	KEY_COLON,	KEY_QUOTE,	KEY_Europe1,KEY_SLASH,	KEY_MINUS,	KEY_0}, 	// ROW8
		{ KEY_SCRLCK,	KEY_NONE,	KEY_NONE,	KEY_LALT,	KEY_Europe2,KEY_RALT,	KEY_NONE,	KEY_PRNSCR},// ROW9
		{ KEY_NONE, 	KEY_BKSP,	KEY_BKSLASH,KEY_F11,	KEY_ENTER,	KEY_F12,	KEY_F9, 	KEY_F10},	// ROW10
		{ KEY_MAC1, 	KEY_NUMLOCK,KEY_KP_7,	KEY_SPACE,	KEY_KP_4,	KEY_DOWN,	KEY_DEL,	KEY_NONE }, // ROW11
		{ KEY_MAC2, 	KEY_KP_SLASH,KEY_KP_8,	KEY_KP_5,	KEY_KP_2,	KEY_RIGHT,	KEY_INSERT, KEY_NONE }, // ROW12
		{ KEY_MAC3, 	KEY_KP_AST,	KEY_KP_9,	KEY_KP_6,	KEY_KP_3,	KEY_NONE,	KEY_PGUP,	KEY_PGDN }, // ROW13
		{ KEY_MAC4, 	KEY_KP_MINUS,KEY_KP_PLUS,KEY_UP, 	KEY_KP_ENTER,KEY_LEFT,	KEY_HOME,	KEY_END },	// ROW14
		{ KEY_KP_0, 	KEY_LSHIFT, KEY_RSHIFT, KEY_KP_1,	KEY_KP_DOT,KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW15
		{ KEY_NONE, 	KEY_LGUI,	KEY_RGUI,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
	},
	
	{	
		// set 2 : FN
		// COL0			COL1		COL2		COL3		COL4		COL5		COL6		COL7
		{ KEY_NUMLOCK,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RCTRL,	KEY_NONE,	KEY_LCTRL,	KEY_F5 },	// ROW0
		{ KEY_Q,		KEY_TAB,	KEY_A,		KEY_ESC,	KEY_Z,		KEY_NONE,	KEY_HASH,	KEY_1 },	// ROW1
		{ KEY_W,		KEY_LED,	KEY_S,		KEY_NONE,	KEY_X,		KEY_NONE,	KEY_F1, 	KEY_2 },	// ROW2
		{ KEY_E,		KEY_F3, 	KEY_D,		KEY_F4, 	KEY_C,		KEY_NONE,	KEY_F2, 	KEY_3 },	// ROW3
		{ KEY_R,		KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_5,		KEY_4 },	// ROW4
		{ KEY_KP_4,		KEY_Y,		KEY_KP_1,	KEY_H,		KEY_KP_0,	KEY_N,		KEY_6,		KEY_KP_7 },	// ROW5
		{ KEY_KP_5,		KEY_RBR,	KEY_KP_2,	KEY_F6, 	KEY_KP_0,	KEY_NONE,	KEY_LED_UP,	KEY_KP_8 },	// ROW6
		{ KEY_KP_6,		KEY_F7, 	KEY_KP_3,	KEY_NONE,	KEY_KP_DOT,	KEY_FN,		KEY_F8, 	KEY_KP_9 },	// ROW7
		{ KEY_KP_AST,	KEY_LBR,	KEY_KP_MINUS,KEY_QUOTE,	KEY_Europe1,KEY_KP_PLUS,KEY_LED_DOWN,KEY_KP_SLASH}, 	// ROW8
		{ KEY_SCRLCK,	KEY_NONE,	KEY_NONE,	KEY_LALT,	KEY_Europe2,KEY_RALT,	KEY_NONE,	KEY_PRNSCR},// ROW9
		{ KEY_NONE, 	KEY_BKSP,	KEY_BKSLASH,KEY_F11,	KEY_KP_ENTER,KEY_F12,	KEY_F9, 	KEY_F10},	// ROW10
		{ KEY_MAC1, 	KEY_NUMLOCK,KEY_KP_7,	KEY_BEYOND_FN,KEY_KP_4,	KEY_DOWN,	KEY_DEL,	KEY_NONE }, // ROW11
		{ KEY_MAC2, 	KEY_KP_SLASH,KEY_KP_8,	KEY_KP_5,	KEY_KP_2,	KEY_RIGHT,	KEY_INSERT, KEY_NONE }, // ROW12
		{ KEY_MAC3, 	KEY_KP_AST,	KEY_KP_9,	KEY_KP_6,	KEY_KP_3,	KEY_NONE,	KEY_PGUP,	KEY_PGDN }, // ROW13
		{ KEY_MAC4, 	KEY_KP_MINUS,KEY_KP_PLUS,KEY_UP, 	KEY_KP_ENTER,	KEY_LEFT,	KEY_HOME,	KEY_END },	// ROW14
		{ KEY_KP_0, 	KEY_LSHIFT, KEY_RSHIFT, KEY_KP_1,	KEY_KP_DOT,KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW15
		{ KEY_NONE, 	KEY_LGUI,	KEY_RGUI,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
	},
	
	{	
		// set 3 : BEYOND_FN
		// COL0			COL1		COL2		COL3		COL4		COL5		COL6		COL7
		{ KEY_PAUSE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RCTRL,	KEY_NONE,	KEY_LCTRL,	KEY_F5 },	// ROW0
		{ KEY_Q,		KEY_TAB,	KEY_A,		KEY_ESC,	KEY_Z,		KEY_NONE,	KEY_HASH,	KEY_1 },	// ROW1
		{ KEY_W,		KEY_CAPS,	KEY_S,		KEY_NONE,	KEY_X,		KEY_NONE,	KEY_F1, 	KEY_2 },	// ROW2
		{ KEY_E,		KEY_F3, 	KEY_D,		KEY_F4, 	KEY_C,		KEY_NONE,	KEY_F2, 	KEY_3 },	// ROW3
		{ KEY_R,		KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_5,		KEY_4 },	// ROW4
		{ KEY_U,		KEY_Y,		KEY_J,		KEY_H,		KEY_M,		KEY_N,		KEY_6,		KEY_7 },	// ROW5
		{ KEY_I,		KEY_RBR,	KEY_K,		KEY_F6, 	KEY_COMMA,	KEY_NONE,	KEY_EQUAL,	KEY_8 },	// ROW6
		{ KEY_O,		KEY_F7, 	KEY_L,		KEY_NONE,	KEY_DOT,	KEY_FN,		KEY_F8, 	KEY_9 },	// ROW7
		{ KEY_P,		KEY_LBR,	KEY_COLON,	KEY_QUOTE,	KEY_Europe1,KEY_SLASH,	KEY_MINUS,	KEY_0}, 	// ROW8
		{ KEY_SCRLCK,	KEY_NONE,	KEY_NONE,	KEY_LALT,	KEY_Europe2,KEY_RALT,	KEY_NONE,	KEY_PRNSCR},// ROW9
		{ KEY_NONE, 	KEY_BKSP,	KEY_BKSLASH,KEY_F11,	KEY_ENTER,	KEY_F12,	KEY_F9, 	KEY_F10},	// ROW10
		{ KEY_MAC1, 	KEY_NUMLOCK,KEY_KP_7,	KEY_SPACE,	KEY_KP_4,	KEY_DOWN,	KEY_DEL,	KEY_NONE }, // ROW11
		{ KEY_MAC2, 	KEY_KP_SLASH,KEY_KP_8,	KEY_KP_5,	KEY_KP_2,	KEY_RIGHT,	KEY_INSERT, KEY_NONE }, // ROW12
		{ KEY_MAC3, 	KEY_KP_AST,	KEY_KP_9,	KEY_KP_6,	KEY_KP_3,	KEY_NONE,	KEY_PGUP,	KEY_PGDN }, // ROW13
		{ KEY_MAC4, 	KEY_KP_MINUS,KEY_KP_PLUS,KEY_UP, 	KEY_KP_ENTER,KEY_LEFT,	KEY_HOME,	KEY_END },	// ROW14
		{ KEY_KP_0, 	KEY_LSHIFT, KEY_RSHIFT, KEY_KP_1,	KEY_KP_DOT,KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW15
		{ KEY_NONE, 	KEY_LGUI,	KEY_RGUI,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
	}
	#endif

	#ifdef KEYMAP_THUMB
	// thumb, face
	{
		// set 1 : normal
		// COL0			COL1		COL2		COL3		COL4		COL5		COL6		COL7
		{ KEY_NONE,		KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE },	// ROW0
		//
		{ KEY_Q,		KEY_TAB,	KEY_A,		KEY_NONE,	KEY_Z,		KEY_NONE,	KEY_ESC,	KEY_1 },	// ROW1
		{ KEY_W,		KEY_CAPS,	KEY_S,		KEY_NONE,	KEY_X,		KEY_NONE,	KEY_NONE, 	KEY_2 },	// ROW2
		{ KEY_E,		KEY_Europe2,KEY_D,		KEY_NONE, 	KEY_C,		KEY_NONE,	KEY_NONE, 	KEY_3 },	// ROW3
		{ KEY_R,		KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_5,		KEY_4 },	// ROW4
		{ KEY_U,		KEY_Y,		KEY_J,		KEY_H,		KEY_M,		KEY_N,		KEY_6,		KEY_7 },	// ROW5
		{ KEY_I,		KEY_RBR,	KEY_K,		KEY_NONE, 	KEY_COMMA,	KEY_NONE,	KEY_EQUAL,	KEY_8 },	// ROW6
		{ KEY_O,		KEY_NONE, 	KEY_L,		KEY_NONE,	KEY_DOT,	KEY_FN,		KEY_NONE, 	KEY_9 },	// ROW7
		{ KEY_P,		KEY_LBR,	KEY_COLON,	KEY_QUOTE,	KEY_Europe1,KEY_SLASH,	KEY_MINUS,	KEY_0}, 	// ROW8
		{ KEY_LCTRL,	KEY_LSHIFT,	KEY_RCTRL,	KEY_LALT,	KEY_RSHIFT,	KEY_RALT,	KEY_APPS,	KEY_NONE},	// ROW9
		{ KEY_LGUI, 	KEY_HASH,	KEY_BKSLASH,KEY_SPACE,	KEY_ENTER,	KEY_FN,		KEY_BKSP, 	KEY_NONE},	// ROW10
		//
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW11
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE, 	KEY_NONE }, // ROW12
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW13
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE },	// ROW14
		{ KEY_NONE, 	KEY_NONE, 	KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW15
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
	},
	
	{	
		// set 2 : FN
		// COL0			COL1		COL2		COL3		COL4		COL5		COL6		COL7
		{ KEY_NONE,		KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE },	// ROW0
		//
		{ KEY_Q,		KEY_HASH,	KEY_LEFT,	KEY_NONE,	KEY_Z,		KEY_NONE,	KEY_ESC,	KEY_F1 },	// ROW1
		{ KEY_UP,		KEY_LED,	KEY_DOWN,	KEY_NONE,	KEY_X,		KEY_NONE,	KEY_NONE, 	KEY_F2 },	// ROW2
		{ KEY_E,		KEY_Europe2,KEY_RIGHT,	KEY_NONE, 	KEY_C,		KEY_NONE,	KEY_NONE, 	KEY_F3 },	// ROW3
		{ KEY_R,		KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_F5,		KEY_F4 },	// ROW4
		{ KEY_NUMLOCK,	KEY_Y,		KEY_J,		KEY_H,		KEY_M,		KEY_N,		KEY_F6,		KEY_F7 },	// ROW5
		{ KEY_PRNSCR,	KEY_INSERT,	KEY_HOME,	KEY_NONE, 	KEY_END,	KEY_NONE,	KEY_F12,	KEY_F8 },	// ROW6
		{ KEY_SCRLCK,	KEY_NONE, 	KEY_PGUP,	KEY_NONE,	KEY_PGDN,	KEY_FN,		KEY_NONE, 	KEY_F9 },	// ROW7
		{ KEY_PAUSE,	KEY_UP,		KEY_LEFT,	KEY_RIGHT,	KEY_Europe1,KEY_DOWN,	KEY_F11,	KEY_F10}, 	// ROW8
		{ KEY_LCTRL,	KEY_LSHIFT,	KEY_LED_UP,	KEY_LALT,	KEY_RSHIFT,	KEY_LED_DOWN,KEY_APPS,	KEY_NONE},	// ROW9
		{ KEY_LGUI, 	KEY_HASH,	KEY_BKSLASH,KEY_BEYOND_FN,KEY_ENTER,KEY_FN,		KEY_DEL, 	KEY_NONE},	// ROW10
		//
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW11
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE, 	KEY_NONE }, // ROW12
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW13
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE },	// ROW14
		{ KEY_NONE, 	KEY_NONE, 	KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW15
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
	},
	
	{	
		// set 3 : BEYOND_FN
		// COL0			COL1		COL2		COL3		COL4		COL5		COL6		COL7
		{ KEY_NONE,		KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE },	// ROW0
		//
		{ KEY_Q,		KEY_TAB,	KEY_A,		KEY_NONE,	KEY_Z,		KEY_NONE,	KEY_ESC,	KEY_1 },	// ROW1
		{ KEY_W,		KEY_CAPS,	KEY_S,		KEY_NONE,	KEY_X,		KEY_NONE,	KEY_NONE, 	KEY_2 },	// ROW2
		{ KEY_E,		KEY_Europe2,KEY_D,		KEY_NONE, 	KEY_C,		KEY_NONE,	KEY_NONE, 	KEY_3 },	// ROW3
		{ KEY_R,		KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_5,		KEY_4 },	// ROW4
		{ KEY_U,		KEY_Y,		KEY_J,		KEY_H,		KEY_M,		KEY_N,		KEY_6,		KEY_7 },	// ROW5
		{ KEY_I,		KEY_RBR,	KEY_K,		KEY_NONE, 	KEY_COMMA,	KEY_NONE,	KEY_EQUAL,	KEY_8 },	// ROW6
		{ KEY_O,		KEY_NONE, 	KEY_L,		KEY_NONE,	KEY_DOT,	KEY_DOWN,	KEY_NONE, 	KEY_9 },	// ROW7
		{ KEY_P,		KEY_LBR,	KEY_COLON,	KEY_QUOTE,	KEY_Europe1,KEY_SLASH,	KEY_MINUS,	KEY_0}, 	// ROW8
		{ KEY_LCTRL,	KEY_LSHIFT,	KEY_RIGHT,	KEY_LALT,	KEY_UP,		KEY_LEFT,	KEY_LEFT,	KEY_NONE},	// ROW9
		{ KEY_LGUI, 	KEY_HASH,	KEY_BKSLASH,KEY_SPACE,	KEY_ENTER,	KEY_FN,		KEY_BKSP, 	KEY_NONE},	// ROW10
		//
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW11
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE, 	KEY_NONE }, // ROW12
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW13
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE },	// ROW14
		{ KEY_NONE, 	KEY_NONE, 	KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW15
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
	}
	#endif

	#ifdef KEYMAP_MX_MINI
	// mx_mini ps2avr V0.2
	{
		// set 1 : normal
		// COL0			COL1		COL2		COL3		COL4		COL5		COL6		COL7
		{ KEY_PAUSE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RCTRL,	KEY_NONE,	KEY_LCTRL,	KEY_F5 },	// ROW0
		{ KEY_Q,		KEY_TAB,	KEY_A,		KEY_ESC,	KEY_Z,		KEY_NONE,	KEY_HASH,	KEY_1 },	// ROW1
		{ KEY_W,		KEY_CAPS,	KEY_S,		KEY_NONE,	KEY_X,		KEY_NONE,	KEY_F1, 	KEY_2 },	// ROW2
		{ KEY_E,		KEY_F3, 	KEY_D,		KEY_F4, 	KEY_C,		KEY_NONE,	KEY_F2, 	KEY_3 },	// ROW3
		{ KEY_R,		KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_5,		KEY_4 },	// ROW4
		{ KEY_U,		KEY_Y,		KEY_J,		KEY_H,		KEY_M,		KEY_N,		KEY_6,		KEY_7 },	// ROW5
		{ KEY_I,		KEY_RBR,	KEY_K,		KEY_F6, 	KEY_COMMA,	KEY_NONE,	KEY_EQUAL,	KEY_8 },	// ROW6
		{ KEY_O,		KEY_F7, 	KEY_L,		KEY_NONE,	KEY_DOT,	KEY_FN,		KEY_F8, 	KEY_9 },	// ROW7
		{ KEY_P,		KEY_LBR,	KEY_COLON,	KEY_QUOTE,	KEY_NONE,	KEY_SLASH,	KEY_MINUS,	KEY_0}, 	// ROW8
		{ KEY_SCRLCK,	KEY_NONE,	KEY_NONE,	KEY_LALT,	KEY_NONE,	KEY_RALT,	KEY_NONE,	KEY_PRNSCR},// ROW9
		{ KEY_NONE, 	KEY_BKSP,	KEY_BKSLASH,KEY_F11,	KEY_ENTER,	KEY_F12,	KEY_F9, 	KEY_F10},	// ROW10
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_SPACE,	KEY_NONE,	KEY_DOWN,	KEY_HOME,	KEY_NONE }, // ROW11
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RIGHT,	KEY_INSERT, KEY_NONE }, // ROW12
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_PGUP,	KEY_PGDN }, // ROW13
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_UP, 	KEY_NONE,	KEY_LEFT,	KEY_DEL,	KEY_END },	// ROW14
		{ KEY_NONE, 	KEY_LSHIFT, KEY_RSHIFT, KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW15
		{ KEY_NONE, 	KEY_LGUI,	KEY_RGUI,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
	},
	
	{	
		// set 2 : FN
		// COL0			COL1		COL2		COL3		COL4		COL5		COL6		COL7
		{ KEY_NUMLOCK,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RCTRL,	KEY_NONE,	KEY_LCTRL,	KEY_F5 },	// ROW0
		{ KEY_Q,		KEY_TAB,	KEY_A,		KEY_ESC,	KEY_Z,		KEY_NONE,	KEY_HASH,	KEY_1 },	// ROW1
		{ KEY_W,		KEY_LED,	KEY_S,		KEY_NONE,	KEY_X,		KEY_NONE,	KEY_F1, 	KEY_2 },	// ROW2
		{ KEY_E,		KEY_F3, 	KEY_D,		KEY_F4, 	KEY_C,		KEY_NONE,	KEY_F2, 	KEY_3 },	// ROW3
		{ KEY_R,		KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_5,		KEY_4 },	// ROW4
		{ KEY_KP_4,		KEY_Y,		KEY_KP_1,	KEY_H,		KEY_KP_0,	KEY_N,		KEY_6,		KEY_KP_7 },	// ROW5
		{ KEY_KP_5,		KEY_RBR,	KEY_KP_2,	KEY_F6, 	KEY_KP_0,	KEY_NONE,	KEY_LED_UP,	KEY_KP_8 },	// ROW6
		{ KEY_KP_6,		KEY_F7, 	KEY_KP_3,	KEY_NONE,	KEY_KP_DOT,	KEY_FN,		KEY_F8, 	KEY_KP_9 },	// ROW7
		{ KEY_KP_AST,	KEY_LBR,	KEY_KP_MINUS,KEY_QUOTE,	KEY_NONE,	KEY_KP_PLUS,KEY_LED_DOWN,	KEY_KP_SLASH}, 	// ROW8
		{ KEY_SCRLCK,	KEY_NONE,	KEY_NONE,	KEY_LALT,	KEY_NONE,	KEY_RALT,	KEY_NONE,	KEY_PRNSCR},// ROW9
		{ KEY_NONE, 	KEY_BKSP,	KEY_BKSLASH,KEY_F11,	KEY_KP_ENTER,KEY_F12,	KEY_F9, 	KEY_F10},	// ROW10
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_SPACE,	KEY_NONE,	KEY_DOWN,	KEY_HOME,	KEY_NONE }, // ROW11
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RIGHT,	KEY_INSERT, KEY_NONE }, // ROW12
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_PGUP,	KEY_PGDN }, // ROW13
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_UP, 	KEY_NONE,	KEY_LEFT,	KEY_DEL,	KEY_END },	// ROW14
		{ KEY_NONE, 	KEY_LSHIFT, KEY_RSHIFT, KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW15
		{ KEY_NONE, 	KEY_LGUI,	KEY_RGUI,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
	},
	
	{	
		// set 3 : BEYOND_FN
		// COL0			COL1		COL2		COL3		COL4		COL5		COL6		COL7
		{ KEY_PAUSE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RCTRL,	KEY_NONE,	KEY_LCTRL,	KEY_F5 },	// ROW0
		{ KEY_Q,		KEY_TAB,	KEY_A,		KEY_ESC,	KEY_Z,		KEY_NONE,	KEY_HASH,	KEY_1 },	// ROW1
		{ KEY_W,		KEY_CAPS,	KEY_S,		KEY_NONE,	KEY_X,		KEY_NONE,	KEY_F1, 	KEY_2 },	// ROW2
		{ KEY_E,		KEY_F3, 	KEY_D,		KEY_F4, 	KEY_C,		KEY_NONE,	KEY_F2, 	KEY_3 },	// ROW3
		{ KEY_R,		KEY_T,		KEY_F,		KEY_G,		KEY_V,		KEY_B,		KEY_5,		KEY_4 },	// ROW4
		{ KEY_U,		KEY_Y,		KEY_J,		KEY_H,		KEY_M,		KEY_N,		KEY_6,		KEY_7 },	// ROW5
		{ KEY_I,		KEY_RBR,	KEY_K,		KEY_F6, 	KEY_COMMA,	KEY_NONE,	KEY_EQUAL,	KEY_8 },	// ROW6
		{ KEY_O,		KEY_F7, 	KEY_L,		KEY_NONE,	KEY_DOT,	KEY_FN,		KEY_F8, 	KEY_9 },	// ROW7
		{ KEY_P,		KEY_LBR,	KEY_COLON,	KEY_QUOTE,	KEY_NONE,	KEY_SLASH,	KEY_MINUS,	KEY_0}, 	// ROW8
		{ KEY_SCRLCK,	KEY_NONE,	KEY_NONE,	KEY_LALT,	KEY_NONE,	KEY_RALT,	KEY_NONE,	KEY_PRNSCR},// ROW9
		{ KEY_NONE, 	KEY_BKSP,	KEY_BKSLASH,KEY_F11,	KEY_ENTER,	KEY_F12,	KEY_F9, 	KEY_F10},	// ROW10
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_SPACE,	KEY_NONE,	KEY_DOWN,	KEY_HOME,	KEY_NONE }, // ROW11
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_RIGHT,	KEY_INSERT, KEY_NONE }, // ROW12
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_PGUP,	KEY_PGDN }, // ROW13
		{ KEY_NONE, 	KEY_NONE,	KEY_NONE,	KEY_UP, 	KEY_NONE,	KEY_LEFT,	KEY_DEL,	KEY_END },	// ROW14
		{ KEY_NONE, 	KEY_LSHIFT, KEY_RSHIFT, KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }, // ROW15
		{ KEY_NONE, 	KEY_LGUI,	KEY_RGUI,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE,	KEY_NONE }	// ROW16
	}
	#endif

 };


#endif

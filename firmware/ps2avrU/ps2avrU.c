
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/wdt.h>

#include "main.h"
#include "keymatrix.h"
#include "hardwareinfo.h"
#include "boot.h"


/* ----------------------- hardware I/O abstraction ------------------------ */
#define PORTCOLUMNS PORTB  ///< port on which we read the state of the columns
#define PINCOLUMNS  PINB   ///< port on which we read the state of the columns
#define DDRCOLUMNS  DDRB   ///< port on which we read the state of the columns
#define PORTROWS1   PORTA  ///< first port connected to the matrix rows
#define PINROWS1    PINA   ///< first port connected to the matrix rows
#define DDRROWS1    DDRA   ///< first port connected to the matrix rows
#define PORTROWS2   PORTC  ///< second port connected to the matrix rows
#define PINROWS2    PINC   ///< second port connected to the matrix rows
#define DDRROWS2    DDRC   ///< second port connected to the matrix rows

// _______________________________________________________________ boot
void delegateGotoBootloader(void){
	wdt_enable(WDTO_15MS);
	for(;;);

	//        	typedef void (*AppPtr_t)(void) __attribute__ ((noreturn));
	//
	//        	AppPtr_t AppStartPtr = (AppPtr_t)(BOOTLOADER_START);
	//        	AppStartPtr();
	//        	((void (*)(void))(BOOTLOADER_START))();
}

// _______________________________________________________________ keymain
uint8_t delegateGetBootmapperStatus(uint8_t xCol, uint8_t xRow){
	if(xCol == 1 && xRow == 1){
		return 1;
	}else{
		return 0;
	}
}

// _______________________________________________________________ keymatrix
void delegateInitMatrixDevice(void){
	// initialize matrix ports - cols, rows
	// PB0-PB7 : col0 .. col7
	// PA0-PA7 : row0 .. row7
	// PC7-PC0 : row8 .. row15
	
	// PD0 : NUM
    // PD1 : CAPS
    // PD2 : D+ / Clock
    // PD3 : D- / Data
    // PD4 : FULL LED
    // PD5 : 3.6V switch TR
	// PD6 : SCRL
    // PD7 : row17
	

	// signal direction : col -> row

	DDRCOLUMNS 	= 0xFF;	// all outputs for cols
	PORTCOLUMNS	= 0xFF;	// high
	DDRROWS1	= 0x00;	// all inputs for rows
	DDRROWS2	= 0x00;
	PORTROWS1	= 0xFF;	// all rows pull-up.
	PORTROWS2	= 0xFF;	
  
	DDRD        &= ~(1<<PIND7); // input row 17
	PORTD 		|= (1<<PIND7);// pull up row 17
}

void delegateSetCellStatus(uint8_t xCol){
	// Col -> set only one port as input and all others as output low
	DDRCOLUMNS  = BV(xCol);	// 해당 col을 출력으로 설정, 나머지 입력
	PORTCOLUMNS = ~BV(xCol);	// 해당 col output low, 나머지 컬럼을 풀업 저항

	/*
	DDR을 1로 설정하면 출력, 0이면 입력
	입력중, PORT가 1이면 풀업(풀업 상태는  high 상태);

	출력 상태의 PORT가 0이면 output low(0v);

	스위치를 on하면 0, off하면 1이 PIN에 저장;
	row는 내부 풀업 저항 상태 이기 때문에 1값이 기본값
	*/
				
	_delay_us(5);
}

uint8_t delegateGetCellStatus(uint8_t xRow){

	if(xRow<8)	{				// for 0..7, PORTA 0 -> 7
		return (~PINROWS1) & BV(xRow);
	}
	else if(xRow<16) {	// for 8..15, PORTC 7 -> 0
		return (~PINROWS2) & BV(15-xRow);
	}
	else {						// for 16, PORTD 7
		return (~PIND) & BV(23-xRow);
	}

}


void delegateGetLiveMatrix(uint8_t *xCurrentMatrix, uint8_t *xIsModified){
	uint8_t col, row;
	uint8_t prev, cur;

	for(col=0;col<COLUMNS;col++)
	{
		delegateSetCellStatus(col);

		// scan each rows
		for(row=0;row<ROWS;row++)
		{
			cur = delegateGetCellStatus(row);

			prev = xCurrentMatrix[row] & BV(col);

			if(!(prev && cur) && !(!prev && !cur)) {
				if(cur)
					xCurrentMatrix[row] |= BV(col);
				else
					xCurrentMatrix[row] &=~ BV(col);

				*xIsModified = 1;
			}
		}

	}

}



# Directory common source filess exist
TOP_DIR = ../..
TIMER = timer

ifndef RESULT
	RESULT = ps2avrU
endif

#DEBUG_LEVEL = 1

########### change this lines according to your project ##################
#put the name of the target mcu here (at90s8515, at90s8535, attiny22, atmega603 etc.)
MCU = atmega32
#	MCU = atmega161
#	MCU = atmega128

F_CPU = 12000000

SRC = ledrender_ps2avrU.c \
	keymatrix.c \
	vusb.c \
	ps2avrU.c

include $(TOP_DIR)/common.mk
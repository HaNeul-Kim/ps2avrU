# Makefile for AVR function library development and examples
# Author: Pascal Stang
#
# For those who have never heard of makefiles: a makefile is essentially a
# script for compiling your code.  Most C/C++ compilers in the world are
# command line programs and this is even true of programming environments
# which appear to be windows-based (like Microsoft Visual C++).  Although
# you could use AVR-GCC directly from the command line and try to remember
# the compiler options each time, using a makefile keeps you free of this
# tedious task and automates the process.
#
# For those just starting with AVR-GCC and not used to using makefiles,
# I've added some extra comments above several of the makefile fields which
# you will have to deal with.

	AVRLIB = $(TOP_DIR)/avrlib
	COMMON_DIR = $(TOP_DIR)/source


#put the name of the target file here (without extension)
#  Your "target" file is your C source file that is at the top level of your code.
#  In other words, this is the file which contains your main() function.

	TRG = main
	

# debug

ifdef DEBUG_LEVEL
    OPT_DEFS += -DDEBUG_LEVEL=$(DEBUG_LEVEL)
	SRC +=  $(COMMON_DIR)/usbdrv/oddebug.c
endif

# Option
ifdef SCROLL_LOCK_LED_IS_APART
    OPT_DEFS += -DSCROLL_LOCK_LED_IS_APART
endif

ifdef GHOST_KEY_PREVENTION
    OPT_DEFS += -DGHOST_KEY_PREVENTION
endif

ifdef DISABLE_HARDWARE_KEYMAPPING
    OPT_DEFS += -DDISABLE_HARDWARE_KEYMAPPING
endif

ifdef INTERFACE_ONLY_USB
    OPT_DEFS += -DINTERFACE_ONLY_USB
else
    SRC +=  $(COMMON_DIR)/keysta.c \
            $(COMMON_DIR)/ps2main.c
endif

ifndef USING_CUSTOM_TIMER
	ifndef TIMER	
		TIMER = timer
	endif

	SRC +=  $(AVRLIB)/$(TIMER).c
endif

#put your C sourcefiles here 
#  Here you must list any C source files which are used by your target file.
#  They will be compiled in the order you list them, so it's probably best
#  to list $(TRG).c, your top-level target file, last.

	SRC +=  $(COMMON_DIR)/custommacro.c \
			$(COMMON_DIR)/keyscan.c \
			$(COMMON_DIR)/quickswap.c \
			$(COMMON_DIR)/dualaction.c \
			$(COMMON_DIR)/keydownbuffer.c \
			$(COMMON_DIR)/keyindex.c \
			$(COMMON_DIR)/keymap.c \
			$(COMMON_DIR)/sleep.c \
			$(COMMON_DIR)/keymapper.c \
			$(COMMON_DIR)/ps2avru_util.c \
			$(COMMON_DIR)/macrobuffer.c \
			$(COMMON_DIR)/bootmapper.c \
			$(COMMON_DIR)/enterframe.c \
			$(COMMON_DIR)/fncontrol.c \
			$(COMMON_DIR)/esctilde.c \
			$(COMMON_DIR)/lazyfn.c \
			$(COMMON_DIR)/usbmain.c \
			$(COMMON_DIR)/usbdrv/usbdrv.c \
			$(COMMON_DIR)/$(TRG).c \
			
#put additional assembler source file here
#  The ASRC line allows you to list files which contain assembly code/routines that
#  you would like to use from within your C programs.  The assembly code must be
#  written in a special way to be usable as a function from your C code.

	ASRC = $(COMMON_DIR)/usbdrv/usbdrvasm.s

#additional libraries and object files to link
#  Libraries and object files are collections of functions which have already been
#  compiled.  If you have such files, list them here, and you will be able to use
#  use the functions they contain in your target program.

	LIB	=

#additional includes to compile
	INC	= $(COMMON_DIR)/usbdrv

#assembler flags
	ASFLAGS = -Wa, -gstabs -DF_CPU=$(F_CPU) -I$(COMMON_DIR)

#compiler flags
	CPFLAGS	= -g -Os -Wall -Wstrict-prototypes -I$(COMMON_DIR) -I$(INC) -I$(AVRLIB) $(LIBSRC) -DF_CPU=$(F_CPU)UL $(OPT_DEFS) -Wa,-ahlms=$(<:.c=.lst)
#	CPGLAGS += -fno-inline-small-functions

#linker flags
	LDFLAGS += -Wl,-Map=$(TRG).map,--cref -Wl,-u,vfprintf -lprintf_min
#	LDFLAGS = -Wl,-Map=$(TRG).map,--cref -lm
# 	KEYMAP_ADDRESS = 0x6500
# 	LDFLAGS += -Wl,--section-start=.key_matrix_basic=$(KEYMAP_ADDRESS)

	
########### you should not need to change the following line #############
include $(COMMON_DIR)/avrproj.mk
	  
###### dependecies, add any dependencies you need here ###################
#  Dependencies tell the compiler which files in your code depend on which
#  other files.  When you change a piece of code, the dependencies allow
#  the compiler to intelligently figure out which files are affected and
#  need to be recompiled.  You should only list the dependencies of *.o 
#  files.  For example: uart.o is the compiled output of uart.c and uart.h
#  and therefore, uart.o "depends" on uart.c and uart.h.  But the code in
#  uart.c also uses information from global.h, so that file should be listed
#  in the dependecies too.  That way, if you alter global.h, uart.o will be
#  recompiled to take into account the changes.

buffer.o		: buffer.c		buffer.h
uart.o		: uart.c			uart.h		global.h
uart2.o		: uart2.c		uart2.h		global.h
rprintf.o	: rprintf.c		rprintf.h
a2d.o			: a2d.c			a2d.h
timer.o		: timer.c		timer.h		global.h
pulse.o		: pulse.c		pulse.h		timer.h	global.h
lcd.o			: lcd.c			lcd.h			global.h
i2c.o			: i2c.c			i2c.h			global.h
spi.o			: spi.c			spi.h			global.h
swpwm.o		: swpwm.c		swpwm.h		global.h
servo.o		: servo.c		servo.h		global.h
swuart.o		: swuart.c		swuart.h		global.h
tsip.o		: tsip.c			tsip.h		global.h
nmea.o		: nmea.c			nmea.h		global.h
vt100.o		: vt100.c		vt100.h		global.h
gps.o			: gps.c			gps.h			global.h
$(TRG).o		: $(TRG).c						global.h
keysta.o: keysta.h keysta.c

prog: all
	avrdude -c stk500v2 -P com3 -p atmega32 -U hfuse:w:0xD0:m -U lfuse:w:0x0F:m
	avrdude -c stk500v2 -P com3 -p atmega32 -U flash:w:main.hex:i
	pause;

#rename: $(TRG).hex
#	cp $(TRG).hex $(RESULT).hex

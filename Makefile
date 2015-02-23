# If the GCC ARM tools are already on the path, use them. Otherwise, use 
# the local version in gcc-arm/bin
GCC_ARM_VERSION := $(shell arm-none-eabi-gcc --version 2>/dev/null)
ifdef GCC_ARM_VERSION
	GCCDIR=
else
	GCCDIR=gcc-arm/bin/
endif

CC = $(GCCDIR)arm-none-eabi-gcc
AR = $(GCCDIR)arm-none-eabi-ar
OBJCOPY = $(GCCDIR)arm-none-eabi-objcopy
OBJDUMP = $(GCCDIR)arm-none-eabi-objdump

DEBUG_OPTS = 	\
				-g3					\
				-gdwarf-2			\
				-gstrict-dwarf
#OPTS = -Os 
OPTS = 			\
				-Os									\
				--specs=nano.specs					\
				-fplugin=tree_switch_shortcut_elf
###--enable-newlib-nano-malloc
DEFS = 
#-Os
#	Optimize for size. -Os enables all -O2 optimizations that do not 
#   typically increase code size. It also performs further optimizations
# designed to reduce code size.
#   -Os disables the following optimization flags:
#       -falign-functions  -falign-jumps  -falign-loops 
#       -falign-labels  -freorder-blocks  -freorder-blocks-and-partition 
#       -fprefetch-loop-arrays
#--specs=nano.specs
#	Use newlib-nano
#-fplugin=tre_switch_shortcut_elf
#	Optimization for FSM
TARGET = -mcpu=cortex-m0
CFLAGS = 	\
			$(DEFS)				\
			-ffunction-sections	\
			-fdata-sections		\
			-Wall				\
			-fmessage-length=0	\
			$(TARGET)			\
			-mthumb				\
			-mfloat-abi=soft	\
			$(DEBUG_OPTS)		\
			$(OPTS)				\
			-I .

LIBOBJS = 	\
			_startup.o		\
			accel.o			\
			app_menu.o		\
			debug.o			\
			delay.o			\
			driver_ADC.o	\
			ring.o			\
			syscalls.o		\
			tests.o			\
			touch.o			\
			uart.o			\
			usb.o 

INCLUDES = \
			app_menu.h		\
			common.h		\
			driver_ADC.h	\
#			driver_LPTMR.h	\
			driver_SYSTICK.h\
			freedom.h		\
			pt.h			\
			pt-sem.h		\
			types.h

.PHONY:	clean gcc-arm deploy

# -----------------------------------------------------------------------------

all: main.srec main.dump

libbare.a: $(LIBOBJS)
	$(AR) -rv libbare.a $(LIBOBJS)

clean:
	rm -f *.o *.lst *.out libbare.a *.srec *.dump

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.dump: %.out
	$(OBJDUMP) --disassemble $< >$@

%.srec: %.out
	$(OBJCOPY) -O srec $< $@

%.out: %.o OpenKL25Z.ld libbare.a
	$(CC) $(CFLAGS) -T OpenKL25Z.ld -o $@ $< libbare.a

# -----------------------------------------------------------------------------
# Burn/deploy by copying to the development board filesystem
#  Hack:  we identify the board by the filesystem size (128mb)
DEPLOY_VOLUME = $(shell df -h 2>/dev/null | fgrep " 128M" | awk '{print $$6}')
deploy: main.srec
	dd conv=fsync bs=64k if=$< of=$(DEPLOY_VOLUME)/$<
	
# -----------------------------------------------------------------------------
# Download and unpack the GCC ARM embedded toolchain (binaries)

DLPATH=https://launchpad.net/gcc-arm-embedded/4.8/4.8-2014-q1-update/+download/gcc-arm-none-eabi-4_8-2014q1-20140314

ifeq ($(shell uname -s), Darwin)
	DL_CMD=curl --location $(DLPATH)-mac.tar.bz2
else
	DL_CMD=wget --verbose $(DLPATH)-linux.tar.bz2 -O -
endif

gcc-arm:
	$(DL_CMD) | tar jx
	ln -s `ls -Artd gcc-arm-none-eabi* | tail -n 1` gcc-arm

burn:
	make clean
	make
	make deploy

# MCU name and submodel
MCU      = cortex-m3
ARCH	 = armv7-m
SUBMDL   = stm32f103

TARGET = librtos.a
SRC = rtos.o os_delay.o os_queue.o systick.o

# toolchain (using code sourcery now)
THUMB    = -mthumb
THUMB_IW = -mthumb-interwork

#--------------compile tools-------------------
CROSS_COMPILE ?= arm-none-eabi-
CC       := $(CROSS_COMPILE)gcc
AR		 := $(CROSS_COMPILE)ar


INC = -I./../inc

CFLAGS = -mcpu=$(MCU) -march=$(ARCH) $(THUMB) $(THUMB_IW) -O2 -g3 -gdwarf-2 -nostdlib -ffunction-sections -fdata-sections -Wl,--gc-sections -fno-builtin $(INC)
ARFLAGS = -rv


$(TARGET):$(SRC)
	$(AR) $(ARFLAGS) $(TARGET) $(SRC)
	mv $(TARGET) ../


%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f *.o

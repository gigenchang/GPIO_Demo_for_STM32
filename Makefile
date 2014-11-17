PROJECT = main

EXECUTABLE = $(PROJECT).elf
BIN_IMAGE = $(PROJECT).bin
HEX_IMAGE = $(PROJECT).hex
MAP_FILE = $(PROJECT).map
LIST_FILE = $(PROJECT).lst

# Toolchain config
CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE = $(CROSS_COMPILE)size

# Cortex-M4 implements the ARMv7E-M architecture
CPU = cortex-m4
CFLAGS = -mcpu=$(CPU) -march=armv7e-m -mtune=cortex-m4
CFLAGS += -mlittle-endian -mthumb
# Need study
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=softfp

define get_library_path
    $(shell dirname $(shell $(CC) $(CFLAGS) -print-file-name=$(1)))
endef

# Basic configurations
CFLAGS += -g -std=c99
CFLAGS += -Wall -Werror

# Optimizations
CFLAGS += -O0 -ffast-math
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections
CFLAGS += -fno-common
CFLAGS += --param max-inline-insns-single=1000

# specify STM32F429
CFLAGS += -DSTM32F429_439xx

# to run from FLASH
LDFLAGS += -T $(PWD)/stm32f429zi_flash.ld
LDFLAGS += -L $(call get_library_path,libc.a)
LDFLAGS += -L $(call get_library_path,libgcc.a)
LDFLAGS += -O3


# STM32F4xx_StdPeriph_Driver
CFLAGS += -DUSE_STDPERIPH_DRIVER


STM32_LIB = ../STM32F429I-Discovery_FW_V1.0.1/Libraries/STM32F4xx_StdPeriph_Driver

# Include Library and header file
INCDIR = $(PWD) \
		 $(PWD)/inc \
		 ../STM32F429I-Discovery_FW_V1.0.1/Libraries/CMSIS/Device/ST/STM32F4xx/Include \
		 ../STM32F429I-Discovery_FW_V1.0.1/Libraries/CMSIS/Include \
		 $(STM32_LIB)/inc \

INCLUDES = $(addprefix -I, $(INCDIR))

# source file path
SRCDIR = $(PWD)/src


SRC = startup_stm32f429_439xx.s \
	  system_stm32f4xx.c \
	  $(STM32_LIB)/src/misc.c \
	  $(STM32_LIB)/src/stm32f4xx_gpio.c \
	  $(STM32_LIB)/src/stm32f4xx_rcc.c \
	  $(STM32_LIB)/src/stm32f4xx_exti.c \
	  $(STM32_LIB)/src/stm32f4xx_syscfg.c \
#	  stm32f4xx_it.c \

SRC += $(wildcard $(addsuffix /*.c, $(SRCDIR))) \
	   $(wildcard $(addsuffix /*.s, $(SRCDIR))) 

# replace src(*.c, *.s) into objs(*.o)
OBJS := $(patsubst %.c, %.o, $(SRC))
OBJS := $(patsubst %.s, %.o, $(OBJS))

all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@
	$(OBJCOPY) -O ihex $^ $(HEX_IMAGE)
	$(OBJDUMP) -h -S -D $^ > $(LIST_FILE)
	$(SIZE) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJS)
	$(LD) -o $@ $^ -Map=$(MAP_FILE) $(LDFLAGS)

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

flash:
	st-flash write $(BIN_IMAGE) 0x8000000

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
	rm -rf $(HEX_IMAGE)
	rm -f $(OBJS)
	rm -f $(MAP_FILE)
	rm -f $(PROJECT).lst

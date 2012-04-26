PROJECT = knobbox
OBJS += main.o syscalls.o TDisplay.o

# -------------------------------------
# Assuming a toolchain from summon-arm-toolchain.
# This toolchain includes gcc, newlib (a c library) and
# libopencm3 (ARM Cortex-M3 library).

TOOLCHAIN = $(HOME)/sat
PREFIX = $(TOOLCHAIN)/bin/arm-none-eabi
CC = $(PREFIX)-gcc
LD = $(PREFIX)-gcc
OBJCOPY = $(PREFIX)-objcopy
OBJDUMP = $(PREFIX)-objdump

LDFLAGS += -lopencm3_stm32f1
LDFLAGS += -lc -lnosys -nostartfiles -Wl,--gc-sections -lstdc++
LDFLAGS += -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float
LDFLAGS += -Tstm32vl-discovery.ld

COMMONFLAGS += -Os -fno-common -g
COMMONFLAGS += -mcpu=cortex-m3 -mthumb -msoft-float -DSTM32F1
COMMONFLAGS += -Wall -Wextra
COMMONFLAGS += -I$(TOOLCHAIN)/arm-none-eabi/include/libopencm3/stm32/f1

CFLAGS += -std=c99 -Werror-implicit-function-declaration

CXXFLAGS += -std=c++0x -fno-exceptions -fno-rtti

# -------------------------------------


all: $(PROJECT).bin $(PROJECT).asm
	@size $(PROJECT).elf

%.o: %.c Makefile
	@echo CC $< --\> $@
	@$(CC) -MD $(COMMONFLAGS) $(CFLAGS) -c -o $@ $<

%.o: %.cpp Makefile
	@echo CC $< --\> $@
	@$(CC) -MD $(COMMONFLAGS) $(CXXFLAGS) -c -o $@ $<

$(PROJECT).elf: $(OBJS)
	@echo LD $@
	@$(LD) -o $@ $(OBJS) $(LDFLAGS) -Wl,-Map,$(PROJECT).map

%.bin: %.elf
	@echo flattening $@
	@$(OBJCOPY) -Obinary $< $@

%.asm: %.elf
	@echo disassembling $@
	@$(OBJDUMP) -d -S $< > $@

-include *.d

clean:
	rm -f $(PROJECT).elf $(PROJECT).bin $(PROJECT).asm $(PROJECT).map
	rm -f $(OBJS)
	rm -f *.d

.PHONY: clean all

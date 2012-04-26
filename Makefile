PROJECT = knobbox
OBJS += main.o
OBJS += syscalls.o
OBJS += TDisplay.o

# -------------------------------------
# Assuming a toolchain from summon-arm-toolchain.
# This toolchain includes gcc, newlib (a c library) and
# libopencm3 (ARM Cortex-M3 library).

BUILDDIR=build
HOSTBUILDDIR=hostbuild

TOOLCHAIN = $(HOME)/sat
PREFIX = $(TOOLCHAIN)/bin/arm-none-eabi
ARMCC = $(PREFIX)-gcc
ARMLD = $(PREFIX)-gcc
ARMOBJCOPY = $(PREFIX)-objcopy
ARMOBJDUMP = $(PREFIX)-objdump

ARM_LDFLAGS += -lopencm3_stm32f1
ARM_LDFLAGS += -lc -lnosys -nostartfiles -Wl,--gc-sections -lstdc++
ARM_LDFLAGS += -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float
ARM_LDFLAGS += -Tstm32vl-discovery.ld
HOST_LDFLAGS +=

ARM_COMMONFLAGS += -Os -fno-common -g
ARM_COMMONFLAGS += -mcpu=cortex-m3 -mthumb -msoft-float -DSTM32F1
ARM_COMMONFLAGS += -Wall -Wextra
ARM_COMMONFLAGS += -I$(TOOLCHAIN)/arm-none-eabi/include/libopencm3/stm32/f1
HOST_COMMONFLAGS += -O0 -fno-common -g -Wall -Wextra -DHOST

CFLAGS += -std=c99 -Werror-implicit-function-declaration

CXXFLAGS += -std=c++0x -fno-exceptions -fno-rtti

ARM_OBJS = $(OBJS:%=$(BUILDDIR)/%)
HOST_OBJS = $(OBJS:%=$(HOSTBUILDDIR)/%)

# -------------------------------------

all: $(HOSTBUILDDIR) $(HOSTBUILDDIR)/$(PROJECT).elf
all: $(BUILDDIR) $(BUILDDIR)/$(PROJECT).bin $(BUILDDIR)/$(PROJECT).asm
	@size $(BUILDDIR)/$(PROJECT).elf

$(BUILDDIR) $(HOSTBUILDDIR):
	mkdir $@

liquid-2.0: external/liquid-2.0.tgz external/liquid.patch Makefile
	tar xf $<
	@patch -d $@ < external/liquid.patch

liquid-2.0/fontliqsting.inc: liquid-2.0

# -------------------------------------
# ARM rules

$(BUILDDIR)/%.o: %.c Makefile
	@echo ARMCC $< --\> $@
	@$(ARMCC) -MD $(ARM_COMMONFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: %.cpp Makefile liquid-2.0/fontliqsting.inc
	@echo ARMCC $< --\> $@
	@$(ARMCC) -MD $(ARM_COMMONFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/$(PROJECT).elf: $(ARM_OBJS)
	@echo ARMLD $@
	@$(ARMLD) -o $@ $(ARM_OBJS) $(ARM_LDFLAGS) -Wl,-Map,$(BUILDDIR)/$(PROJECT).map

%.bin: %.elf
	@echo flattening $@
	@$(ARMOBJCOPY) -Obinary $< $@

%.asm: %.elf
	@echo disassembling $@
	@$(ARMOBJDUMP) -d -S $< > $@

# -------------------------------------
# Host rules

$(HOSTBUILDDIR)/%.o: %.c Makefile
	@echo CC $< --\> $@
	@$(CC) -MD $(HOST_COMMONFLAGS) $(CFLAGS) -c -o $@ $<

$(HOSTBUILDDIR)/%.o: %.cpp Makefile liquid-2.0/fontliqsting.inc
	@echo CC $< --\> $@
	@$(CC) -MD $(HOST_COMMONFLAGS) $(CXXFLAGS) -c -o $@ $<

$(HOSTBUILDDIR)/$(PROJECT).elf: $(HOST_OBJS)
	@echo LD $@
	$(CXX) -o $@ $(HOST_OBJS) $(HOST_LDFLAGS)

# -------------------------------------

-include $(BUILDDIR)/*.d
-include $(HOSTBUILDDIR)/*.d

clean:
	rm -f $(BUILDDIR)

.PHONY: clean all

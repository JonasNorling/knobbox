PROJECT = knobbox
SRCS += TControllerPage.cpp
SRCS += TControllers.cpp
SRCS += TDisplay.cpp
SRCS += TGui.cpp
SRCS += TKnobs.cpp
SRCS += TMemory.cpp
SRCS += TSequencer.cpp
SRCS += TSeqPage.cpp
SRCS += TSpiDmaJob.cpp
ARM_SRCS += main.cpp
ARM_SRCS += stm32.cpp
HOST_SRCS += main.cpp
HOST_SRCS += host.cpp
TEST_SRCS += tests/testmain.cpp
TEST_SRCS += host.cpp

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

ARM_COMMONFLAGS += -I. -Os -fno-common -g
ARM_COMMONFLAGS += -mcpu=cortex-m3 -mthumb -msoft-float -DSTM32F1
ARM_COMMONFLAGS += -Wall -Wextra
ARM_COMMONFLAGS += -I$(TOOLCHAIN)/arm-none-eabi/include/libopencm3/stm32
HOST_COMMONFLAGS += -I. -O0 -fno-common -g -Wall -Wextra -DHOST

CFLAGS += -std=c99 -Werror-implicit-function-declaration

CXXFLAGS += -std=c++0x -fno-exceptions -fno-rtti

ARM_OBJS += $(SRCS:%.cpp=$(BUILDDIR)/%.o)
ARM_OBJS += $(ARM_SRCS:%.cpp=$(BUILDDIR)/%.o)
HOST_OBJS += $(SRCS:%.cpp=$(HOSTBUILDDIR)/%.o)
HOST_OBJS += $(HOST_SRCS:%.cpp=$(HOSTBUILDDIR)/%.o)
TEST_OBJS += $(SRCS:%.cpp=$(HOSTBUILDDIR)/%.o)
TEST_OBJS += $(TEST_SRCS:%.cpp=$(HOSTBUILDDIR)/%.o)

# -------------------------------------

all: $(HOSTBUILDDIR) $(HOSTBUILDDIR)/$(PROJECT).elf
all: $(BUILDDIR) $(BUILDDIR)/$(PROJECT).bin $(BUILDDIR)/$(PROJECT).asm
	@size $(BUILDDIR)/$(PROJECT).elf
	@$(ARMOBJDUMP) -t -j .bss -C $(BUILDDIR)/$(PROJECT).elf | \
		awk '{n = strtonum("0x"$$5); if (n!=0) printf("%20s %5d bytes\n", $$6, n);}'

$(BUILDDIR) $(HOSTBUILDDIR):
	mkdir $@ $@/tests

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

$(HOSTBUILDDIR)/%.o: tests/%.cpp Makefile
	@echo CC $< --\> $@
	@$(CC) -MD $(HOST_COMMONFLAGS) $(CXXFLAGS) -c -o $@ $<

$(HOSTBUILDDIR)/$(PROJECT).elf: $(HOST_OBJS)
	@echo LD $@
	@$(CXX) -o $@ $(HOST_OBJS) $(HOST_LDFLAGS)

$(TEST_OBJS): tests/*.cpp

$(HOSTBUILDDIR)/test: $(TEST_OBJS)
	@echo LD $@
	$(CXX) -o $@ $(TEST_OBJS) $(HOST_LDFLAGS)

tests: $(HOSTBUILDDIR)/test
	exec $(HOSTBUILDDIR)/test

# -------------------------------------

doc: $(SRCS) $(ARM_SRCS) $(HOST_SRCS) doc/Doxyfile
	doxygen doc/Doxyfile

# -------------------------------------

-include $(BUILDDIR)/*.d
-include $(HOSTBUILDDIR)/*.d

clean:
	rm -rf $(BUILDDIR) $(HOSTBUILDDIR)

.PHONY: clean all tests doc

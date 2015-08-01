PROJECT = knobbox

SRCS += src/TControllerPage.cpp
SRCS += src/TControllers.cpp
SRCS += src/TFlashTest.cpp
SRCS += src/TFlash.cpp
SRCS += src/TGui.cpp
SRCS += src/TKnobs.cpp
SRCS += src/TLeds.cpp
SRCS += src/TMemory.cpp
SRCS += src/TMidi.cpp
SRCS += src/TMidiParser.cpp
SRCS += src/TPopup.cpp
SRCS += src/TSequencer.cpp
SRCS += src/TSeqPage.cpp
SRCS += src/TSettingsPage.cpp
SRCS += src/TStartAnimation.cpp
SRCS += src/TSwitches.cpp

ARM_C_SRCS += src/target/usbmidi.c
ARM_SRCS += src/main.cpp
ARM_SRCS += src/target/stm32.cpp
ARM_SRCS += src/target/TDisplay.cpp
ARM_SRCS += src/target/TSchedulerArm.cpp
ARM_SRCS += src/target/TSpiDmaJob.cpp
ARM_SRCS += src/target/TUsb.cpp

HOST_SRCS += src/main.cpp
HOST_SRCS += src/host/host.cpp
HOST_SRCS += src/host/TDisplay.cpp
HOST_SRCS += src/host/TSchedulerHost.cpp
HOST_SRCS += src/host/TSpiDmaJob.cpp
HOST_SRCS += src/host/WxLcdDisplay.cpp

TEST_SRCS += src/tests/testmain.cpp
TEST_SRCS += src/host/TDisplay.cpp
TEST_SRCS += src/host/TSchedulerHost.cpp
TEST_SRCS += src/host/TSpiDmaJob.cpp
TEST_SRCS += src/host/host.cpp

# -------------------------------------
# Assuming a toolchain from summon-arm-toolchain.
# This toolchain includes gcc, newlib (a c library) and
# libopencm3 (ARM Cortex-M3 library).

BUILDDIR=build
HOSTBUILDDIR=hostbuild

PREFIX = arm-none-eabi
ARMCC = $(PREFIX)-gcc
ARMLD = $(PREFIX)-gcc
ARMOBJCOPY = $(PREFIX)-objcopy
ARMOBJDUMP = $(PREFIX)-objdump
LIBOPENCM3 = libopencm3

ARM_LDFLAGS += -L$(LIBOPENCM3)/lib/ -lopencm3_stm32f1
ARM_LDFLAGS += -lc -lnosys -nostartfiles -Wl,--gc-sections -lstdc++
ARM_LDFLAGS += -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float
ARM_LDFLAGS += -Tstm32f1.ld
HOST_LDFLAGS += -pthread
HOST_LDFLAGS += `wx-config --libs`

ARM_COMMONFLAGS += -I. -Isrc -Isrc/target -O1 -fno-common -g -fconserve-stack
ARM_COMMONFLAGS += -mcpu=cortex-m3 -mthumb -msoft-float -DSTM32F1
ARM_COMMONFLAGS += -Wall -Wextra
ARM_COMMONFLAGS += -I$(LIBOPENCM3)/include/
ARM_COMMONFLAGS +=  -fno-rtti
HOST_COMMONFLAGS += -I. -Isrc -Isrc/host -O0 -fno-common -g -Wall -Wextra -DHOST -pthread
HOST_COMMONFLAGS += `wx-config --cflags`

CFLAGS += -std=c99 -Werror-implicit-function-declaration

CXXFLAGS += -std=c++11 -fno-exceptions

ARM_OBJS += $(SRCS:%.cpp=$(BUILDDIR)/%.o)
ARM_OBJS += $(ARM_SRCS:%.cpp=$(BUILDDIR)/%.o)
ARM_OBJS += $(ARM_C_SRCS:%.c=$(BUILDDIR)/%.o)
HOST_OBJS += $(SRCS:%.cpp=$(HOSTBUILDDIR)/%.o)
HOST_OBJS += $(HOST_SRCS:%.cpp=$(HOSTBUILDDIR)/%.o)
TEST_OBJS += $(SRCS:%.cpp=$(HOSTBUILDDIR)/%.o)
TEST_OBJS += $(TEST_SRCS:%.cpp=$(HOSTBUILDDIR)/%.o)

# -------------------------------------

all: $(HOSTBUILDDIR)/$(PROJECT).elf
all: $(BUILDDIR)/flashtest.elf
all: $(BUILDDIR)/tasktest.elf
all: $(BUILDDIR)/knobtest.elf
all: $(BUILDDIR)/$(PROJECT).bin $(BUILDDIR)/$(PROJECT).asm
	@arm-none-eabi-size $(BUILDDIR)/$(PROJECT).elf
#	@$(ARMOBJDUMP) -t -j .bss -C $(BUILDDIR)/$(PROJECT).elf | \
		awk '{n = strtonum("0x"$$5); if (n!=0) printf("%20s %5d bytes\n", $$6, n);}'

liquid-2.0: external/liquid-2.0.tgz external/liquid.patch Makefile
	@echo
	@echo "##### Unpacking liquid font"
	@echo
	tar xf $<
	@patch -d $@ < external/liquid.patch

liquid-2.0/fontliqsting.inc: liquid-2.0

# libopencm3 from 2015-07-30
LIBOPENCM3_V=a28ba5c

$(LIBOPENCM3): external/libopencm3-$(LIBOPENCM3_V).tgz
	@echo
	@echo "##### Unpacking libopencm3"
	@echo
	rm -rf $@ libopencm3-$(LIBOPENCM3_V)
	tar xf $<
	mv libopencm3-$(LIBOPENCM3_V) $@
	@touch $@

$(LIBOPENCM3)/lib/libopencm3_stm32f1.a $(LIBOPENCM3)/include: $(LIBOPENCM3)
	@echo
	@echo "##### Building libopencm3"
	@echo
	# Disable tail call optimization, for easier USB code debugging
	cd $(LIBOPENCM3) && $(MAKE) TARGETS=stm32/f1 DEBUG_FLAGS="-fno-optimize-sibling-calls -g"
	touch $(LIBOPENCM3)/lib/libopencm3_stm32f1.a $(LIBOPENCM3)/include

# -------------------------------------
# ARM rules

$(BUILDDIR)/%.o: %.c Makefile $(LIBOPENCM3)/include
	@echo ARMCC $< --\> $@
	@mkdir -p $(dir $@)
	@$(ARMCC) -MD $(ARM_COMMONFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: %.cpp Makefile liquid-2.0/fontliqsting.inc $(LIBOPENCM3)/include
	@echo ARMCC $< --\> $@
	@mkdir -p $(dir $@)
	@$(ARMCC) -MD $(ARM_COMMONFLAGS) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/$(PROJECT).elf: $(ARM_OBJS) $(LIBOPENCM3)/lib/libopencm3_stm32f1.a
	@echo ARMLD $@
	@$(ARMLD) -o $@ $(ARM_OBJS) $(ARM_LDFLAGS) -Wl,-Map,$(BUILDDIR)/$(PROJECT).map

%.bin: %.elf
	@echo flattening $@
	@$(ARMOBJCOPY) -Obinary $< $@

%.asm: %.elf
	@echo disassembling $@
	@$(ARMOBJDUMP) -d -S $< > $@

program: $(BUILDDIR) $(BUILDDIR)/$(PROJECT).bin
	st-flash write $(BUILDDIR)/$(PROJECT).bin 0x08000000

# -------------------------------------
# ARM test program rules

$(BUILDDIR)/flashtest.elf: $(BUILDDIR)/test/flashtest/flashtest.o
	@echo ARMLD $@
	$(ARMLD) -o $@ $^ $(ARM_LDFLAGS)

$(BUILDDIR)/knobtest.elf: $(BUILDDIR)/test/knobtest/knobtest.o $(BUILDDIR)/src/target/stm32.o
	@echo ARMLD $@
	$(ARMLD) -o $@ $^ $(ARM_LDFLAGS)

$(BUILDDIR)/tasktest.elf: $(BUILDDIR)/test/tasktest/tasktest.o $(BUILDDIR)/src/target/TSchedulerArm.o
	@echo ARMLD $@
	$(ARMLD) -o $@ $^ $(ARM_LDFLAGS)

# -------------------------------------
# Host rules

$(HOSTBUILDDIR)/%.o: %.c Makefile
	@echo CC $< --\> $@
	@mkdir -p $(dir $@)
	@$(CC) -MD $(HOST_COMMONFLAGS) $(CFLAGS) -c -o $@ $<

$(HOSTBUILDDIR)/%.o: %.cpp Makefile liquid-2.0/fontliqsting.inc
	@echo CC $< --\> $@
	@mkdir -p $(dir $@)
	@$(CC) -MD $(HOST_COMMONFLAGS) $(CXXFLAGS) -c -o $@ $<

$(HOSTBUILDDIR)/%.o: tests/%.cpp Makefile
	@echo CC $< --\> $@
	@mkdir -p $(dir $@)
	@$(CC) -MD $(HOST_COMMONFLAGS) $(CXXFLAGS) -c -o $@ $<

$(HOSTBUILDDIR)/$(PROJECT).elf: $(HOST_OBJS)
	@echo LD $@
	@$(CXX) -o $@ $(HOST_OBJS) $(HOST_LDFLAGS)

$(TEST_OBJS): src/tests/*.cpp

$(HOSTBUILDDIR)/test: $(TEST_OBJS)
	@echo LD $@
	$(CXX) -o $@ $(TEST_OBJS) $(HOST_LDFLAGS)

tests: $(HOSTBUILDDIR)/test
	exec $(HOSTBUILDDIR)/test

# -------------------------------------

doc: $(SRCS) $(ARM_SRCS) $(HOST_SRCS) doc/Doxyfile
	doxygen doc/Doxyfile

# -------------------------------------

-include $(BUILDDIR)/src/*.d
-include $(HOSTBUILDDIR)/src/*.d

clean:
	rm -rf $(BUILDDIR) $(HOSTBUILDDIR) libopencm3-$(LIBOPENCM3_V)
	rm -rf liquid-2.0 libopencm3

.PHONY: clean all tests doc program

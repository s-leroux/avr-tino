PROGRAM_PREFIX=avr-
PROGRAM_SUFFIX=

# C preprocessor
CPP=$(PROGRAM_PREFIX)cpp$(PROGRAM_SUFFIX)
CPPFLAGS=-I./include -DBOARD_$(BOARD)=1

# C & C++
C_CXX_COMMON_FLAGS= -mmcu=$(MCU) -Os \
	-ffunction-sections -fdata-sections

CC=$(PROGRAM_PREFIX)gcc$(PROGRAM_SUFFIX)
CCFLAGS=-std=c99 $(C_CXX_COMMON_FLAGS)

CXX=$(PROGRAM_PREFIX)g++$(PROGRAM_SUFFIX)
CXXFLAGS= $(C_CXX_COMMON_FLAGS)

# Linker
#
# I use 'gcc' here since it knowns which specific library to add
# according to the target mcu
#
LD=$(PROGRAM_PREFIX)gcc$(PROGRAM_SUFFIX)
LDFLAGS=-Wl,--gc-sections -Wl,--print-gc-sections -mmcu=$(MCU)

# Object copy
OBJCOPY=$(PROGRAM_PREFIX)objcopy$(PROGRAM_SUFFIX)

#
# Target
#
MCU=attiny2313
BOARD=factory

DEMOSRCDIR=./demo/
SRCDIR=./src/avr-tino/

BUILDDIR=build-$(MCU)-$(BOARD)/
OBJDIR=$(BUILDDIR)obj/
BINDIR=$(BUILDDIR)bin/
DEPDIR=$(BUILDDIR)deps/
DEMOBINDIR=$(BUILDDIR)bin/demo/
DIRS=$(BUILDDIR) $(OBJDIR) $(BINDIR) $(DEPDIR) $(DEMOBINDIR)

DEMOS=$(DEMOBINDIR)input $(DEMOBINDIR)blink $(DEMOBINDIR)shiftout
OBJS=$(OBJDIR)pin.o

all:	objs demo hex


$(DIRS): 
	mkdir $@

demo:	$(DIRS) $(DEMOS)

hex:	$(DIRS) $(DEMOS:=.hex)

objs:	$(DIRS) $(OBJS)

clean:
	rm -rf $(DIRS) build-*

$(OBJDIR)%.o : $(SRCDIR)%.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -MMD -MP -MF $(DEPDIR)$*.deps -o $@

$(OBJDIR)%.o : $(SRCDIR)%.c
	$(CC) -c $(CPPFLAGS) $(CCFLAGS) $< -MMD -MP -MF $(DEPDIR)$*.deps -o $@

$(OBJDIR)%.o : $(DEMOSRCDIR)%.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -MMD -MP -MF $(DEPDIR)$*.deps -o $@

$(OBJDIR)%.o : $(DEMOSRCDIR)%.c
	$(CC) -c $(CPPFLAGS) $(CCFLAGS) $< -MMD -MP -MF $(DEPDIR)$*.deps -o $@

$(DEMOBINDIR)% : $(OBJDIR)%.o $(OBJS)
	$(LD) $(LDFLAGS) -s -o $@ $^


$(DEMOBINDIR)%.hex : $(DEMOBINDIR)%
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

-include $(DEPDIR)*.deps


#
# Target
#
MCU=attiny2313
BOARD=factory

PROGRAM_PREFIX=avr-
PROGRAM_SUFFIX=

# Tools
OBJCOPY=$(PROGRAM_PREFIX)objcopy$(PROGRAM_SUFFIX)
GCC=$(PROGRAM_PREFIX)gcc$(PROGRAM_SUFFIX)

#
# For better optimization, this makefile
# will combine all source files in one that will
# be compiled and linked by a single command-line
# call to avr-gcc
#
CPPFLAGS=-I./include -DBOARD_$(BOARD)=1
CXXFLAGS=-mmcu=$(MCU) \
	-Os -mcall-prologues \
	-ffunction-sections -fdata-sections
LDFLAGS=-Wl,--gc-sections -Wl,--print-gc-sections -Wl,--relax


DEMOSRCDIR=./demo/
SRCDIR=./src/avr-tino/

BUILDDIR=build-$(MCU)-$(BOARD)/
OBJDIR=$(BUILDDIR)obj/
BINDIR=$(BUILDDIR)bin/
DEPDIR=$(BUILDDIR)deps/
DEMOBINDIR=$(BUILDDIR)bin/demo/
DIRS=$(BUILDDIR) $(OBJDIR) $(BINDIR) $(DEPDIR) $(DEMOBINDIR)

DEMOS=	$(DEMOBINDIR)input  \
	$(DEMOBINDIR)blink \
	$(DEMOBINDIR)shiftout \
	$(DEMOBINDIR)spiout

SRCFILES=$(SRCDIR)pin.cc $(SRCDIR)SPI.cc

all:	demo hex


$(DIRS): 
	mkdir $@

demo:	$(DIRS) $(DEMOS)

hex:	$(DIRS) $(DEMOS:=.hex)

clean:
	rm -rf $(DIRS) build-*

#
# Combine all source file in one for better optimization
#
.PRECIOUS: $(OBJDIR)%.combined.cc
$(OBJDIR)%.combined.cc : $(DEMOSRCDIR)%.cc $(SRCFILES)
	for i in $^; do \
	    echo '#line 1 "'$$i'"' ; \
	    cat $$i; \
	done > $@

#
# Compile and link the combined file to produce executable
#
$(DEMOBINDIR)% : $(OBJDIR)%.combined.cc
	$(GCC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $< -fwhole-program \
               -MMD -MP -MF $(DEPDIR)$*.deps 

#
# Intel Hex file to upload on target
#
$(DEMOBINDIR)%.hex : $(DEMOBINDIR)%
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

-include $(DEPDIR)*.deps


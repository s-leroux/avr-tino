
#  Copyright (c) 2012 Sylvain Leroux <sylvain@chicoree.fr>
#  
#  This file is part of avr-tino -- http://github.com/s-leroux/avr-tino
#  
#  avr-tino is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  avr-tino is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with avr-tino.  If not, see <http://www.gnu.org/licenses/>.

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
SIZE=$(PROGRAM_PREFIX)size$(PROGRAM_SUFFIX)


#
# For better optimization, this makefile
# will combine all source files in one that will
# be compiled and linked by a single command-line
# call to avr-gcc
#
CPPFLAGS=-I./include -include "avr-tino/target/$(BOARD).h"
CXXFLAGS=-mmcu=$(MCU) \
	-Os \
	-ffunction-sections -fdata-sections \
	-fno-rtti \
	-g
LDFLAGS=-Wl,--gc-sections -Wl,--print-gc-sections


DEMOSRCDIR=./demo/
SRCDIR=./src/avr-tino/

BUILDDIR=build-$(MCU)-$(BOARD)/
OBJDIR=$(BUILDDIR)obj/
BINDIR=$(BUILDDIR)bin/
DEPDIR=$(BUILDDIR)deps/
DIRS=$(BUILDDIR) $(OBJDIR) $(BINDIR) $(DEPDIR)

DEMOS=	$(BINDIR)input  \
	$(BINDIR)shiftout \
	$(BINDIR)spiout \
	$(BINDIR)usart \
	$(BINDIR)lcd \
	$(BINDIR)blink

SRCFILES=$(SRCDIR)pin.cc \
	$(SRCDIR)printer.cc \
	$(SRCDIR)SPI.cc \
	$(SRCDIR)serial.cc \
	$(SRCDIR)HD44780.cc \
	$(SRCDIR)MCP2515.cc 

ifeq ($(BOARD),CANModule)
DEMOS += build-$(MCU)-$(BOARD)/bin/canmodule
SRCFILES += $(SRCDIR)/target/CANModule.cc
endif

all:	demo hex

stat:	all
	$(SIZE) `find $(BINDIR) -executable -type f -print`

$(DIRS): 
	mkdir $@

demo:	$(DIRS) $(DEMOS)

hex:	$(DIRS) $(DEMOS:=.hex)

clean:
	rm -rf -- $(DIRS) build-*

git-clean: clean
	rm -f -- `git status -unormal --porcelain | awk '$$1=="??" { print $$2 }'`

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
$(BINDIR)% : $(OBJDIR)%.combined.cc
	$(GCC) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $< \
               -MMD -MP -MF $(DEPDIR)$*.deps 

#
# Intel Hex file to upload on target
#
$(BINDIR)%.hex : $(BINDIR)%
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

-include $(DEPDIR)*.deps


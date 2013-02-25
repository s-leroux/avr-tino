
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
MCU=tiny2313
BOARD=factory

# Load specific config
CONFIG_MK=./hardware/avr/$(MCU)/Makefile
-include $(CONFIG_MK)

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
CPPFLAGS=-I./$(BUILDDIR)include -I./include -include "avr-tino/target/$(BOARD).h"
ifdef F_CPU
    CPPFLAGS+= -DF_CPU=$(F_CPU)
endif

CXXFLAGS=-mmcu=$(GCC_MMCU) \
	-Os \
	-fipa-cp \
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
DIRS=$(BUILDDIR) $(BUILDDIR)include $(BUILDDIR)include/avr-tino  $(BUILDDIR)include/avr-tino/hardware \
     $(OBJDIR) $(BINDIR) $(DEPDIR)

DEMOS=	$(BINDIR)input  \
	$(BINDIR)port \
	$(BINDIR)eeprom \
	$(BINDIR)shiftout \
	$(BINDIR)spiout \
	$(BINDIR)1-wire \
	$(BINDIR)ds18s20 \
	$(BINDIR)usart \
	$(BINDIR)lcd \
	$(BINDIR)ioexpander \
	$(BINDIR)blink

SRCFILES=$(SRCDIR)pin.cc \
	$(SRCDIR)printer.cc \
	$(SRCDIR)1-wire.cc \
	$(SRCDIR)SPI.cc \
	$(SRCDIR)serial.cc \
	$(SRCDIR)HD44780.cc \
	$(SRCDIR)DS18S20.cc \
	$(SRCDIR)MCP23Sxx.cc \
	$(SRCDIR)MCP2515.cc 

ifeq ($(BOARD),CANModule)
DEMOS += build-$(MCU)-$(BOARD)/bin/canmodule-tx \
	build-$(MCU)-$(BOARD)/bin/canmodule-rx \
	build-$(MCU)-$(BOARD)/bin/canmodule-log \
	build-$(MCU)-$(BOARD)/bin/toucan-core \
	build-$(MCU)-$(BOARD)/bin/toucan-send
SRCFILES += $(SRCDIR)/target/CANModule.cc
endif

TARGET_DEF=$(BUILDDIR)include/avr-tino/hardware/mcu-def.h

all:	demo hex

stat:	all
	$(SIZE) `find $(BINDIR) -executable -type f -print`

$(DIRS): 
	mkdir $@

dirs:	$(DIRS)

init:	$(DIRS) $(TARGET_DEF)

demo:	$(DIRS) $(TARGET_DEF) $(DEMOS)

hex:	$(DIRS) $(DEMOS:=.hex)

clean:
	rm -rf -- $(DIRS) build-*

git-clean: clean
	rm -rf -- `git status -unormal --porcelain | awk '$$1=="??" { print $$2 }'`

help:
	@echo
	@echo "Supported hardware"
	@echo "------------------"
	@cd ./hardware/avr/ && for i in * ; do [ -f $$i/Makefile ] && echo -n 'M' || echo -n ' ' ; [ -f $$i/$$i.h ] && echo -n 'h' || echo -n ' ' ; echo "      "$$i; done 
	@echo
	@echo "Config"
	@echo "------"
	@echo "MCU=${MCU}"
	@echo "BOARD=${BOARD}"
	@echo "CONFIG_MK=${CONFIG_MK}"
	@echo "GCC_MMCU=${GCC_MMCU}"
	@echo "AVRDUDE_PART=${AVRDUDE_PART}"

.PHONY: all clean init dirs demo hex stat help git-clean

#
# Combine all source file in one for better optimization
#
.PRECIOUS: $(OBJDIR)%.combined.cc
$(OBJDIR)%.combined.cc : init $(DEMOSRCDIR)%.cc $(SRCFILES)
	for i in $^; do \
	    if [ -f $$i ] ; then \
		echo '#line 1 "'$$i'"' ; \
		cat $$i; \
	    fi; \
	done > $@


#
#
#
$(BUILDDIR)include/avr-tino/hardware/mcu-def.h : ./hardware/avr/$(MCU)/$(MCU).h
	( echo '#line 1 "$<"' ; cat $< ) > $@

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


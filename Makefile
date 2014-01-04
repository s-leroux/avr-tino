
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

AVRDUDE=avrdude
PROGRAMMER=usbasp

#
# Directories & main targets
#

# Adjust this to suit your needs
FIRMWARE=$(DEMOS)
BASEDIR=./

TINO_SRCDIR=./src/avr-tino/

SRCDIR=$(BASEDIR)/fw/
BUILDDIR=$(BASEDIR)/build-$(MCU)-$(BOARD)/
OBJDIR=$(BUILDDIR)obj/
BINDIR=$(BUILDDIR)bin/
DEPDIR=$(BUILDDIR)deps/
DIRS=$(BUILDDIR) $(BUILDDIR)include $(BUILDDIR)include/avr-tino  $(BUILDDIR)include/avr-tino/hardware \
     $(OBJDIR) $(BINDIR) $(DEPDIR)

DEMOS=	input  \
	port \
	eeprom \
	shiftout \
	spiout \
	1-wire \
	ds18s20 \
	usart \
	lcd \
	ioexpander \
	blink

SRCFILES=

ifeq ($(BOARD),CANModule)
DEMOS += canmodule-tx \
	canmodule-rx \
	canmodule-log \
	toucan-core \
	toucan-send
SRCFILES += $(TINO_SRCDIR)/target/CANModule.cc
endif

TARGET_DEF=$(BUILDDIR)include/avr-tino/hardware/mcu-def.h
BOARD_DEF=$(BUILDDIR)include/avr-tino/hardware/board-def.h

#
# Load specific config
#
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
CPPFLAGS=-I./$(BUILDDIR)include -I./include -I./$(BASEDIR)
ifdef F_CPU
    CPPFLAGS+= -DF_CPU=$(F_CPU)
endif
ifdef FW_VERSION
    CPPFLAGS+= -DFW_VERSION='"$(FW_VERSION)"'
endif

CXXFLAGS=-mmcu=$(GCC_MMCU) \
        -Wno-multichar \
	-Os \
	-fipa-cp \
	-ffunction-sections -fdata-sections \
	-fno-rtti \
	-g 
LDFLAGS=-Wl,--gc-sections -Wl,--print-gc-sections


all:	firmware hex

stat:	all
	$(SIZE) `find $(BINDIR) -executable -type f -print`

upload: firmware $(FIRMWARE:%=$(BINDIR)%.hex)
	if [ $(words $(FIRMWARE)) -gt 1 ] ; then \
	    echo "Could only upload 1 firmware" && exit 1 ; \
        fi
	for i in $(FIRMWARE:%=$(BINDIR)%.hex) ; do \
	    $(AVRDUDE) -p $(AVRDUDE_PART) -c $(PROGRAMMER) -U flash:w:$$i; \
	done

            
        

$(DIRS): 
	mkdir $@

dirs:	$(DIRS)

init:	$(DIRS) $(TARGET_DEF) $(BOARD_DEF)
.PHONY : init

firmware:	$(DIRS) $(TARGET_DEF) $(BOARD_DEF) $(FIRMWARE:%=$(BINDIR)%)

hex:	$(DIRS) $(FIRMWARE:%=$(BINDIR)%.hex)

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
	@echo
	@echo "Build"
	@echo "====="
	@echo "pwd=" `pwd`
	@echo "FIRMWARE=${FIRMWARE}"
	@echo "SRCDIR=${SRCDIR}"
	@echo "BINDIR=${BINDIR}"

.PHONY: all clean init dirs demo hex stat help git-clean

#
# Combine all source file in one for better optimization
#
.PRECIOUS: $(OBJDIR)%.combined.cc
$(OBJDIR)%.combined.cc : $(SRCDIR)/%.cc $(SRCFILES)
	for i in $(filter %.cc,$^); do \
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

$(BUILDDIR)include/avr-tino/hardware/board-def.h : ./hardware/boards/$(BOARD)/$(BOARD).h
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


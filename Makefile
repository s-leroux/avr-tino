CC=avr-gcc
CXX=avr-gcc

#
# Target
#
MCU=attiny2313
BOARD=factory

CPPFLAGS=-I./include -DBOARD_$(BOARD)=1
CCFLAGS:=$(CFLAGS) -mmcu=$(MCU) -O2
CXXFLAGS:=$(CXXFLAGS) -mmcu=$(MCU) -O2

DEMOSRC=./demo/

BUILDDIR=build-$(MCU)-$(BOARD)/
OBJDIR=$(BUILDDIR)obj/
BINDIR=$(BUILDDIR)bin/
DEMOBINDIR=$(BUILDDIR)bin/demo/
DIRS=$(BUILDDIR) $(OBJDIR) $(BINDIR) $(DEMOBINDIR)

all:	demo


$(DIRS): 
	mkdir $@

demo:	$(DIRS) $(DEMOBINDIR)input $(DEMOBINDIR)blink

clean:
	rm -rf $(DIRS) build-*

$(DEMOBINDIR)% : $(DEMOSRC)%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@



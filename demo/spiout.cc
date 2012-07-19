#include "avr-tino.h"
#include "avr-tino/SPI.h"
#include "avr-tino/delay.h"

/*
    This program demonstrate the use of
    the SPI interface to send a byte
*/
int main() {
    SPIMaster	spi;

    while(1) {
	spi.transfert(0x65);
	delay(50);
    }
    return 0;
}

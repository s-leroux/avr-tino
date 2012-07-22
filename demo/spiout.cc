#include "avr-tino.h"
#include "avr-tino/SPI.h"
#include "avr-tino/delay.h"

/*
    This program demonstrate the use of
    the SPI interface to send a byte
*/
int main() {
    typedef SPIMaster SPI;
    SPI::begin();

    while(1) {
	SPI::transfert(0x65);
	delay(5);
    }
    return 0;
}

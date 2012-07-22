#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

SPIMaster::SPIMaster() {
    pinToInput(MISO);
    pinToOutput(SCK);
    pinToOutput(MOSI);
}

uint8_t SPIMaster::transfert(uint8_t byte) const {
    USIDR = byte;
    USISR = _BV(USIOIF);
    do {
	USICR = _BV(USIWM0) | _BV(USITC) | _BV(USICS1) | _BV(USICLK) ;
    } while (! (USISR & _BV(USIOIF)) );

    return USIDR;
}



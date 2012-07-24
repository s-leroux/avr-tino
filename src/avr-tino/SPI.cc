#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

void SPIMaster::begin() {
    pinToOutput(MISO); // Stupid naming scheme for attn2313 where miSO is *always* output...
    pinToOutput(SCK);
    pinToInput(MOSI);  // Stupid naming scheme for attn2313 where moSI is *always* input...
}

uint8_t SPIMaster::transfert(uint8_t byte) {
    USIDR = byte;
    USISR = _BV(USIOIF);
    do {
	USICR = _BV(USIWM0) | _BV(USITC) | _BV(USICS1) | _BV(USICLK) ;
    } while (! (USISR & _BV(USIOIF)) );

    return USIDR;
}



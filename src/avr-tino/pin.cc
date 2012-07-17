#include "avr-tino.h"
#include "avr-tino/pin.h"

/*
    Based on shiftIn part of Arduino by David A. Mellis
*/
uint8_t shiftIn(pin_t dataPin, pin_t clockPin,
                bitorder_t bitOrder) {
    uint8_t value = 0;

    for (uint8_t i = 0; i < 8; ++i) {
	digitalWrite(clockPin, HIGH);
	if (bitOrder == LSBFIRST) {
	    value |= digitalRead(dataPin) << i;
	}
	else {
	    value |= digitalRead(dataPin) << (7 - i);
	}
	digitalWrite(clockPin, LOW);
    }

    return value;
}

/*
    Based on shiftIn part of Arduino by David A. Mellis
*/
void shiftOut(pin_t dataPin, pin_t clockPin,
                bitorder_t bitOrder,
                uint8_t val) {
    for (uint8_t i = 0; i < 8; ++i) {
	if (bitOrder == LSBFIRST) {
	    digitalWrite(dataPin, (pinstate_t)((val >> i) & 0x01));
	}
	else {
	    digitalWrite(dataPin, (pinstate_t)((val >> (7 - i)) & 0x01));
	}
	digitalWrite(clockPin, HIGH);
	digitalWrite(clockPin, LOW);
    }
}


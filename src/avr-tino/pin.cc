#include "avr-tino.h"
#include "avr-tino/pin.h"

void pinMode(pin_t pin, pinmode_t mode) {
    if (mode == INPUT) {
	pin_to_mode(pin) &= ~pin_to_mask(pin);
    }
    else {
	pin_to_mode(pin) |= pin_to_mask(pin);
    }
}

void digitalWrite(pin_t pin, pinstate_t state) {
    if (state == LOW) {
	pin_to_output(pin) &= ~pin_to_mask(pin);
    }
    else {
	pin_to_output(pin) |= pin_to_mask(pin);
    }
}

pinstate_t digitalRead(pin_t pin) {
    return ( pin_to_input(pin) & pin_to_mask(pin) ) ? HIGH : LOW;
}

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

void shiftOut(pin_t dataPin, pin_t clockPin,
                bitorder_t bitOrder,
                const void *data, uint16_t len) {
    for(uint16_t i = 0; i < len; ++i) {
	shiftOut(dataPin, clockPin, bitOrder, ((const uint8_t*)data)[i]);
    }
}

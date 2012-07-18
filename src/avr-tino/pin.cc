#include "avr-tino.h"
#include "avr-tino/pin.h"

void pinMode(pin_t pin, pinmode_t mode) {
    uint8_t port = pin_to_port(pin);

    if(port != NOT_A_PORT) {
        if (mode == INPUT) {
            *(uint8_t*)(pgm_read_word(&port_to_mode_PGM[port])) &= ~pin_to_mask(pin);
        }
        else {
            *(uint8_t*)(pgm_read_word(&port_to_mode_PGM[port])) |= pin_to_mask(pin);
        }
    }
    /* else what? */
}

void digitalWrite(pin_t pin, pinstate_t state) {
    uint8_t port = pin_to_port(pin);

    if(port != NOT_A_PORT) {
	if (state == LOW) {
	    *(uint8_t*)(pgm_read_word(&port_to_output_PGM[port])) &= ~pin_to_mask(pin);
	}
	else {
	    *(uint8_t*)(pgm_read_word(&port_to_output_PGM[port])) |= pin_to_mask(pin);
	}
    }
    /* else what? */
}

pinstate_t digitalRead(pin_t pin) {
    uint8_t port = pin_to_port(pin);

    if(port != NOT_A_PORT) {
	return ( *(uint8_t*)(pgm_read_word(&port_to_input_PGM[port])) & pin_to_mask(pin) ) ? HIGH : LOW;
    }
    /* else what? */

    return LOW;
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

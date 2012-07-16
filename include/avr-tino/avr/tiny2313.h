#if !defined AVR_TINO_TINY2313_H
#define AVR_TINO_TINY2313_H

/*
 List of pins for this mcu
*/
enum pin_t {
    PIN_PA0 = 0,
    PIN_PA1,
    PIN_PA2,
    PIN_PB0,
    PIN_PB1,
    PIN_PB2,
    PIN_PB3,
    PIN_PB4,
    PIN_PB5,
    PIN_PB6,
    PIN_PB7,
    PIN_PD0,
    PIN_PD1,
    PIN_PD2,
    PIN_PD3,
    PIN_PD4,
    PIN_PD5,
    PIN_PD6,
};

enum port_t {
    PORT_A,
    PORT_B,
    PORT_D,
};

#define pin_to_port(p) ( (p <= PIN_PA2) ? PORT_A : \
                         (p <= PIN_PB7) ? PORT_B : \
                         (p <= PIN_PD6) ? PORT_D : NOT_A_PORT )

#define pin_to_mask(p) ( (p <= PIN_PA2) ? _BV(p) : \
                         (p <= PIN_PB7) ? _BV(p-PIN_PB0) : \
                         _BV(p-PIN_PD0) )

// Some versions of gcc (like avr-g++ 4.3.5 / Debian Squeeze) issue a
// warning here:
//  "warning: only initialized variables can be placed into program memory area"
// It should be safetly ignored.
// I switched to gcc 4.7.1 and it vanished...
const uint16_t port_to_mode_PGM[] PROGMEM = {
    (uint16_t)&DDRA,
    (uint16_t)&DDRB,
    (uint16_t)&DDRD,
};

const uint16_t port_to_output_PGM[] PROGMEM = {
    (uint16_t)&PORTA,
    (uint16_t)&PORTB,
    (uint16_t)&PORTD,
};

const uint16_t port_to_input_PGM[] PROGMEM = {
    (uint16_t)&PINA,
    (uint16_t)&PINB,
    (uint16_t)&PIND,
};

#endif


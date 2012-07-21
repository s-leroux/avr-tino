#if !defined AVR_TINO_TINY2313_H
#define AVR_TINO_TINY2313_H

/*
 List of pins for this mcu
*/
enum __attribute__ ((__packed__)) port_t {
    PORT_BASE_A	= 0x10,
    PORT_BASE_B = 0x20,
    PORT_BASE_D = 0x30,
};
enum __attribute__ ((__packed__)) pin_t {
    PIN_PA0 = PORT_BASE_A | 0,
    PIN_PA1,
    PIN_PA2,
    PIN_PB0 = PORT_BASE_B | 0,
    PIN_PB1,
    PIN_PB2,
    PIN_PB3,
    PIN_PB4,
    PIN_PB5,
    PIN_PB6,
    PIN_PB7,
    PIN_PD0 = PORT_BASE_D | 0,
    PIN_PD1,
    PIN_PD2,
    PIN_PD3,
    PIN_PD4,
    PIN_PD5,
    PIN_PD6,
};

/* SPI interface */
static const pin_t MOSI = PIN_PB5;
static const pin_t MISO = PIN_PB6;
static const pin_t SCK	= PIN_PB7;

/*
 I/O Port abstraction layer
*/
/* Surprisely enougth the following macro if replaced by an inline function
   waste 6 bytes per call...
*/
#define pin_to_mask(p) ( _BV(p & 0x0F) )


/* ...but those are as efficient as their macro counterparts */
volatile uint8_t& pin_to_mode(pin_t p) {
    return ( (p <= PIN_PA2) ? DDRA :
	     (p <= PIN_PB7) ? DDRB :
	     DDRD );
}

volatile uint8_t& pin_to_output(pin_t p) {
    return ( (p <= PIN_PA2) ? PORTA :
	     (p <= PIN_PB7) ? PORTB :
	     PORTD );
}

volatile uint8_t& pin_to_input(pin_t p) {
    return ( (p <= PIN_PA2) ? PINA :
	     (p <= PIN_PB7) ? PINB :
	     PIND );
}


#endif


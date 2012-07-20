#if !defined AVR_TINO_TINY2313_H
#define AVR_TINO_TINY2313_H

/*
 List of pins for this mcu
*/
enum __attribute__ ((__packed__)) pin_t {
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

/* SPI interface */
static const pin_t MOSI = PIN_PB5;
static const pin_t MISO = PIN_PB6;
static const pin_t SCK	= PIN_PB7;

/*
 I/O Port abstraction layer
*/
#define pin_to_mask(p) ( (p <= PIN_PA2) ? _BV(p) : \
                         (p <= PIN_PB7) ? _BV(p-PIN_PB0) : \
                         _BV(p-PIN_PD0) )

#define pin_to_mode(p)  ( (p <= PIN_PA2) ? DDRA : \
                          (p <= PIN_PB7) ? DDRB : \
                          DDRD )
#define pin_to_output(p)( (p <= PIN_PA2) ? PORTA : \
                          (p <= PIN_PB7) ? PORTB : \
                          PORTD )
#define pin_to_input(p)	( (p <= PIN_PA2) ? PINA : \
                          (p <= PIN_PB7) ? PINB : \
                          PIND )

#endif


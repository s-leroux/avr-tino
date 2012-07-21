#if !defined avr_tino_pin_h
#define avr_tino_pin_h

#if !defined NOT_A_PORT
#  define NOT_A_PORT uint8_t(-1)
#else
#  error "Multiple definitions of NOT_A_PORT"
#endif

enum __attribute__ ((__packed__)) pinmode_t {
    INPUT = 0,
    OUTPUT,
};

enum __attribute__ ((__packed__)) pinstate_t {
    LOW,
    HIGH
};

//extern "C" {

#define pinToHigh(pin) ( pin_to_output(pin) |= pin_to_mask(pin) )
#define pinToLow(pin) ( pin_to_output(pin) &= ~pin_to_mask(pin) )
#define pinToOutput(pin) ( pin_to_mode(pin) |= pin_to_mask(pin) )
#define pinToInput(pin) ( pin_to_mode(pin) &= ~pin_to_mask(pin) )

void pinMode(pin_t pin, pinmode_t mode);
void digitalWrite(pin_t pin, pinstate_t state);
pinstate_t digitalRead(pin_t pin);

enum __attribute__ ((__packed__)) bitorder_t {
    LSBFIRST,
    MSBFIRST
};

uint8_t shiftIn(pin_t dataPin, pin_t clockPin, 
		bitorder_t bitOrder);
void shiftOut(pin_t dataPin, pin_t clockPin, 
		bitorder_t bitOrder, 
		uint8_t val);

void shiftOut(pin_t dataPin, pin_t clockPin,
                bitorder_t bitOrder,
                const void *data, uint16_t size);

//} // extern "C"

#endif

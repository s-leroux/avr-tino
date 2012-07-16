#if !defined avr_tino_pin_h
#define avr_tino_pin_h

#if !defined NOT_A_PORT
#  define NOT_A_PORT uint8_t(-1)
#else
#  error "Multiple definitions of NOT_A_PORT"
#endif

enum /* class */ pinmode_t {
    INPUT = 0,
    OUTPUT = 1
};

enum /* class */ pinstate_t {
    LOW,
    HIGH
};

inline int pinMode(pin_t pin, pinmode_t mode) {
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

inline void digitalWrite(pin_t pin, pinstate_t state) {
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


#endif

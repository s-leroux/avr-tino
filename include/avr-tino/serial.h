#if !defined AVRTINO_SERIAL_H
#define AVRTINO_SERIAL_H

// XXX should use namespace instead ?
class Serial {
    public:
    enum __attribute__ ((__packed__)) protocol_t {
	// Asynchronous
	A8N1	= 6 ,
	A8E1	= _BV(UPM1) | 6 ,
	A8O1	= _BV(UPM1) |_BV(UPM0) |  6 ,
	// Synchronous
	S8N1	= _BV(UMSEL) | 6 ,
	S8E1	= _BV(UMSEL) | _BV(UPM1) | 6 ,
	S8O1	= _BV(UMSEL) | _BV(UPM1) | _BV(UPM0) | 6 ,
    };

    static void begin(uint32_t baud, protocol_t protocol = A8N1) {
	uint16_t UBRR = F_CPU / 16 / baud - 1;
	
	UBRRH = (uint8_t)(UBRR >> 8);
	UBRRL = (uint8_t)(UBRR);
	
	UCSRB = _BV(RXEN) | _BV(TXEN);
	UCSRC = protocol;
    }

    static void send(uint8_t data) {
	while ( ! (UCSRA & _BV(UDRE)) ) {
	    // do nothing
	}
	UDR = data;
    }

    static uint8_t receive() {
	while ( ! (UCSRA & _BV(RXC) ) ) {
	    // do nothing
	}
	return UDR;
    }
};


#endif



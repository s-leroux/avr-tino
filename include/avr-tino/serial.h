#if !defined AVRTINO_SERIAL_H
#define AVRTINO_SERIAL_H

// XXX should use namespace instead ?
class Serial {
    public:
    enum __attribute__ ((__packed__)) protocol_t {
	// Asynchronous
	A8N1	= 3 ,
	A8E1	= _BV(UPM1) | 3 ,
	A8O1	= _BV(UPM1) |_BV(UPM0) |  3 ,
	// Synchronous
	S8N1	= _BV(UMSEL) | 3 ,
	S8E1	= _BV(UMSEL) | _BV(UPM1) | 3 ,
	S8O1	= _BV(UMSEL) | _BV(UPM1) | _BV(UPM0) | 3 ,
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
};


#endif



/*
  Copyright (c) 2012 Sylvain Leroux <sylvain@chicoree.fr>
  
  This file is part of avr-tino -- http://github.com/s-leroux/avr-tino
  
  avr-tino is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  avr-tino is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with avr-tino.  If not, see <http://www.gnu.org/licenses/>.
*/
#if !defined AVR_TINO_ZIGBEE_H
#define AVR_TINO_ZIGBEE_H

/**
    Support for Digi ZigBee serie 2 modules
*/

//uint64_t test = 0x1122334455667788ULL;

template <class UART>
class SimpleZigBee {
    public:
    typedef struct __attribute__ ((__packed__)) {
        uint8_t     byte[8];
    } Addr64; // use an array to overcome endianness

    typedef struct __attribute__ ((__packed__)) {
        uint8_t     byte[2];
    } Addr16; // use an array to overcome endianness

    public:
    static const uint8_t TRANSMIT_REQUEST = 0x10;
   
    struct __attribute__ ((__packed__)) TransmitRequest {
        uint8_t _frameType;
        uint8_t _frameID;
        Addr64  _destinationAddress64;
        Addr16  _destinationAddress16;
        uint8_t _broadcastRadius;
        uint8_t _options;

        TransmitRequest(void) {
            _frameType = TRANSMIT_REQUEST;
            memset(&_frameID, 0, sizeof(this)-1);
        }
    };

    static void sendRequest(uint16_t hLen, const void* header,
                            uint16_t dLen, const void* data) {
        OutputFrame of(hLen+dLen);

        of.send(hLen, header);
        of.send(dLen, data);
    }

    template <class R>
    static inline void sendRequest(const R& request,
                    uint16_t length, const void *data) {
        sendRequest(sizeof(R), &request, length, data);
    }

    static void sendTransmitRequest(Addr64 addr64, Addr16 addr16,
                    uint16_t length, const void *data) {
        TransmitRequest request;

        request._frameID = 0x01;
        request._destinationAddress64 = addr64;
        request._destinationAddress16 = addr16;

        sendRequest(request, length, data);
    }

    protected:
    static const uint8_t    API_START   = 0x7E;
    static const uint8_t    ESCAPE      = 0x7D;
    static const uint8_t    XON         = 0x11;
    static const uint8_t    XOFF        = 0x12;

    class OutputFrame {
        public:
        OutputFrame(uint16_t length)  : _checksum(0xFF) {
            sendRaw(API_START);
            send(uint8_t(length >> 8));
            send(uint8_t(length & 0xFF));
        }

        ~OutputFrame() {
            send(_checksum);
        }

        inline void sendRaw(uint8_t byte) {
            UART::send(byte);
        }

        inline void send(uint8_t byte) {
            switch (byte) {
                case API_START:
                case ESCAPE:
                case XON:
                case XOFF:      UART::send(ESCAPE);
                                byte ^= 0x20;
                                // don't break here !
                default:        UART::send(byte);
            }
        }

        void send(uint16_t length, const void* data) {
            const uint8_t *buffer = (const uint8_t*)data;
            while(length--) {
                uint8_t byte = *buffer++;
                _checksum -= byte;
                
                send(byte);
            }
        }

        uint8_t _checksum;
    };
};

template <class UART>
class ZigBee {
    public:
    typedef struct __attribute__ ((__packed__)) {
        uint8_t     byte[8];
    } Addr64; // use an array to overcome endianness

    typedef struct __attribute__ ((__packed__)) {
        uint8_t     byte[2];
    } Addr16; // use an array to overcome endianness

    struct  TransmitRequest {
        public:
        TransmitRequest()
        {
            memset(&_frame._frameID, 0, sizeof(_frame)-1);

            _frame._frameType = 0x10;
            _frame._frameID = 0x01;
        }

        void send(uint16_t length, const void* data) {
            Frame   frame(length+sizeof(_frame));

            frame.send(sizeof(_frame), &_frame);
            frame.send(length, data);
        }

        struct __attribute__ ((__packed__)) {
            uint8_t _frameType;
            uint8_t _frameID;
            Addr64  _destinationAddress64;
            Addr16  _destinationAddress16;
            uint8_t _broadcastRadius;
            uint8_t _options;
        } _frame;
    };

    static void sendData(Addr64 addr64, Addr16 addr16, 
                    uint16_t length, const void *data) {
        TransmitRequest req;
        req._frame._destinationAddress64 = addr64;
        req._frame._destinationAddress16 = addr16;

        req.send(length,data);
    }
    
    public:
    class Frame {
        public:
        Frame(uint16_t length) : _checksum(0xFF) {
            sendRaw(API_START);
            sendEscaped(uint8_t(length >> 8));
            sendEscaped(uint8_t(length &0xFF));
        }

        ~Frame() {
            sendEscaped(_checksum);
        }

        inline void sendRaw(uint8_t byte) {
            UART::send(byte);
        }

        inline void sendEscaped(uint8_t byte) {
            switch (byte) {
                case API_START:
                case ESCAPE:
                case XON:
                case XOFF:      UART::send(ESCAPE);
                                byte ^= 0x20;
                                // don't break here !
                default:        UART::send(byte);
            }
        }

        void send(uint16_t length, const void* data) {
            const uint8_t *buffer = (const uint8_t*)data;
            while(length--) {
                uint8_t byte = *buffer++;
                _checksum -= byte;
                
                sendEscaped(byte);
            }
        }

        private:
        static const uint8_t    API_START   = 0x7E;
        static const uint8_t    ESCAPE      = 0x7D;
        static const uint8_t    XON         = 0x11;
        static const uint8_t    XOFF        = 0x12;
        uint8_t _checksum;
    };
/**
    The UART template parameter should provide the following methods:
        void UART::send(uint8_t);
        uint8_t UART::receive(void);
*/

};
#if 0
// XXX should use namespace instead ?
template <uint8_t DR, uint8_t SRA, uint8_t SRB, uint8_t SRC, uint8_t RRL, uint8_t RRH>
class Serial : public Printer<Serial<DR,SRA,SRB,SRC,RRL,RRH> > {
    public:
    /*
	Hard coded values common (?) to every 8-bit AVR
	XXX
    */
    enum __attribute__ ((__packed__)) protocol_t {
	// Asynchronous
	A8N1	= 6 ,
	A8E1	= _BV(5) | 6 ,
	A8O1	= _BV(5) |_BV(4) |  6 ,
	// Synchronous
	S8N1	= _BV(6) | 6 ,
	S8E1	= _BV(6) | _BV(5) | 6 ,
	S8O1	= _BV(6) | _BV(5) | _BV(4) | 6 ,
    };

    enum {
	f_TXEN	    = 3,
	f_RXEN	    = 4,
    };

    enum {
	f_UDRE	    = 5,
	f_TXC	    = 6,
	f_RXC	    = 7,
    };

    static void begin(uint32_t baud, protocol_t protocol = A8N1) {
	uint16_t UBRR = F_CPU / 16 / baud - 1;
	
	_SFR_MEM8(RRH) = (uint8_t)(UBRR >> 8);
	_SFR_MEM8(RRL) = (uint8_t)(UBRR);
	
	_SFR_MEM8(SRB) = _BV(f_RXEN) | _BV(f_TXEN);
	_SFR_MEM8(SRC) = protocol;
    }

    static void write(const char* str) {
	while(*str) { send(*str++); }
    }

    static void send(uint8_t data) { // XXX Shouldn't we call that 'write' ?
	while ( ! (_SFR_MEM8(SRA) & _BV(f_UDRE)) ) {
	    // do nothing
	}
	_SFR_MEM8(DR) = data;
    }

    static void send(const void* data, uint8_t len) {
	for(uint8_t i = 0; i < len; ++i) {
	    send(((const uint8_t*)data)[i]);
	}
    }

    static uint8_t receive() {
	while ( ! (_SFR_MEM8(SRA) & _BV(f_RXC) ) ) {
	    // do nothing
	}
	return _SFR_MEM8(DR);
    }
};

#endif

#endif



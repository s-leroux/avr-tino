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
            UART::send(API_START);
            UART::send(uint8_t(length >> 8));
            UART::send(uint8_t(length &0xFF));
        }

        ~Frame() {
            UART::send(_checksum);
        }

        void send(uint16_t length, const void* data) {
            const uint8_t *buffer = (const uint8_t*)data;
            while(length--) {
                uint8_t byte = *buffer++;
                UART::send(byte);
                _checksum -= byte;
            }
        }

        private:
        static const uint8_t    API_START   = 0x7E;
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


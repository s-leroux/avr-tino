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

#include <util/atomic.h>
#include "avr-tino/queue.h"

static const uint8_t    API_START   = 0x7E;
static const uint8_t    ESCAPE      = 0x7D;
static const uint8_t    XON         = 0x11;
static const uint8_t    XOFF        = 0x12;

typedef struct __attribute__ ((__packed__)) {
    uint8_t     byte[8];
} Addr64; // use an array to overcome endianness

typedef struct __attribute__ ((__packed__)) {
    uint8_t     byte[2];
} Addr16; // use an array to overcome endianness

static const uint8_t AT_COMMAND       = 0x08;
struct __attribute__ ((__packed__)) ATCommand {
    uint8_t     frameType;
    uint8_t     frameID;
    char        command[2];


};

static const uint8_t TRANSMIT_REQUEST = 0x10;
struct __attribute__ ((__packed__)) TransmitRequest {
    uint8_t     frameType;
    uint8_t     frameID;
    Addr64      destinationAddress64;
    Addr16      destinationAddress16;
    uint8_t     broadcastRadius;
    uint8_t     options;

//        TransmitRequest(void) {
//            _frameType = TRANSMIT_REQUEST;
//            memset(&_frameID, 0, sizeof(*this)-1);
//        }
};

static const uint8_t AT_COMMAND_RESPONSE = 0x88;
struct __attribute__ ((__packed__)) ATCommandResponse {
    uint8_t     frameType;
    uint8_t     frameID;
    char        atCommand[2];
    uint8_t     commandStatus;
    uint8_t     commandData[0];
};

static const uint8_t MODEM_STATUS = 0x8A;
struct __attribute__ ((__packed__)) ModemStatus {
    uint8_t     frameType;
    uint8_t     status;

    enum __attribute__ ((__packed__)) {
        stHardwareReset             = 0x00,
        stWatchdogTimerReset        = 0x01,
        stJoinedNetwork             = 0x02,
        stDisassociated             = 0x03,
        stCoordinatorStarted        = 0x06,
        stSecurityKeyUpdated        = 0x07,
        stConfigChangeWhileJoined   = 0x11,
    };
};

static const uint8_t TRANSMIT_STATUS = 0x8B;
struct __attribute__ ((__packed__)) TransmitStatus {
    uint8_t     frameType;
    uint8_t     frameID;
    Addr16      destinationAddress16;
    uint8_t     transmitRetryCount;
    uint8_t     deliveryStatus;
    uint8_t     discoveryStatus;

    enum __attribute__ ((__packed__)) DeliveryStatus {
        stSuccess                   = 0x00,
        stMACACKFailure             = 0x01,
        stCCAFailure                = 0x02,
        stInvalidDestinationEndpoint= 0x15,
        stNetworkACKFailure         = 0x21,
        stNotJoined                 = 0x22,
        stSelfAddressied            = 0x23,
        stAddressNotFound           = 0x24,
        stRouteNotFound             = 0x25,
        stBroadcastNoNeighborRelay  = 0x26,
        stInvalidBindingTableIndex  = 0x2B,
        stResourceError             = 0x2C,
        stBraodcastWithAPS          = 0x2D,
        stUnicastAPSWithEE0         = 0x2E,
        stResourceError2            = 0x32,
        stDataPayloadTooLarge       = 0x74,
        stIndirectMessageUnrequested= 0x75,
    };

    enum __attribute__ ((__packed__)) DiscoveryStatus {
        /* those are *probably* bit mask but I can't find it in the
           datasheet */
        stNoDiscoveryOverhead       = 0x00,
        stAddressDicovery           = 0x01,
        stRouteDicovery             = 0x02,
        stAddressAndRouteDicovery   = 0x03,
        stExtendedTimeoutDicovery   = 0x40,
    };
};

static const uint8_t RECEIVE_PACKET = 0x90;
struct __attribute__ ((__packed__)) ReceivePacket {
    uint8_t     frameType;
    Addr64      destinationAddress64;
    Addr16      destinationAddress16;
    uint8_t     options;
    uint8_t     data[0];
};

struct __attribute__ ((__packed__)) APIFrame {
    union __attribute__ ((__packed__)) {
        TransmitRequest         transmitRequest;
        ATCommand               atCommand;
        ATCommandResponse       atCommandResponse;
        ModemStatus             modemStatus;
        TransmitStatus          transmitStatus;
        ReceivePacket           receivePacket;

        uint8_t                 frameType;
        uint8_t                 raw[0];
    };
};

template <class UART>
class SimpleZigBee {
    public:
   

    static void sendFrame(uint16_t hLen, const void* header,
                            uint16_t dLen, const void* data) {
        OutputFrame of(hLen+dLen);

        of.send(hLen, header);
        of.send(dLen, data);
    }

    template <class F>
    static inline void sendFrame(const F& frame,
                    uint16_t length, const void *data) {
        sendFrame(sizeof(F), &frame, length, data);
    }

    static void sendTransmitRequest(Addr64 addr64, Addr16 addr16,
                    uint16_t length, const void *data) {
        TransmitRequest request;

        request.frameType = TRANSMIT_REQUEST;
        request.frameID = 0x01;
        request.destinationAddress64 = addr64;
        request.destinationAddress16 = addr16;
        request.broadcastRadius = 0;
        request.options = 0;

        sendFrame(request, length, data);
    }

    static void sendATCommand(char c1, char c2) {
        ATCommand request;

        request.frameType = AT_COMMAND;
        request.command[0] = c1;
        request.command[1] = c2;

        sendFrame(request, 0, NULL);
    }

    static void sendATCommand(uint16_t cmd) {
        ATCommand request;

        request.frameType = AT_COMMAND;
        request.command[0] = cmd>>8;
        request.command[1] = cmd;

        sendFrame(request, 0, NULL);
    }

    protected:
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

class ZBReader {
    public:
    ZBReader() {
        reset();
    }

    const APIFrame* frame() volatile const {
        const void * result;

        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            result = _q.first();
            if ((!result) || (result == _frame))
                result = NULL;
        }

        return (APIFrame*)result;
    }

    const uint8_t frame_size(const void* frame) const {
        return _q.size(frame);
    }

    void next() volatile {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        {
            _q.next();
        }
    }

    void append(uint8_t byte) {
        switch (byte) {
            case API_START: 
                    // New frame.
                    reset();
                    _mode = LOOK_HI_LENGTH;
                    break;
            case ESCAPE:    
                    _escaping = true;
                    break;
            default:        
                    if (_escaping) {
                        byte ^= 0x20;
                        _escaping = false;
                    }
                    switch(_mode) {
                        case LOOK_HI_LENGTH:
                                if (byte != 0) {
                                    // only support data up to 254
                                    // bytes
                                    reset();
                                }
                                else {
                                    _mode = LOOK_LO_LENGTH;
                                }
                                break;
                        case LOOK_LO_LENGTH:
                                byte = byte+1; // incl. checksum
                                if ((!byte) || 
                                    ((_frame=(uint8_t*)_q.alloc(byte)) == NULL)) {
                                    reset();
                                }
                                else {
                                    _end = byte;
                                    _mode = LOOK_DATA;;
                                }
                                break;
                        case LOOK_DATA:
                                _frame[_idx++] = byte;
                                _cs += byte;
                                if (_idx == _end) {
                                    if (_cs != 0xFF) {
                                        reset();
                                    }
                                    else {
                                        /* ready for next frame */
                                        _frame = NULL;
                                        _mode = SYNC;
                                    }
                                }
                    }
        }
        
    }

    protected:
    void reset() {
        /** ready for next incomming frame */
        if (_frame)
            _q.discard(_frame);

        _escaping = false;
        _mode = SYNC;
        _frame = NULL;
        _idx = 0;
        _end = 0;
        _cs = 0;
    }

    private:
    enum Mode {
        LOOK_HI_LENGTH,
        LOOK_LO_LENGTH,
        LOOK_DATA,
        OK,
        SYNC,
    };

    Queue               _q;

    bool                _escaping;
    Mode                _mode;
    uint8_t             *_frame; // ptr to the current frame
    uint8_t             _idx;    // current position
    uint8_t             _end;    // assumed end of the current frame
    uint8_t             _cs;   // checksum of the *current* frame.
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



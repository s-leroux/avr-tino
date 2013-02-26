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
#if !defined AVR_TINO_TWI_H
#define AVR_TINO_TWI_H

#include "avr-tino/printer.h"

/**
    Two Wire Interface
    
    2-wire is an i2c compatible protocol supported on some AVR (ATMega328)
*/
template<typename Impl> class TWI_Start;
template<typename Impl> class TWI_SLA;

template<typename Impl> 
class TWI_Common {
    public:
    /**
	Send a start condition
    */
    static bool start(void) {
	_SFR_MEM8(Impl::CR) = Impl::START;

	return true;
    }

    /**
	Send a stop condition
    */
    static bool stop(void) {
	_SFR_MEM8(Impl::CR) = Impl::STOP;

	return true;
    }
    
    /**
	Active wait for 2-wire op. completion
    */
    static void wait() {
	while(! ( _SFR_MEM8(Impl::CR) & Impl::SEND)) {
	    /* do nothing */
	}
    }

    /** wait for transmission to complete and expect a status */
    static bool isState(uint8_t status) {
	wait();
    
	return ((_SFR_MEM8(Impl::SR) & 0xF8) == status);
    }

    static bool write_if_state(uint8_t byte, uint8_t state) {
	if (!isState(state))
	    return false;

	_SFR_MEM8(Impl::DR) = byte;
	_SFR_MEM8(Impl::CR) = Impl::WRITE;
	
	return true;
    }

    static bool write_sla(uint8_t addr, bool w = false) {
	return write_if_state(addr | ( w ? 0x00 : 0x01), Impl::MT_START);
    }

    static bool read_if_state(uint8_t *byte, uint8_t state) {
	if (!isState(state))
	    return false;

	*byte = _SFR_MEM8(Impl::DR);

	return true;
    }

    static void read_next_ack() {
	_SFR_MEM8(Impl::CR) = Impl::READ_ACK;
    }

    static void read_next_nack() {
	_SFR_MEM8(Impl::CR) = Impl::READ_NACK;
    }
};

template <typename Impl>
class TWI {
    public:
    class Device : public Printer<Device> {
	public:
	Device(uint8_t addr) : _addr(addr) {}

	bool write(uint8_t len, const uint8_t * buffer) {
	    return TWI<Impl>::write(len, buffer, _addr);
	};

	bool read(uint8_t len, uint8_t * buffer) {
	    return TWI<Impl>::read(len, buffer, _addr);
	}

	private:
	const uint8_t _addr;
    };

    static Device device(uint8_t addr) { return Device(addr); }

    /**
	Master write
    */
    static bool write(uint8_t len, const uint8_t * buffer, uint8_t addr) {
	TWBR = 2; // XXX debug

	bool result = TWI_Common<Impl>::start();
	
	if (result)
	    result = TWI_Common<Impl>::write_sla(addr, true);

	if (result && len--) {
	    result = TWI_Common<Impl>::write_if_state(*buffer++, Impl::MT_W_SLA_ACK);
	}

	while (result && len--) {
	    result = TWI_Common<Impl>::write_if_state(*buffer++, Impl::MT_W_DATA_ACK);
	}

	if (result)
	    TWI_Common<Impl>::wait();
	TWI_Common<Impl>::stop();
	
	return result;
    }

    static bool read(uint8_t len, uint8_t * buffer, uint8_t addr) {
	bool result = TWI_Common<Impl>::start();

	if (result)
	    result = TWI_Common<Impl>::write_sla(addr, false);

	uint8_t state = Impl::MR_R_SLA_ACK;

	while (result && len-- > 0) {
	    TWI_Common<Impl>::read_next_ack();
	    result = TWI_Common<Impl>::read_if_state(buffer++, state);
	    state = Impl::MR_R_DATA_ACK;
	}

	if (result) {
	    TWI_Common<Impl>::read_next_nack();
	    result = TWI_Common<Impl>::read_if_state(buffer++, state);
	}

	if (result)
	    TWI_Common<Impl>::wait();
	TWI_Common<Impl>::stop();

	return result;
    }


    /**
	Write a byte
    */
    static bool write(uint8_t byte) {
	
    }
};


#endif



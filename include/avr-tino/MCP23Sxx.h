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
#if !defined AVR_TINO_MCP23SXX_H
#define AVR_TINO_MCP23SXX_H

/*
 * Interface file for the MCP23Sxx I/O Expander With SPI
 */
template<class SPI, pin_t cs> class MCP23Sxx {
    public:
    MCP23Sxx();

    void reset() const;

    /* Common interface */
    enum __attribute__ ((__packed__)) regs {
	IODIR	    = 0x00, /* I/O direction register */
	IOCON	    = 0x05, /* I/O expander configuration register */
	GPIO	    = 0x09, /* general purpose I/O port register */
    };

    /**
	Get a register value
    */
    uint8_t get(regs r, uint8_t addr = 0x00) const;

    /**
	Set a register value
    */
    void set(regs r, uint8_t addr, uint8_t value) const;
    inline void set(regs r, uint8_t value) const {
	set(r, 0x00, value);
    }

    /**
	Set and/or clear some bits of a register
    */
    void update(regs r, uint8_t addr, uint8_t mask, uint8_t value) const;
    inline void update(regs r, uint8_t mask, uint8_t value) const {
	update(r, 0x00, mask, value);
    }


    /**
	Set the direction of the I/O pins

	Only bits with a corresponding 1 in the mask are affected.
	If direction is set to 1, the correspondig I/O pin is set to
	input.
    */
    void setDirection(uint8_t addr, uint8_t mask, uint8_t direction) const;

    /**
	Set the general purpose the I/O pins

	With default IPOL a 1 set the correspnding pin to logic high.
	Only bits with a corresponding 1 in the mask are affected.
    */
    void setGPIO(uint8_t addr, uint8_t mask, uint8_t state) const;
    
    void enableHardwareAddress(uint8_t addr) const;

    public:
    enum __attribute__ ((__packed__)) opcode {
	WRITE	= 0x40,
	READ	= 0x41,
    };

    enum __attribute__ ((__packed__)) iocon_bits {
	INPOL	= 1,
	ODR,
	HAEN,
	DISSLW,
	SEQOP,
    };

    private:
    class Command : GuardPinLow<cs> {
	public:
	Command(uint8_t c) {
	    SPI::transfert(c);
	}

	void write(uint8_t byte) const {
	    SPI::transfert(byte);
	}

	uint8_t read() const {
	    return SPI::transfert(0xFF);
	}
    };
};

#endif


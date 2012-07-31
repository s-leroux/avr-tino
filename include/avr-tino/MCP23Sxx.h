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

    /* Reset by lowering the correspondig pin on the mcu */
    void reset() const;

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

    public:
    enum __attribute__ ((__packed__)) regs {
	IODIR	    = 0x00, /* I/O direction register */
	GPIO	    = 0x09, /* general purpose I/O port register */
    };

    enum __attribute__ ((__packed__)) opcode {
	WRITE	= 0x40,
	READ	= 0x41,
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
    };
};

#endif


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
#if !defined AVR_TINO_HD44780_H
#define AVR_TINO_HD44780_H

enum __attribute__((__packed__)) message_t {
    DATA_MESSAGE,
    INSTRUCTION_MESSAGE,
};

/* Abstract out access to the pin interface */
template<pin_t DataBase, pin_t RS, pin_t E>
class Interface4Bits {
    public:
    static void begin(message_t type);

    static void init(void);
    static inline void write(uint8_t data) { write(data, false); }

    private:
    static void write(uint8_t data, bool highHalf);
};

/*
 * Interface file for LCD displays that are compatible with
 * the Hitachi HD44780 driver. This includes many LCD displays
 * like the low-cost 1602.
 */
template <class Interface>
class HD44780 {
    public:
    HD44780();

    /* Init the LCD display - reset cursor to initial position */
    void init() const;

    void move(uint8_t x, uint8_t y) const;

    /**
	Print a character on the display
    */
    void print(char c) const;

    private:
    class Instruction {
	public:
	Instruction() {
	    Interface::begin(INSTRUCTION_MESSAGE);
	}

	/** Initialisation sequence for the device */
	void init() const;

	/** Move the cursor */
	void move(uint8_t x, uint8_t y) const;
    };

    class DataMessage {
	public:
	DataMessage() {
	    Interface::begin(DATA_MESSAGE);
	}

	/** Write one character on the device */
	void write(char c) const; 
    };
};

#endif


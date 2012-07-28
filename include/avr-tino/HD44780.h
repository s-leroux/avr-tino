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

    static void init(uint8_t function_set);
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

    /* Clear the display */
    void clear() const;

    /* Set display on */
    inline void display() const { setDisplayControl(DISPLAY_ON); };

    /* Set display off */
    inline void noDisplay() const { setDisplayControl(0); };

    /**
	Print a character on the display
    */
    void print(char c) const;

    /* print C-string */
    void print(const char* str) const;

    enum __attribute__((__packed__)) entry_mode_t {
	CURSOR_MOVE_LEFT    =	0x00,
	CURSOR_MOVE_RIGHT   =	0x02,
	DISPLAY_SHIFT_LEFT  =	0x03,
	DISPLAY_SHIFT_RIGHT  =	0x01,
    };

    void setEntryMode(entry_mode_t mode) const;

    enum __attribute__((__packed__)) display_control_mask {
	DISPLAY_ON	= 0x04,
	CURSOR_ON	= 0x02,
	CURSOR_BLINK	= 0x01,
    };

    /* Combination of the above
	XXX should I use bitfield instead?
    */
    typedef uint8_t display_control_t;

    void setDisplayControl(display_control_t mode) const;

    enum __attribute__((__packed__)) function_set_mask {
	EIGHT_BITS	= 0x10,
	TWO_LINES	= 0x08,
	FONT_5X10	= 0x04,
    };

    /* Combination of the above
	XXX should I use bitfield instead?
    */
    typedef uint8_t function_set_t;

    private:
    class Instruction {
	public:
	Instruction() {
	    Interface::begin(INSTRUCTION_MESSAGE);
	}

	/** Initialisation sequence for the device */
	void init(uint8_t function_set) const;

	/** Move the cursor */
	void move(uint8_t x, uint8_t y) const;

	/** Clear the display */
	void clear() const;

	void setEntryMode(entry_mode_t mode) const;

	void setDisplayControl(display_control_t mode) const;

	enum __attribute__((__packed__)) instruction_t {
	    CLEAR_DISPLAY   = 0x01,
	    RETURN_HOME	    = 0x02, // kind o'reset
	    ENTRY_MODE_SET  = 0x04,
	    DISPLAY_CONTROL = 0x08,
	    CURSOR_SHIFT    = 0x10,
	    FUNCTION_SET    = 0x20,
	    SET_CGRAM_ADDR  = 0x40,
	    SET_DDRAM_ADDR  = 0x80,
	};
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


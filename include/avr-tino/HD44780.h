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

/*
 * Interface file for LCD displays that are compatible with
 * the Hitachi HD44780 driver. This includes many LCD displays
 * like the low-cost 1602.
 */

/* Abstract out access to the pin interface */
template<pin_t DataBase, pin_t RS, pin_t E>
class Interface4Bits {
    public:
    static void init();
    static void writeData(uint8_t data);
    static void writeInstruction(uint8_t data);

    class Message {
	public:
	Message(bool isData);
	void write(uint8_t data, bool highHalf = false);
    };
};

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

    enum __attribute__ ((__packed__)) mode_t {
	NORMAL	        = 0,  /* Normal mode         000 */
	SLEEP	        = 1,  /* Sleep mode          001 */
        LOOPBACK        = 2,  /* Loopback mode       010 */
        LISTEN          = 3,  /* Listen only mode    011 */
        CONFIGURATION	= 4,  /* Configuration mode  100 */
    };
};

#endif


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
#if !defined AVR_TINO_PRINTER_H
#define AVR_TINO_PRINTER_H

#include <string.h>
/*
    Printer object to display formated data.
*/
template<class T>
class Printer {
    public:
    Printer(const T& dest) : _dest(dest) { }

    /* virtual ~Printer(void) {} */

    /**
	Print a row sequence of bytes

	All 'print(...)' methors are implemented in terms of write().
    */
    void write(const char* data) const {
	_dest.write(data);
    }

    /**
	Print one char
    */
    void print(char c) const { char s[2] = { c, 0 }; write(s); }

    void print(int i) const;
   
    /**
	Print a C-string
    */
    void print(const char* str) const { write(str); }

    private:
    const T&	_dest;
};

#endif


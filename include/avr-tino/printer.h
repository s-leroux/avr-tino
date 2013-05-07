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
    Text printing functions

    All default implementations delegate to the destination
    print(const char*) method.
*/
template<class T>
void print(const T& dest, const char* str) {
    dest.print(str);
}

template<class T>
void print(const T& dest, char c) {
    char s[2] = { c, 0 }; 

    print(dest, s); 
}

template<class T>
void print(const T& dest, int i, uint8_t base = 10);

/**
    Forward all 'print' method to T::write(const char*)
*/
template<class T>
class Printer {
    public:
    static void print(const char* str) {
	T::write(str);
    }

    static void print(char c) {
	char s[2] = { c, 0 }; 

	T::write(s); 
    }

    static void print(float v);

    static void print(int16_t i, uint8_t base = 10);
    static void print(uint16_t i, uint8_t base = 10) {
	// XXX fixme
	// fall-back to print(int, base)
	print((int16_t)i, base);
    }

    static void bindump(uint8_t len, const void* data) {
	const uint8_t *src = (const uint8_t*)data;
	while(len--) {
	    uint8_t byte = *src++;

	    for(uint8_t i = 0; i < 8; ++i) {
		print( (byte & 0x80) ? "1" : "0");
		byte <<= 1;
	    }
	    print(" ");
	}
    }

    template <class Blob>
    static void bindump(const Blob* data) {
	bindump(sizeof(Blob), data);
    }
};


#endif


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
void print(const T& dest, int i);

#endif


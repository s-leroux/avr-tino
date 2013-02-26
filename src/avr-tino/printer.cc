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
#include "avr-tino/printer.h"

#include <stdlib.h>

template<class T>
void Printer<T>::print(int n, uint8_t base) {
    static const int LEN = 18;
    char	buffer[LEN]; // int goes up to 32767 -- that is 5 digits

    itoa(n, buffer, base);
    T::print(buffer);
}

template<class T>
void print(const T& dest, int n, uint8_t base) {
    static const int LEN = 18;
    char	buffer[LEN]; // int goes up to 32767 -- that is 5 digits

    itoa(n, buffer, base);
    print(dest, buffer);
}


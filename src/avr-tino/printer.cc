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

void Printer::print(int n) const {
    static const int LEN = 7;
    char	buffer[LEN]; // int goes up to 32767 -- that is 5 digits
    char	*ptr = &buffer[LEN-1];

    *ptr = 0;
    if (n == 0) {
	*--ptr = '0';
    }
    else {
	bool	positive;
	if (n >= 0) {
	    positive = true;
	    n = -n;
	}
	else {
	    positive = false;
	}

	while(n) {
	    int8_t remainder = n % 10;
	    n /= 10;

	    *--ptr = '0' - remainder;
	}
	if (!positive) {
	    *--ptr = '0';
	}
    }
    print(ptr);
}


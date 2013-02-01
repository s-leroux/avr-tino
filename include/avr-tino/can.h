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
#if !defined AVR_TINO_CAN_H
#define AVR_TINO_CAN_H

/**
    CAN (Controller Area Network) common data structures
*/
namespace CAN {

struct __attribute__ ((__packed__)) ID {
    union {
	struct {
	    uint8_t	    sidh;
	    uint8_t	    sidl;
	    uint8_t	    eid8;
	    uint8_t	    eid0;
	};

	uint32_t	    addr; // beware of endianness !!!
    };
};

// bit 31                          bit 0
// |                              |
// < SIDH >< SIDL >< EID8 >< EID0 >
// <   SID   >xxx<       EID      >
// 10987654321098765432109876543210


}; // namespace


#endif

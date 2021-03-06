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
#if !defined avr_tino_h
#define avr_tino_h

extern "C" void __cxa_pure_virtual(void) { /* do nothing */ }

//
// Core includes

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "avr-tino/hardware/board-def.h"
#include "avr-tino/hardware/mcu-def.h"

#include "avr-tino/pin.h"
#include "avr-tino/bits.h"

#endif

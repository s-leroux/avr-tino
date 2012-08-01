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

#include "avr-tino.h"

#include "avr-tino/delay.h"
#include "avr-tino/MCP23Sxx.h"
#include "avr-tino/SPI.h"

int main() __attribute__ ((OS_main));
int main() {
    typedef SPIMaster SPI;

    /* MCP2515 reset */
    const MCP23Sxx<SPI, PIN_PB0>	mcp23S08;

    SPI::begin();
    mcp23S08.reset();
    mcp23S08.enableHardwareAddress(0x00);
    mcp23S08.setDirection(0x00, 0xFF, 0x00);

    while(1) {
	mcp23S08.setGPIO(0x00, 0xFF, 0x0F);
	delay(1500);
	mcp23S08.setGPIO(0x00, 0xFF, 0xF0);
	delay(500);
    }

    return 0;
}

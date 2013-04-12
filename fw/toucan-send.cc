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
/*
    Test program to use with my MCP2515 based CANModule
*/

#include <avr/interrupt.h>

#include "avr-tino.h"

#include "avr-tino/delay.h"
#include "avr-tino/toucan.h"
#include "avr-tino/MCP2515.h"
#include "avr-tino/SPI.h"
// XXX Must be done otherwise
#include "avr-tino/target/CANModule.h"

/*
    touCAN demonstration program
*/

typedef SPIMaster SPI;
typedef MCP2515<SPI, MCP2515_CS>	CAN_CTRL;

static const CAN_CTRL	mcp2515;


static CAN_CTRL::Frame frame = { TOUCAN_OID(0, 0, TOUCAN_CMD_SET, 0, 0), 1 };

int main() __attribute__ ((OS_main));
int main() {

    /* MCP2515 reset */

    SPI::begin();
    mcp2515.reset();
    mcp2515.setBaud(100000);
    mcp2515.RXB0.setMask(TOUCAN_CONFIG_MASK);
    mcp2515.RXB0.setMode(mcp2515.RXM_EID);
    mcp2515.setFilter(CAN_CTRL::RXF0, TOUCAN_CONFIG_OID);
    mcp2515.setOperationMode(mcp2515.NORMAL);

    DDRD = 0xFF;
    DDRB |= _BV(6);

    while(1) {
	uint8_t data = PIND;

	frame.data[0] = data;
	mcp2515.TXB0.loadTX(&frame);

	delay(1000);
    }

    return 0;
}






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

static const uint16_t	CID	= TOUCAN_CID_DIGITAL_INPUT;
static TouCAN::OID	self	= TOUCAN_OID(0, CID, 0, 0, 0x0000);

static const CAN_CTRL	mcp2515;

static CAN_CTRL::Frame	frame;
static volatile bool	empty	= true;

ISR(PCINT_vect) {
    if (empty) {
	uint8_t port = PINB;
	if (! (PINB & _BV(0)) ) {
	    mcp2515.RXB0.readRX(&frame);
	    empty = false;
	}
    }
    else {
	// discard
    }
}

int main() __attribute__ ((OS_main));
int main() {

    /* MCP2515 reset */

    SPI::begin();
    mcp2515.reset();
    mcp2515.setBaud(100000);

    mcp2515.setOperationMode(mcp2515.NORMAL);
    mcp2515.RXB0.setMode(mcp2515.RXM_EID);

    GIMSK |= _BV(PCIE);
    PCMSK |= _BV(PCINT0);

    // 1) TouCAN device auto ID assignation
    //	    The device take one random address
    //	    Ping that address
    //	    If no response is obtained after 1000ms keep that address
    do {
	empty = true;
	self.eid0++;
	mcp2515.setOperationMode(mcp2515.CONFIGURATION);
	mcp2515.RXB0.setMask(TOUCAN_OID_DEV_MASK);
	mcp2515.setFilter(CAN_CTRL::RXF0, self);
	mcp2515.setOperationMode(mcp2515.NORMAL);
	mcp2515.TXB0.loadTX(TouCAN::ping(self, self));
	
	delay(1000);
    } while(!empty);

    while(1) {
	if (!empty) {
	    if (TouCAN::match(self, frame.id)) {
		if (TOUCAN_CMD(frame.id) == TOUCAN_CMD_PING) {
		    mcp2515.TXB0.loadTX(TouCAN::echo(TOUCAN_PING(frame).from, self));
		}
	    }
	}
    }

    return 0;
}






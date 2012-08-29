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


#include "avr-tino.h"

#include "avr-tino/delay.h"
#include "avr-tino/serial.h"
#include "avr-tino/MCP2515.h"
#include "avr-tino/SPI.h"
// XXX Must be done otherwise
#include "avr-tino/target/CANModule.h"

/*
    Sample program to use with my MCP2515-based CAN module.

    This program logs to serial incomming CAN frames. Mostly for
    debugging purpose
*/

typedef SPIMaster SPI;
typedef MCP2515<SPI, MCP2515_CS>	CAN_CTRL;

int main() __attribute__ ((OS_main));
int main() {

    /* MCP2515 reset */
    const CAN_CTRL	mcp2515;

    Serial  serial;
    serial.begin(9600);
    SPI::begin();
    mcp2515.reset();
    mcp2515.setBaud(100000);

    mcp2515.setOperationMode(mcp2515.NORMAL);
    mcp2515.RXB0.setMode(mcp2515.RXM_ANY);

    while(1) {
	CAN_CTRL::RXStatus status = mcp2515.readRXStatus();

	if (status.hasMessageInRXB0()) {
	    CAN_CTRL::Frame frame;
	    mcp2515.RXB0.readRX(&frame);
	    // mcp2515.RXB0.clear();
	    serial.send(&frame, sizeof(frame));
	}
    }

    return 0;
}

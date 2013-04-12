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
#include "avr-tino/MCP2515.h"
#include "avr-tino/SPI.h"
// XXX Must be done otherwise
#include "avr-tino/target/CANModule.h"

typedef SPIMaster SPI;
typedef MCP2515<SPI, MCP2515_CS>	CAN_CTRL;

#if 1
struct __attribute__ ((__packed__)) MyFrame {
    CAN::ID    id;
    uint8_t dlc;
    uint8_t c[8];
};
MyFrame	frame[] = {
    { MCP2515_SID(0b11110010001), 8, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H' }
};
#else
struct __attribute__ ((__packed__)) MyFrame {
    const char addr[4];
    uint8_t len;
    const char data[8];
} frame[] = { { "abc", 8, "efghijk" } };
#endif

int main() __attribute__ ((OS_main));
int main() {

    /* MCP2515 reset */
    const CAN_CTRL	mcp2515;

    SPI::begin();
    mcp2515.reset();
    mcp2515.setBaud(100000);
    mcp2515.setOperationMode(mcp2515.NORMAL);
//    volatile uint8_t r = mcp2515.read(mcp2515.CANCTRL);
//    mcp2515.write(mcp2515.CANCTRL, r);

    mcp2515.TXB0.loadTX(frame[0]);
    // mcp2515.TXB1.loadTX(frame[0]);

    while(1) {
	pinToLow(PIN_PD0);
	pinToLow(PIN_PD1);
	pinToLow(PIN_PD2);
	mcp2515.TXB0.doTransmitBuffer();
	for(uint8_t i = 0; i < 20; ++i) {
	    CAN_CTRL::TXStatus status = mcp2515.TXB0.status();

	    digitalWrite(PIN_PD0, status.isPending());
	    digitalWrite(PIN_PD1, status.isError());
	    digitalWrite(PIN_PD2, status.hasLostArbitration());
	    delay(50);
	}
//	mcp2515.TXB1.doTransmitBuffer();
    }

    return 0;
}

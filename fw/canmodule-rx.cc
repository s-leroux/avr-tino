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

int main() __attribute__ ((OS_main));
int main() {
    typedef SPIMaster SPI;

    /* MCP2515 reset */
    typedef MCP2515<SPI, MCP2515_CS>	CAN_CTRL;
    const MCP2515<SPI, MCP2515_CS>	mcp2515;

    SPI::begin();
    mcp2515.reset();
    mcp2515.setBaud(100000);

    static CAN_CTRL::Mask	mask = 
#if 1
	MCP2515_MASK(0b11000000000,0b110000000011111111);
#else
    {
    //	 NNNNNNNN   NNN                             SID
    //                    NN   NNNNNNNN   NNNNNNNN  EID
    //                  F                           EXIDE (filter only)
    //                 X X                          unused
      { b11000000, b00000011, b00000000, b11111111 }
    };
#endif
    static CAN_CTRL::Filter	filter = 
	MCP2515_EXT_FILTER(0b11000000000,0b110000000011111111);
//	b00110000, b00000000, b00000000, b00000000
    mcp2515.RXB0.setMask(mask);

    mcp2515.setOperationMode(mcp2515.NORMAL);

    mcp2515.RXB0.setMode(mcp2515.RXM_ANY);
    mcp2515.RXB1.setMode(mcp2515.RXM_ANY);

    CAN_CTRL::RXStatus	status;

    bool state = false;
    pinToOutput(PIN_PD1);

    while(1) {
	status = mcp2515.readRXStatus();
	if (status.hasMessageInRXB0()) {
	    uint8_t buffer[6];
	    mcp2515.RXB0.readData(sizeof(buffer), buffer);
	    state = !state;
	    digitalWrite(PIN_PD1, state);

	    mcp2515.RXB0.clear();
	}
    }

//    uint8_t data;
//    mcp2515.RXB0.readData(1,&data);
//    mcp2515.RXB0.readData(1,&data);
//    mcp2515.RXB1.readData(1,&data);
//    mcp2515.RXB1.readData(1,&data);
//    volatile uint8_t r = mcp2515.read(mcp2515.CANCTRL);
//    mcp2515.write(mcp2515.CANCTRL, r);

    // mcp2515.setTransmitBuffer(mcp2515.TXB0, 0x1234, 0x5678, 6, "Hello");
//    mcp2515.setTransmitBuffer(mcp2515.TXB0, 0x1234, 0x5678, 0, "");

    while(1) {
//	mcp2515.doTransmitBuffer(mcp2515.TXB0);
//	delay(5000);
    }

    return 0;
}

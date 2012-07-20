/*
    Test program to use with my MCP2515 based CANModule
*/


// XXX Must be done otherwise
#include "avr-tino.h"

#include "avr-tino/MCP2515.h"
#include "avr-tino/SPI.h"
#include "avr-tino/target/CANModule.h"

int main() {
    /* MCP2515 reset */
    const SPIMaster	spi;
    const MCP2515	mcp2515(MCP2515_CS, 0, MCP2515_RESET);

    mcp2515.reset();
    mcp2515.write(spi, 0x01, 0x02);
//    pinMode(MCP2515_RESET, OUTPUT);
//    digitalWrite(MCP2515_RESET, LOW);
//    digitalWrite(MCP2515_RESET, HIGH);

    

    return 0;
}

/*
    Test program to use with my MCP2515 based CANModule
*/


// XXX Must be done otherwise
#include "avr-tino.h"

#include "avr-tino/delay.h"
#include "avr-tino/MCP2515.h"
#include "avr-tino/SPI.h"
#include "avr-tino/target/CANModule.h"

int main() {
    /* MCP2515 reset */
    const SPIMaster	spi;
    const MCP2515	mcp2515(MCP2515_CS, 0, MCP2515_RESET);

    mcp2515.reset();
    mcp2515.setPrescaler(spi, 0);

    return 0;
}

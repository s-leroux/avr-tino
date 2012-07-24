/*
    Test program to use with my MCP2515 based CANModule
*/


// XXX Must be done otherwise
#include "avr-tino.h"

#include "avr-tino/delay.h"
#include "avr-tino/MCP2515.h"
#include "avr-tino/SPI.h"
#include "avr-tino/target/CANModule.h"

int main() __attribute__ ((OS_main));
int main() {
    typedef SPIMaster SPI;

    /* MCP2515 reset */
    const MCP2515<SPI, MCP2515_CS, MCP2515_RESET>	mcp2515;

    SPI::begin();
    mcp2515.reset();
    mcp2515.setPrescaler(0);
    volatile uint8_t r = mcp2515.read(12);

    return 0;
}

#include "avr-tino/target/CANModule.h"

void init_canmodule(void) __attribute__((naked)) __attribute__((section(".init5")));
void init_canmodule() {
    pinToOutput(MCP2515_RESET);
    pinToLow(MCP2515_RESET);
    pinToHigh(MCP2515_RESET);
}


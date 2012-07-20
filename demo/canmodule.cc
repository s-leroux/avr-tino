/*
    Test program to use with my MCP2515 based CANModule
*/


// XXX Must be done otherwise
#include "avr-tino.h"

#include "avr-tino/target/CANModule.h"

int main() {
    pinMode(MCP2515_RESET, OUTPUT);
    digitalWrite(MCP2515_RESET, LOW);
    digitalWrite(MCP2515_RESET, HIGH);

    return 0;
}

/*
    Test program to use with my MCP2515 based CANModule
*/
#include "avr-tino.h"

#include "avr-tino/serial.h"
#include "avr-tino/delay.h"

int main() {
    Serial::begin(9600);
    while(1) {
	Serial::send(0x65);
	delay(1);
    }

    return 0;
}

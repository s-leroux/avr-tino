/*
    Test program to use with my MCP2515 based CANModule
*/
#include <string.h>

#include "avr-tino.h"

#include "avr-tino/serial.h"
#include "avr-tino/delay.h"

int main() {
    char    message[] = "Hello X\n";

    Serial::begin(9600);
    uint8_t n = 0;
    while(1) {
	message[6] = (n++) % 10 + '0';
	for(uint8_t i = 0; i < strlen(message); ++i) {
	    Serial::send(message[i]);
	}
	delay(100);
    }

    return 0;
}

#include "avr-tino.h"
#include "avr-tino/delay.h"

/*
    This program demonstrate the use of
    'shiftOut' to send a byte
*/
int main() {
    const pin_t clk_pin	    = PIN_PD0;
    const pin_t data_pin    = PIN_PD1;
    pinMode(clk_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);


    while(1) {
	digitalWrite(clk_pin, LOW);
	shiftOut(data_pin, clk_pin, MSBFIRST, "Hello", 5);
	delay(50);

	digitalWrite(clk_pin, LOW);
	shiftOut(data_pin, clk_pin, LSBFIRST, "Hello", 5);
	delay(500);
    }
    return 0;
}

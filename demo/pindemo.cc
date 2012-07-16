#include "avr-tino.h"
#include "avr-tino/delay.h"

int main() {
    pinMode(PIN_PD0, OUTPUT);
    while(1) {
	digitalWrite(PIN_PD0, HIGH);
	delay(500);
	digitalWrite(PIN_PD0, LOW);
	delay(500);
    }
    return 0;
}

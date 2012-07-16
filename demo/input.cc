#include "avr-tino.h"
#include "avr-tino/delay.h"

int main() {
    pinMode(PIN_PD0, OUTPUT);
    pinMode(PIN_PD1, OUTPUT);
    pinMode(PIN_PD3, INPUT);

    digitalWrite(PIN_PD1, LOW);
    digitalWrite(PIN_PD3, HIGH);

    while(1) {
	digitalWrite(PIN_PD0, digitalRead(PIN_PD3));
	delay(100);
    }
    return 0;
}

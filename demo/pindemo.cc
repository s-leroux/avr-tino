#include "avr-tino.h"
#include "avr-tino/delay.h"

int main() {
    int x = pin_to_port(PIN_PB0);

    pinMode(PIN_PD0, OUTPUT);
    digitalWrite(PIN_PD0, HIGH);
    delay(500);
    digitalWrite(PIN_PD0, LOW);
}

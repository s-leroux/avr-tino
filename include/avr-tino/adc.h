/*
  Copyright (c) 2012 Sylvain Leroux <sylvain@chicoree.fr>
  
  This file is part of avr-tino -- http://github.com/s-leroux/avr-tino
  
  avr-tino is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  avr-tino is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with avr-tino.  If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined avr_tino_adc_h
#define avr_tino_adc_h

class ADConv {
    public:
    static void select(uint8_t mux_selection) {
        ADMUX = mux_selection;
    }

    static void start8BitsConversion() {
        PRR &= ~(1<<PRADC);
        ADMUX |= (1<<ADLAR);
        ADCSRA |= (1<<ADSC)|(1<<ADEN);
    }

    static void start10BitsConversion() {
        PRR &= ~(1<<PRADC);
        ADMUX &= ~(1<<ADLAR);
        ADCSRA |= (1<<ADSC)|(1<<ADEN);
    }

    static uint8_t conversionPending() {
        return ADCSRA & (1<<ADSC);
    }

    static void waitForConversion() {
        while(conversionPending()) {
            // do nothing
        }
    }

    static uint8_t read8Bits() {
        return ADCH;
    }

    static uint16_t read10Bits() {
        uint8_t lo = ADCL;
        return ADCH*256 + ADCL;
    }

    static uint16_t convert10Bits() {
        start10BitsConversion();
        waitForConversion();
        return read10Bits();
    }

    static uint8_t convert8Bits() {
        start8BitsConversion();
        waitForConversion();
        return read8Bits();
    }

    static uint16_t convert10Bits(uint8_t mux_selection) {
        select(mux_selection);
        return convert10Bits();
    }

    static uint8_t convert8Bits(uint8_t mux_selection) {
        select(mux_selection);
        return convert8Bits();
    }
};

#endif


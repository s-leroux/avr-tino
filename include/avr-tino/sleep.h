/*
  Copyright (c) 2012-2013 Sylvain Leroux <sylvain@chicoree.fr>
  
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
#if !defined AVR_TINO_SLEEP_H
#define AVR_TINO_SLEEP_H

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>


#if 1
static void wdt_enable_it(uint8_t value) {
    _WD_CONTROL_REG |= (1<<_WD_CHANGE_BIT)|(1<<WDE);
    _WD_CONTROL_REG = (1<<WDIE)|((value & 0x08) ? _WD_PS3_MASK : 0)|(value & 0x07);
}
#else

#define wdt_enable_it(value)   \
__asm__ __volatile__ (  \
    "in __tmp_reg__,__SREG__" "\n\t"    \
    "cli" "\n\t"    \
    "wdr" "\n\t"    \
    "sts %0,%1" "\n\t"  \
    "out __SREG__,__tmp_reg__" "\n\t"   \
    "sts %0,%2" "\n\t" \
    : /* no outputs */  \
    : "M" (_SFR_MEM_ADDR(_WD_CONTROL_REG)), \
    "r" (_BV(_WD_CHANGE_BIT) | _BV(WDE)), \
    "r" ((uint8_t) ((value & 0x08 ? _WD_PS3_MASK : 0x00) | \
        _BV(WDIE) | (value & 0x07)) ) \
    : "r0"  \
)

#endif

#define _power_down_opt(value, adcmode, bodmode) do { \
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); \
    cli(); \
    adcmode; \
    wdt_reset(); \
    wdt_enable_it(value); \
    sleep_enable(); \
    bodmode; \
    sei(); \
    sleep_cpu(); \
    sleep_disable(); \
    wdt_disable(); \
} while(0)

/**
    Turn off browout detector during deep sleep
*/
#define SL_BODOFF   sleep_bod_disable()
#define SL_BODKEEP 

/**
    Turn off ACD before sleep
*/
#define SL_ADCOFF   PRR |= (1<<PRADC); ADCSRA &= ~(1<<ADEN)
#define SL_ADCKEEP


#define _power_down(value) _power_down_opt(value, SL_ADCOFF, SL_BODOFF)

static void power_down_s(uint16_t duration) {
    while(duration) {
        // PORTB |= _BV(1);
#ifdef WDTO_8S
        if (duration >= 8) {
            _power_down(WDTO_8S);
            duration -= 8;
        }
        else
#endif
#ifdef WDTO_4S
        if (duration >= 4) {
            _power_down(WDTO_4S);
            duration -= 4;
        }
        else
#endif
#ifdef WDTO_2S
        if (duration >= 2) {
            _power_down(WDTO_2S);
            duration -= 2;
        }
        else
#endif
        {
            _power_down(WDTO_1S);
            duration -= 1;
        }
        // PORTB &= ~_BV(1);
    }
}

#endif


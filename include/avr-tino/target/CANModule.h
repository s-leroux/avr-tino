#if !defined AVR_TINO_TARGET_CANMODULE_H
#define AVR_TINO_TARGET_CANMODULE_H

#include "avr-tino.h"

#if !defined(F_CPU)
#  define F_CPU 1000000
#endif

static const pin_t MCP2515_RESET = PIN_PB3;
static const pin_t MCP2515_CS = PIN_PB4;

#endif


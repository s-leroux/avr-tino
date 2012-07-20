#include "avr-tino/MCP2515.h"

#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

MCP2515::MCP2515(pin_t cs, uint8_t addr, pin_t reset) 
    : _cs(cs), _addr(addr), _reset(reset) 
{
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
} 

void MCP2515::reset() const {
    pinMode(_reset, OUTPUT);
    digitalWrite(_reset, LOW);    
    digitalWrite(_reset, HIGH);    
}

void MCP2515::write(const SPIMaster& spi, uint8_t addr, uint8_t data) const {
    digitalWrite(_cs, LOW);
    spi.transfert(WRITE);        
    spi.transfert(addr);        
    spi.transfert(data);        
    digitalWrite(_cs, HIGH);
}


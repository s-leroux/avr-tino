#include "avr-tino/MCP2515.h"

#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

MCP2515::MCP2515(pin_t cs, uint8_t addr, pin_t reset) 
    : _cs(cs), _addr(addr), _reset(reset) 
{
    pinToOutput(_cs);
    pinToHigh(_cs);
} 

void MCP2515::reset() const {
    pinToOutput(_reset);
    pinToLow(_reset);
    pinToHigh(_reset);
}

void MCP2515::write(const SPIMaster& spi, uint8_t addr, uint8_t data) const {
    pinToLow(_cs);
    spi.transfert(WRITE);        
    spi.transfert(addr);        
    spi.transfert(data);        
    pinToHigh(_cs);
}


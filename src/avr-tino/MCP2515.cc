#include "avr-tino/MCP2515.h"

#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

template<class SPI>
MCP2515<SPI>::MCP2515(pin_t cs, uint8_t addr, pin_t reset) 
    : _cs(cs), _addr(addr), _reset(reset) 
{
    pinToOutput(_cs);
    pinToHigh(_cs);
} 

template<class SPI>
void MCP2515<SPI>::reset() const {
    pinToOutput(_reset);
    
    pinToLow(_reset);
    pinToHigh(_reset);
}

template<class SPI>
void MCP2515<SPI>::write(uint8_t addr, uint8_t data) const {
    GuardPinLow	    pin(_cs);

    SPI::transfert(WRITE);        
    SPI::transfert(addr);        
    SPI::transfert(data);        
}

template<class SPI>
void MCP2515<SPI>::bitModify(uint8_t addr, 
			uint8_t mask, uint8_t data) const {
    GuardPinLow	    pin(_cs);

    SPI::transfert(BIT_MODIFY);        
    SPI::transfert(addr);        
    SPI::transfert(mask);        
    SPI::transfert(data);        
}

template<class SPI>
void MCP2515<SPI>::setPrescaler(uint8_t prescaler) const {
    bitModify(CANCTRL, 0x03, prescaler);
}

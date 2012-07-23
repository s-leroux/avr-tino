#include "avr-tino/MCP2515.h"

#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

template<class SPI, pin_t cs, pin_t resetp>
MCP2515<SPI,cs,resetp>::MCP2515() 
{
    pinToOutput(cs);
    pinToHigh(cs);
} 

template<class SPI, pin_t cs, pin_t resetp>
void MCP2515<SPI,cs,resetp>::reset() const {
    pinToOutput(resetp);
    
    pinToLow(resetp);
    pinToHigh(resetp);
}

template<class SPI, pin_t cs, pin_t resetp>
void MCP2515<SPI,cs,resetp>::write(uint8_t addr, uint8_t data) const {
    GuardPinLow<cs>	guard;

    SPI::transfert(WRITE);        
    SPI::transfert(addr);        
    SPI::transfert(data);        
}

template<class SPI, pin_t cs, pin_t resetp>
void MCP2515<SPI,cs,resetp>::bitModify(uint8_t addr, 
			uint8_t mask, uint8_t data) const {
    GuardPinLow<cs>	guard;

    SPI::transfert(BIT_MODIFY);        
    SPI::transfert(addr);        
    SPI::transfert(mask);        
    SPI::transfert(data);        
}

template<class SPI, pin_t cs, pin_t resetp>
uint8_t MCP2515<SPI,cs,resetp>::read(uint8_t addr) const {
    GuardPinLow<cs>	guard;

    SPI::transfert(READ);
    SPI::transfert(addr);        
    return SPI::transfert(0);
}

template<class SPI, pin_t cs, pin_t resetp>
void MCP2515<SPI,cs,resetp>::setPrescaler(uint8_t prescaler) const {
    bitModify(CANCTRL, 0x03, prescaler);
}

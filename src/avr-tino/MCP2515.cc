#include "avr-tino/MCP2515.h"

#include "avr-tino/pin.h"
#include "avr-tino/SPI.h"

template<class SPI, pin_t cs, pin_t pin_reset>
MCP2515<SPI,cs,pin_reset>::MCP2515() 
{
    pinToOutput(cs);
    pinToHigh(cs);
} 

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::reset() const {
    pinToOutput(pin_reset);
    
    pinToLow(pin_reset);
    pinToHigh(pin_reset);
}

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::write(uint8_t addr, uint8_t data) const {
    GuardPinLow<cs>	guard;

    SPI::transfert(WRITE);        
    SPI::transfert(addr);        
    SPI::transfert(data);        
}

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::bitModify(uint8_t addr, 
			uint8_t mask, uint8_t data) const {
    GuardPinLow<cs>	guard;

    SPI::transfert(BIT_MODIFY);        
    SPI::transfert(addr);        
    SPI::transfert(mask);        
    SPI::transfert(data);        
}

template<class SPI, pin_t cs, pin_t pin_reset>
uint8_t MCP2515<SPI,cs,pin_reset>::read(uint8_t addr) const {
    GuardPinLow<cs>	guard;

    SPI::transfert(READ);
    SPI::transfert(addr);        
    return SPI::transfert(0);
}

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::setPrescaler(uint8_t prescaler) const {
    bitModify(CANCTRL, 0x03, prescaler);
}

template<class SPI, pin_t cs, pin_t pin_reset>
void MCP2515<SPI,cs,pin_reset>::setMode(mode_t mode) const {
    bitModify(CANCTRL,
		_BV(REQOP2) | _BV(REQOP1) | _BV(REQOP0),
		mode << REQOP0);
}


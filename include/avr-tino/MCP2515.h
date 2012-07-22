#if !defined AVR_TINO_MCP2515_H
#define AVR_TINO_MCP2515_H

class SPIMaster;

/*
 * Interface file for the MCP2515 Stand Alone CAN controller With SPI
 */
class MCP2515 {
    public:
    MCP2515(pin_t cs, uint8_t addr, pin_t reset);

    /* Reset by lowering the correspondig pin on the mcu */
    void reset() const;

    /* Generic commands */
    void write(const SPIMaster& spi, uint8_t addr, 
		uint8_t data) const;
    void bitModify(const SPIMaster& spi, uint8_t addr, 
		uint8_t masq, uint8_t data) const;

    /* Specific commands */
    void setPrescaler(const SPIMaster& spi, uint8_t prescaler) const;

    public:
    enum __attribute__ ((__packed__)) {
	RESET	    = 0xC0,
	WRITE	    = 0x02,
	BIT_MODIFY  = 0x05,
    };

    enum __attribute__ ((__packed__)) {
	CANCTRL = 0x0F, // CAN control register
    };

    enum __attribute__ ((__packed__)) canctrl_bits {
	CLKPRE0	= 0,
	CLKPRE1,
	CLKEN,
	OSM,
	ABAT,
	REQOP0,
	REQOP1,
	REQOP2,
    };

    private:
    const pin_t   _cs;
    const uint8_t _addr;
    const pin_t   _reset;
};

#endif


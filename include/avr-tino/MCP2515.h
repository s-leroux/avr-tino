#if !defined AVR_TINO_MCP2515_H
#define AVR_TINO_MCP2515_H

/*
 * Interface file for the MCP2515 Stand Alone CAN controller With SPI
 */
template<class SPI, pin_t cs, pin_t resetp> class MCP2515 {
    public:
    MCP2515();

    /* Reset by lowering the correspondig pin on the mcu */
    void reset() const;

    /* Generic commands */
    void write(uint8_t addr, 
		uint8_t data) const;
    void bitModify(uint8_t addr, 
		uint8_t masq, uint8_t data) const;
    uint8_t read(uint8_t addr) const;

    /* Specific commands */
    void setPrescaler(uint8_t prescaler) const;

    public:
    enum __attribute__ ((__packed__)) {
	RESET	    = 0xC0,
	WRITE	    = 0x02,
	BIT_MODIFY  = 0x05,
	READ	    = 0x03,
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
};

#endif


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

    void write(const SPIMaster& spi, uint8_t addr, uint8_t data) const;

    public:
    enum __attribute__ ((__packed__)) {
	RESET	= 0xC0,
	WRITE	= 0x02,
    };

    private:
    const pin_t   _cs;
    const uint8_t _addr;
    const pin_t   _reset;
};

#endif


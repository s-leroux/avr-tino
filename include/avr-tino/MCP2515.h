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
		uint8_t len,
		const uint8_t *data) const;
    void write(uint8_t addr, 
		uint8_t data) const;
    void bitModify(uint8_t addr, 
		uint8_t masq, uint8_t data) const;
    uint8_t read(uint8_t addr) const;

    /* Specific commands */
    void setPrescaler(uint8_t prescaler) const;

    enum __attribute__ ((__packed__)) mode_t {
	NORMAL	        = 0,  /* Normal mode         000 */
	SLEEP	        = 1,  /* Sleep mode          001 */
        LOOPBACK        = 2,  /* Loopback mode       010 */
        LISTEN          = 3,  /* Listen only mode    011 */
        CONFIGURATION	= 4,  /* Configuration mode  100 */
    };
    void setMode(mode_t mode) const;

    public:
    enum __attribute__ ((__packed__)) {
	RESET	    = 0xC0, /* Reset                    - 1100 0000 */
	WRITE	    = 0x02, /* Write Date               - 0000 0010 */
	BIT_MODIFY  = 0x05, /* Bit Modify               - 0000 0101 */
	READ	    = 0x03, /* Read data                - 0000 0011 */
	RTS	    = 0x80, /* Request To Send          - 1000 0nnn */
    };

    enum __attribute__ ((__packed__)) regs {
	CANCTRL	    = 0x0F, /* CAN control register     - ?Fh */
	CANSTAT	    = 0x0E, /* CAN status register      - ?Eh */
	TEC	    = 0x1C, /* Transmit error counter   - 1Ch */
	REC	    = 0x1D, /* reveiver error counter   - 1Dh */

	/* Transmit buffer 0 */
	TXB0CTRL    = 0x30, /* Transmit buffer control */
	TXB0SIDH    = 0x31, /* Transmit buffer std id high */
	TXB0SIDL    = 0x32, /* Transmit buffer std id low */
	TXB0EID8    = 0x33, /* Transmit buffer ext id high*/
	TXB0EID0    = 0x34, /* Transmit buffer ext id low */
	TXB0DLC     = 0x35, /* Transmit buffer data length */
	TXB0D0      = 0x36, /* Transmit buffer data byte 1 */
	TXB0D1      = 0x37, /* Transmit buffer data byte 1 */
	TXB0D2      = 0x38, /* Transmit buffer data byte 2 */
	TXB0D3      = 0x39, /* Transmit buffer data byte 3 */
	TXB0D4      = 0x3A, /* Transmit buffer data byte 4 */
	TXB0D5      = 0x3B, /* Transmit buffer data byte 5 */
	TXB0D6      = 0x3C, /* Transmit buffer data byte 6 */
	TXB0D7      = 0x3D, /* Transmit buffer data byte 7 */
    };

    enum __attribute__ ((__packed__)) canctrl_bits {
	CLKPRE0	    = 0,
	CLKPRE1,
	CLKEN,
	OSM,
	ABAT,
	REQOP0,
	REQOP1,
	REQOP2,
    };

    enum __attribute__ ((__packed__)) txctrl_bits {
	TXP0	    = 0,    /* Transmission priority */
	TXP1,		    /* Transmission priority */
	TXREQ	    = 3,    /* Transmission request */
	TXERR,              /* Transmission error */
        MLOA,               /* Message lost arbitration */
        ABTF,               /* Message aborted flag */
    };

    enum __attribute__ ((__packed__)) canstat_bits {
	ICOD0	    = 1,
	ICOD1,
	ICOD2,
	OPMOD0	    = 5,
	OPMOD1,
	OPMOD2,
    };
};

#endif


/*
  Copyright (c) 2012 Sylvain Leroux <sylvain@chicoree.fr>
  
  This file is part of avr-tino -- http://github.com/s-leroux/avr-tino
  
  avr-tino is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  avr-tino is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with avr-tino.  If not, see <http://www.gnu.org/licenses/>.
*/
#if !defined AVR_TINO_MCP2515_H
#define AVR_TINO_MCP2515_H

/*
 * Interface file for the MCP2515 Stand Alone CAN controller With SPI
 */
template<class SPI, pin_t cs> class MCP2515 {
    public:
    typedef MCP2515<SPI,cs>	DEVICE;

    MCP2515();

    /**
	Issue a RESET command throught SPI

	According to the datasheet, the MCP2515 is set
	to configuration mode after a reset.

	You must use setOperationMode to change that.
    */
    void reset() const;

    enum __attribute__ ((__packed__)) reqop_t {
	NORMAL		= b00000000,
	SLEEP		= b00100000,
	LOOPBACK	= b01000000,
	LISTEN_ONLY	= b01100000,
	CONFIGURATION	= b10000000,
    };
    /**
	Change the operation mode of the device
    */
    void setOperationMode(reqop_t mode) const;

    enum __attribute__ ((__packed__)) regs {
	CANCTRL	    = 0x0F, /* CAN control register     - ?Fh */
	CANSTAT	    = 0x0E, /* CAN status register      - ?Eh */
	TEC	    = 0x1C, /* Transmit error counter   - 1Ch */
	REC	    = 0x1D, /* reveiver error counter   - 1Dh */

	/* Transmit buffer 0 */
//	TXB0CTRL    = 0x30, /* Transmit buffer control */
	TXB0SIDH    = 0x31, /* Transmit buffer std id high */
	TXB0SIDL    = 0x32, /* Transmit buffer std id low */
	TXB0EID8    = 0x33, /* Transmit buffer ext id high*/
	TXB0EID0    = 0x34, /* Transmit buffer ext id low */
	TXB0DLC     = 0x35, /* Transmit buffer data length */
//	TXB0D0      = 0x36, /* Transmit buffer data byte 1 */
	TXB0D1      = 0x37, /* Transmit buffer data byte 1 */
	TXB0D2      = 0x38, /* Transmit buffer data byte 2 */
	TXB0D3      = 0x39, /* Transmit buffer data byte 3 */
	TXB0D4      = 0x3A, /* Transmit buffer data byte 4 */
	TXB0D5      = 0x3B, /* Transmit buffer data byte 5 */
	TXB0D6      = 0x3C, /* Transmit buffer data byte 6 */
	TXB0D7      = 0x3D, /* Transmit buffer data byte 7 */

	/* Receive buffer 0 */
//	RXB0CTRL    = 0x60, /* Receive buffer control */
    };

    /* Common interface */
    /**
	Read a register value
    */
    static uint8_t read(regs r);

    /**
	Sequencial read registers
    */
    static void read(regs r, uint8_t len, void * buffer);

    /**
	Write a value into a register
    */
    void write(regs r, uint8_t data) const;

    void write(regs r, 
		uint8_t len,
		const void *data) const;

    /**
	Set and/or clear some bits of a register
    */
    static void update(regs r, uint8_t masq, uint8_t value);

    /**
	Clear some bits of a register
    */
    void clear(regs r, uint8_t mask) const {
	update(r, mask, 0x00);
    }


    /**
	Set some bits of a register
    */
    void set(regs r, uint8_t mask) const {
	update(r, mask, mask);
    }

    /* Specific commands */
    void setPrescaler(uint8_t prescaler) const;

    template<class INT>
    inline void setBaud(INT baud) const {
	setPrescaler(F_CPU/baud/2-1);
    }

    /**
	Prescaler values for common baud based of F_CPU.
    */
    static const uint8_t BAUD9600 = F_CPU/9600/2-1;

    /**
	Transmit buffer selector.

	High order 4-bits maps to the correspondig TX buffer
	base addr. Low order 4-bits maps to the corresponding
	bit for 'Request To Send' command'.

	Ugly but efficient (mostly)...
    */
    enum __attribute__ ((__packed__)) txb_t {
                                /*                    TXB  RTS  */
                                /*                    ---- ---- */
	//TXB0	= 0x31,		/* Transmit buffer 0  0011 0001 */
	//TXB1	= 0x42,		/* Transmit buffer 1  0100 0010 */
	TXB2	= 0x54,		/* Transmit buffer 2  0101 0100 */
	TXB0_BASE = 0x30,
	TXB1_BASE = 0x40,
    };
    static void setTransmitBuffer(txb_t tx_base,
			    uint16_t sid,
			    uint16_t eid,
			    uint8_t len,
			    const void *data);

    void doTransmitBuffer(txb_t buffer_set) const;

    /* ------------------------------------------------ */
    /* Transmit buffer			                */
    /* ------------------------------------------------ */
    template<uint8_t REG>
    struct TXBnCTRL {
	static const regs	reg = (regs)REG;

	enum __attribute__((__packed__)) mask {
	    ABTF	= b01000000,	/* Message aborted */
	    MLOA	= b00100000,	/* Message lost arbitration */
	    TXERR	= b00010000,	/* Transmission error detect */
	    TXREQ	= b00001000,	/* Message transmit request */
	    /* bit 2 is unimplemented */
	    TXP		= b00000011,	/* Transmit buffer priority */
	};
    };

    typedef TXBnCTRL<0x30>  TXB0CTRL;
    typedef TXBnCTRL<0x40>  TXB1CTRL;

    struct TXB0D0 {
    };

    struct TXB1D0 {
    };

    static struct {
	typedef TXB0CTRL    TXBCTRL;
	typedef TXB0D0	    TXBDATA;

	static void setTransmitBuffer(
			    uint16_t sid,
                            uint16_t eid,
                            uint8_t len,
                            const void *data) {
	    DEVICE::setTransmitBuffer((txb_t)TXBCTRL::reg,
			    sid,
			    eid,
			    len,
			    data);
	}

    } TXB0;

    static struct {
	typedef TXB1CTRL    TXBCTRL;
	typedef TXB1D0	    TXBDATA;

	static void setTransmitBuffer(
			    uint16_t sid,
                            uint16_t eid,
                            uint8_t len,
                            const void *data) {
	    DEVICE::setTransmitBuffer((txb_t)TXBCTRL::reg,
			    sid,
			    eid,
			    len,
			    data);
	}
    } TXB1;



    /* ------------------------------------------------ */
    /* Receive buffer			                */
    /* ------------------------------------------------ */
    struct RXB0CTRL {
	static const regs	reg = (regs)0x60;

	enum __attribute__((__packed__)) mask {
	    RXM		= b01100000,
	    RXRTR	= b00001000,
	    BUKT	= b00000100,
	    BUKT1	= b00000010,
	    FILHIT	= b00000001,
	};
    };

    struct RXB1CTRL {
	static const regs	reg = (regs)0x70;

	enum __attribute__((__packed__)) mask {
	    RXM		= b01100000,
	    RXRTR	= b00001000,
	    FILHIT	= b00000111,
	};
    };

    enum __attribute__ ((__packed__)) receive_mode_t {
	RXM_ANY	    = b01100000, /* Receive any message */
	RXM_EID	    = b01000000, /* Receive valid message based on EID */
	RXM_SID	    = b00100000, /* Receive valid message based on SID */
	RXM_BOTH    = b00000000, /* Receive valid message based on SID or EID*/
    };

    struct RXB0D0 {
	static const regs       reg = (regs)0x66;
    };

    struct RXB1D0 {
	static const regs       reg = (regs)0x76;
    };

    static struct {
	typedef RXB0CTRL    RXBCTRL;
	typedef RXB0D0	    RXBDATA;

	static void setMode(receive_mode_t mode) {
	    update(RXBCTRL::reg, RXBCTRL::RXM, mode);
	}

	static void readData(uint8_t len, void * buffer) {
	    read(RXBDATA::reg, len, buffer);
	}
    } RXB0;

    static struct {
	typedef RXB1CTRL    RXBCTRL;
	typedef RXB1D0	    RXBDATA;

	static void setMode(receive_mode_t mode) {
	    update(RXBCTRL::reg, RXBCTRL::RXM, mode);
	}

	static void readData(uint8_t len, void * buffer) {
	    read(RXBDATA::reg, len, buffer);
	}
    } RXB1;

    /* RX status */
    struct RXStatus {
	uint8_t	    status;

	inline RXStatus() {}
	inline RXStatus(uint8_t s) : status(s) {}

	enum {
	    MESSAGE_IN_RXB0 = b10000000,
	    MESSAGE_IN_RXB1 = b01000000,
	    /* bit 5 is unused */
	    EXTENDED_FRAME  = b00010000,
	    REMOTE_FRAME    = b00001000,
	    FILTER_MATCH    = b00000111, /* Filter match mask */
	};

	inline bool hasMessageInRXB0() const {
	    return status & MESSAGE_IN_RXB0;
	}

	inline bool hasMessageInRXB1() const {
	    return status & MESSAGE_IN_RXB1;
	}
    };

    static RXStatus readRXStatus();

    /* ------------------------------------------------ */

    public:
    enum __attribute__ ((__packed__)) instr {
	RESET	    = 0xC0, /* Reset                    - 1100 0000 */
	WRITE	    = 0x02, /* Write Date               - 0000 0010 */
	BIT_MODIFY  = 0x05, /* Bit Modify               - 0000 0101 */
	READ	    = 0x03, /* Read data                - 0000 0011 */
	RTS	    = 0x80, /* Request To Send          - 1000 0nnn */
	RX_STATUS   = b10110000, /* Quick polling of received message */
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

    private:
    class Command : GuardPinLow<cs> {
	public:
	Command(instr c) {
	    SPI::transfert(c);
	}

	void write(uint8_t byte) const {
	    SPI::transfert(byte);
	}

	uint8_t read() const {
	    return SPI::transfert(0);
	}

	void write(uint8_t n, const void *data) const {
	    for(int i = 0; i < n; ++i) { 
		SPI::transfert(((uint8_t*)data)[i]); 
	    }
	}

	void read(uint8_t n, const void *data) const {
	    for(int i = 0; i < n; ++i) { 
		((uint8_t*)data)[i] = SPI::transfert(0); 
	    }
	}
    };
};

#endif


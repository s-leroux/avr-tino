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

    /* ------------------------------------------------ */
    /* Register description		                */
    /* ------------------------------------------------ */
    enum __attribute__ ((__packed__)) REG {};

    static const REG	CANCTRL = (REG)0x0F; /* CAN Control register ?Fh */
    struct CANCTRL {
	enum __attribute__ ((__packed__)) mask {
	    REQOP	= b11100000,
	    ABAT	= b00010000,
	    OSM		= b00001000,
	    CLKEN	= b00000100,
	    CLKPRE	= b00000011,
	};
    };

    static const REG    CANINTF = (REG)0x2C; /* Interrupt flag 2Ch */
    struct CANINTF {
	enum __attribute__ ((__packed__)) mask {
	    MERRF	= b10000000,
	    WAKIF	= b01000000,
	    ERRIF	= b00100000,
	    TX2IF	= b00010000,
	    TX1IF	= b00001000,
	    TX0IF	= b00000100,
	    RX1IF	= b00000010,
	    RX0IF	= b00000001,
	};
    };

    struct TXBnCTRL {
	enum __attribute__((__packed__)) mask {
	    ABTF	= b01000000,	/* Message aborted */
	    MLOA	= b00100000,	/* Message lost arbitration */
	    TXERR	= b00010000,	/* Transmission error detect */
	    TXREQ	= b00001000,	/* Message transmit request */
	    /* bit 2 is unimplemented */
	    TXP		= b00000011,	/* Transmit buffer priority */
	};
    };

    static const REG	TXB0CTRL    = (REG)0x30;
    struct TXB0CTRL : public TXBnCTRL {};

    static const REG	TXB1CTRL    = (REG)0x40;
    struct TXB1CTRL : public TXBnCTRL {};

    static const REG	TXB2CTRL    = (REG)0x50;
    struct TXB2CTRL : public TXBnCTRL {};


    static const REG	RXB0CTRL    = (REG)0x60;
    struct RXB0CTRL {
	enum __attribute__((__packed__)) mask {
	    RXM		= b01100000,
	    RXRTR	= b00001000,
	    BUKT	= b00000100,
	    BUKT1	= b00000010,
	    FILHIT	= b00000001,
	};
    };

    static const REG	RXB1CTRL    = (REG)0x70;
    struct RXB1CTRL {
	enum __attribute__((__packed__)) mask {
	    RXM		= b01100000,
	    RXRTR	= b00001000,
	    FILHIT	= b00000111,
	};
    };

    static const REG	RXF0SIDH    = (REG)0x00; 
    static const REG	RXF1SIDH    = (REG)0x04;
    static const REG	RXF2SIDH    = (REG)0x08;
    static const REG	RXF3SIDH    = (REG)0x10;
    static const REG	RXF4SIDH    = (REG)0x14;
    static const REG	RXF5SIDH    = (REG)0x18;

    static const REG	RXM0SIDH    = (REG)0x20;
    static const REG	RXM0SIDL    = (REG)0x21;
    static const REG	RXM0EID8    = (REG)0x22;
    static const REG	RXM0EID0    = (REG)0x23;

    static const REG	RXM1SIDH    = (REG)0x24;
    static const REG	RXM1SIDL    = (REG)0x25;
    static const REG	RXM1EID8    = (REG)0x26;
    static const REG	RXM1EID0    = (REG)0x27;
    /* ------------------------------------------------ */



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
	CANSTAT	    = 0x0E, /* CAN status register      - ?Eh */
	TEC	    = 0x1C, /* Transmit error counter   - 1Ch */
	REC	    = 0x1D, /* reveiver error counter   - 1Dh */
    };

    /* ------------------------------------------------ */
    /* Common interface			                */
    /* ------------------------------------------------ */
    /**
	Read a register value
    */
    static uint8_t read(REG r);

    /**
	Sequencial read registers
    */
    static void read(REG r, uint8_t len, void * buffer);

    /**
	Write a value into a register
    */
    void write(REG r, uint8_t data) const;

    void write(REG r, 
		uint8_t len,
		const void *data) const;

    /**
	Set and/or clear some bits of a register
    */
    static void update(REG r, uint8_t masq, uint8_t value);

    /**
	Clear some bits of a register
    */
    static void clear(REG r, uint8_t mask) {
	update(r, mask, 0x00);
    }


    /**
	Set some bits of a register
    */
    static void set(REG r, uint8_t mask) {
	update(r, mask, mask);
    }
    /* ------------------------------------------------ */



    /* Specific commands */
    void setPrescaler(uint8_t prescaler) const;

    template<class INT>
    inline void setBaud(INT baud) const {
	setPrescaler(F_CPU/baud/2-1);
    }

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
	TXB0_ID	= 0x31,		/* Transmit buffer 0  0011 0001 */
	TXB1_ID	= 0x42,		/* Transmit buffer 1  0100 0010 */
	TXB2_ID	= 0x54,		/* Transmit buffer 2  0101 0100 */
    };

    /* ------------------------------------------------ */
    /* Frame message			                */
    /* ------------------------------------------------ */
    struct __attribute__ ((__packed__)) Frame {
	uint8_t	    sidh;
	uint8_t	    sidl;
	uint8_t	    eid8;
	uint8_t	    eid0;
	uint8_t	    dlc;
	uint8_t	    data[8];
    };
    /* ------------------------------------------------ */

    static void loadTX(uint8_t load_tx_location,
			    uint16_t sid,
			    uint16_t eid,
			    uint8_t len,
			    const void *data);
    static void loadTX(uint8_t load_tx_location,
			const Frame& frame, uint8_t len = sizeof(Frame));

    enum __attribute__ ((__packed__)) txb_rts_t {
	TXB0_RTS = _BV(0),
	TXB1_RTS = _BV(1),
	TXB2_RTS = _BV(2),
	/* plus combination of the above */
    };

    friend txb_rts_t operator|(txb_rts_t a, txb_rts_t b) {
	return (txb_rts_t)(a|b);
    }

    static void doTransmitBuffer(txb_rts_t buffer_set);
    static void doTransmitBuffer(txb_t id) {
	doTransmitBuffer((txb_rts_t)(id & 0x07));
    }

    /* ------------------------------------------------ */
    /* Transmit buffer			                */
    /* ------------------------------------------------ */
    struct TXStatus {
	public:
	TXStatus(uint8_t status) : _status(status) {}

	inline bool hasLostArbitration(void) const {
	    return _status & TXBnCTRL::MLOA;
	}

	inline bool isError(void) const {
	    return _status & TXBnCTRL::TXERR;
	}

	inline bool isPending(void) const {
	    return _status & TXBnCTRL::TXREQ;
	}
	
	private:
	uint8_t	_status;
    };
    
    template<typename TXTrait>
    struct TXBn {
	static const uint8_t	LOAD_TX_SIDH_START  = TXTrait::LOAD_TX_SIDH_START ;
	static const uint8_t	LOAD_TX_DATA_START  = TXTrait::LOAD_TX_DATA_START;

	static const uint8_t	RTS_MASK	    = TXTrait::RTS_MASK;

	enum __attribute__ ((__packed__)) {
	    TXBCTRL		= TXTrait::TXBCTRL,
	    TXBSIDH,	/* Transmit buffer std id high */
	    TXBSIDL,	/* Transmit buffer std id low */
	    TXBEID8,	/* Transmit buffer ext id high*/
	    TXBEID0,	/* Transmit buffer ext id low */
	    TXBDLC,	/* Transmit buffer data length */
	    TXBD0,	/* Transmit buffer data byte 1 */
	    TXBD1,	/* Transmit buffer data byte 1 */
	    TXBD2,	/* Transmit buffer data byte 2 */
	    TXBD3,	/* Transmit buffer data byte 3 */
	    TXBD4,	/* Transmit buffer data byte 4 */
	    TXBD5,	/* Transmit buffer data byte 5 */
	    TXBD6,	/* Transmit buffer data byte 6 */
	    TXBD7,	/* Transmit buffer data byte 7 */
	};

	static void loadTX(
			    uint16_t sid,
                            uint16_t eid,
                            uint8_t len,
                            const void *data) {
	    DEVICE::loadTX(LOAD_TX_SIDH_START,
			    sid,
			    eid,
			    len,
			    data);
	}

	static void loadTX(const Frame& frame, uint8_t len = sizeof(Frame)) {
	    DEVICE::loadTX(LOAD_TX_SIDH_START, frame, len);
	}

	static void doTransmitBuffer(void) {
	    DEVICE::doTransmitBuffer((txb_rts_t)RTS_MASK);
	}

	static inline TXStatus status() {
	    return DEVICE::read((typename DEVICE::REG)TXBCTRL);
	}
    };

    struct TXB0Trait	{
	static const uint8_t	LOAD_TX_SIDH_START  = b00000000;
	static const uint8_t	LOAD_TX_DATA_START  = b00000001;

	static const uint8_t	RTS_MASK	    = b00000001;

	static const uint8_t	TXBCTRL		    = 0x30;
    };

    struct TXB1Trait	{
	static const uint8_t	LOAD_TX_SIDH_START  = b00000010;
	static const uint8_t	LOAD_TX_DATA_START  = b00000011;

	static const uint8_t	RTS_MASK	    = b00000010;

	static const uint8_t	TXBCTRL		    = 0x40;
    };

    struct TXB2Trait	{
	static const uint8_t	LOAD_TX_SIDH_START  = b00000100;
	static const uint8_t	LOAD_TX_DATA_START  = b00000101;

	static const uint8_t	RTS_MASK	    = b00000100;

	static const uint8_t	TXBCTRL		    = 0x50;
    };

    TXBn<TXB0Trait>	TXB0;
    TXBn<TXB1Trait>	TXB1;
    TXBn<TXB2Trait>	TXB2;

    /* ------------------------------------------------ */
    /* Acceptance mask			                */
    /* ------------------------------------------------ */
    struct __attribute__ ((__packed__)) Mask {
	uint8_t	    sidh;
	uint8_t	    sidl;
	uint8_t	    eid8;
	uint8_t	    eid0;

	enum __attribute__ ((__packed__)) base_reg_t {
	    RXM0    = RXM0SIDH,
	    RXM1    = RXM1SIDH,
	};
    };

    static void setMask(typename Mask::base_reg_t reg, const Mask *mask) {
	write((REG)reg, sizeof(Mask), mask);
    }

    /* ------------------------------------------------ */
    /* Acceptance filter		                */
    /* ------------------------------------------------ */
    struct __attribute__ ((__packed__)) Filter {
	uint8_t	    sidh;
	uint8_t	    sidl;
	uint8_t	    eid8;
	uint8_t	    eid0;

	enum __attribute__ ((__packed__)) base_reg_t {
	    RXF0    = RXF0SIDH,
	    RXF1    = RXF1SIDH,
	    RXF2    = RXF2SIDH,
	    RXF3    = RXF3SIDH,
	    RXF4    = RXF4SIDH,
	    RXF5    = RXF5SIDH,
	};
    };

    static void setFilter(typename Mask::base_reg_t reg, const Mask *mask) {
	write((REG)reg, sizeof(Mask), mask);
    }

    /* ------------------------------------------------ */

    /* ------------------------------------------------ */
    /* Receive buffer			                */
    /* ------------------------------------------------ */
    enum __attribute__ ((__packed__)) receive_mode_t {
	RXM_ANY	    = b01100000, /* Receive any message */
	RXM_EID	    = b01000000, /* Receive valid message based on EID */
	RXM_SID	    = b00100000, /* Receive valid message based on SID */
	RXM_BOTH    = b00000000, /* Receive valid message based on SID or EID*/
    };

    struct RXB0Trait {
	static const REG	    RXBCTRL = RXB0CTRL;
	struct RXBCTRL : public RXB0CTRL {};

	static const uint8_t	    RXIF    = CANINTF::RX0IF;
	static const typename Mask::base_reg_t RXM   = Mask::RXM0;

	static const uint8_t	READ_RX_SIDH = b00000000;
	static const uint8_t	READ_RX_DATA = b00000010;
    };

    struct RXB1Trait {
	static const REG	    RXBCTRL = RXB1CTRL;
	struct RXBCTRL : public RXB1CTRL {};

	static const uint8_t	    RXIF    = CANINTF::RX1IF;
	static const typename Mask::base_reg_t RXM   = Mask::RXM1;

	static const uint8_t	READ_RX_SIDH = b00000100;
	static const uint8_t	READ_RX_DATA = b00000110;
    };

    template<class RXTrait>
    struct RXBn {
	enum __attribute__ ((__packed__)) {
	    RXBCTRL	= RXTrait::RXBCTRL,
	    RXBSIDH,
	    RXBSIDL,
	    RXBEID8,
	    RXBEID0,
	    RXBDLC,
	    RXBD0,
	    RXBD1,
	    RXBD2,
	    RXBD3,
	    RXBD4,
	    RXBD5,
	    RXBD6,
	    RXBD7,
	};

	static const uint8_t	RXIF	    = RXTrait::RXIF;
	struct RXBCTRL : public RXTrait::RXBCTRL {};

	static const typename Mask::base_reg_t RXM   = RXTrait::RXM;

	static const uint8_t	READ_RX_SIDH = RXTrait::READ_RX_SIDH;
	static const uint8_t	READ_RX_DATA = RXTrait::READ_RX_DATA;

	static void setMode(receive_mode_t mode) {
	    update((REG)RXBCTRL, RXBCTRL::RXM, mode);
	}

	/**
	    Read RX data and clears corresponding RX flag
	*/
	static void readData(uint8_t len, void * buffer) {
	    read((REG)(READ_RX | READ_RX_DATA), len, buffer);
	}

	/**
	    Read RX buffer and clears corresponding RX flag
	*/
	static void readRX(Frame * dest, const uint8_t len = sizeof(Frame)) {
	    read((REG)(READ_RX | READ_RX_SIDH), len, dest);
	}

	static void clear() {
	    DEVICE::clear((REG)CANINTF, RXIF);
	}

	static void setMask(const Mask *mask) {
	    DEVICE::setMask(RXM, mask);
	}
    };

    static RXBn<RXB0Trait>	RXB0;
    static RXBn<RXB1Trait>	RXB1;

    /* RX status */
    struct RXStatus {
	uint8_t	    status;

	inline RXStatus() {}
	inline RXStatus(uint8_t s) : status(s) {}

	enum {
	    MESSAGE_IN_RXB1 = b10000000,
	    MESSAGE_IN_RXB0 = b01000000,
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
	LOAD_TX	    = 0x40, /* Load TX buffer		- 0100 0abc */
	RTS	    = 0x80, /* Request To Send          - 1000 0nnn */
	READ_RX	    = 0x90, /* Read RX buffer		- 1001 0nm1 */
	RX_STATUS   = 0xB0, /* RX status                - 1011 0000 */
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


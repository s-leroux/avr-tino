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

#if !defined avr_tino_rf12_h
#define avr_tino_rf12_h

#include "avr-tino.h"
#include "avr-tino/delay.h"
#include "avr-tino/serial.h"
#include "avr-tino/printer.h"

template<class SPI, class SS, class IRQ>
class RF12Module {
    public:
    static void begin() {
        // assume begin on SPI interface already called
        static const uint16_t band = 2;

        transfert(0x0000);
        transfert(0x8205); // Disable output clock
        transfert(RF_TX_WRITE);
        while(IRQ::isClear()) {
            transfert(0x0000);
        }

        transfert(0x80C7 | (band << 4)); // EL (ena TX), EF (ena RX FIFO), 12.0pF 
        transfert(0xA640); // 868MHz 
        //transfert(0xC606); // approx 49.2 Kbps, i.e. 10000/29/(1+6) Kbps
        transfert(DATA_RATE_49200); // approx 49.2 Kbps, i.e. 10000/29/(1+6) Kbps
        transfert(0x94A2); // VDI,FAST,134kHz,0dBm,-91dBm 
        transfert(0xC2AC); // AL,!ml,DIG,DQD4
#if 1
        transfert(0xCA83); // FIFO8,2-SYNC,!ff,DR 
        transfert(0xCEAA); // SYNC=...;
#else
        transfert(0xCA8B); // FIFO8,1-SYNC,!ff,DR 
        transfert(0xCE2D); // SYNC=2D;
#endif

        transfert(0xC483); // @PWR,NO RSTRIC,!st,!fi,OE,EN 
        transfert(0x9850); // !mp,90kHz,MAX OUT 
        transfert(0xCC77); // OB1，OB0, LPX,！ddy，DDIT，BW0 

        transfert(0xE000); // NOT USE 
        transfert(0xC800); // NOT USE 
        transfert(0xC049); // 1.66MHz,3.1V 

//        transfert(RF_IDLE);
    }

    // RF12 control words are 16-bits width
    static uint16_t transfert(uint16_t word) {
        select();

        uint16_t result = SPI::transfert(word >> 8) << 8;
        result |= SPI::transfert(word);

        deselect();

        return result;
    }

    static void send(uint8_t len, const void * data) {
    cli();    
        memcpy((void*)buff, data, len);
        buff_pos = 0;
        buff_len = len;
        mode = TX_MODE;
        
        transfert(RF_XMITTER_ON);
    sei();
    }

    static void receive() {
    cli();    
        buff_pos = 0;
        buff_len = 0;
        mode = RX_MODE;
        transfert(RF_IDLE);
        transfert(RF_RECEIVER_ON);
    sei();
    }

    static uint16_t interrupt() {
        //OUT::print("inter in\r\n");
        uint16_t status = transfert(0x0000);
        //OUT::print("status read\r\n");

//        uint8_t byte;
//        if ((status & 0x8000)) {
//                        byte = transfert(RF_RX_READ);
//        }

        switch(mode) {
            case RX_MODE: {
                    uint8_t byte = '-';
                    if ((status & 0x8000)) {
                        byte = transfert(RF_RX_READ);
                        //byte = (uint8_t)(transfert(RF_RX_READ) & 0xFF);
                        #if 1
                        // assume available data (should check status?)
                        if (buff_len < RX_BUFF_SIZE) {
                            buff[buff_len++] = byte;
                        }
                        else {
                            // Buffer full. Do not collect more bytes
                            mode = IDLE_MODE;
                            transfert(RF_IDLE);
                        }
                        #endif
                    }
                }
                break;
            case TX_MODE:
                uint8_t byte;
                if (buff_pos < buff_len) {
                    byte = buff[buff_pos++];
                }
                else {
                    byte = 0xAA; // send dummy byte to clear nIRQ
                    mode = IDLE_MODE;
                    transfert(RF_IDLE);
                }
                transfert(RF_TX_WRITE + byte);
                //transfert(0xB848);
                break;
            default:
                mode = IDLE_MODE;
                transfert(RF_IDLE);
        }
        return status;
    }

    enum { IDLE_MODE, RX_MODE, TX_MODE };
    static volatile uint8_t mode;

    static const uint8_t    RX_BUFF_SIZE = 31;
    static volatile uint8_t buff_pos;
    static volatile uint8_t buff_len;
    static volatile uint8_t          buff[RX_BUFF_SIZE];

    /* Commands */
    static const uint16_t   RF_XMITTER_ON   = 0x823D;
    static const uint16_t   RF_RECEIVER_ON  = 0x82DD;
    static const uint16_t   RF_IDLE         = 0x820D;
    static const uint16_t   RF_RX_READ      = 0xB000;
    static const uint16_t   RF_TX_WRITE     = 0xB800;

    static const uint16_t   DATA_RATE       = 0xC600;
    static const uint16_t   DATA_RATE_49200 = DATA_RATE | 0x06;
    static const uint16_t   DATA_RATE_4800  = DATA_RATE | 0x47;
    static const uint16_t   DATA_RATE_2400  = DATA_RATE | 0x91;
    static const uint16_t   DATA_RATE_1200  = DATA_RATE | 0x9E;
    static const uint16_t   DATA_RATE_600  = DATA_RATE | 0xC8;

    static void select() { SS::clear(); }
    static void deselect() { SS::set(); }
};

#endif


/*
  Copyright (c) 2013 Sylvain Leroux <sylvain@chicoree.fr>
  
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
#if !defined AVR_TINO_HIH6120_H
#define AVR_TINO_HIH6120_H

/*
    Support for Honeywell HIH6120 -- 2-wire Digital Humidity/Temperature Sensors
*/

template<class TWI>
class HIH6120 : public TWI::Device { // XXX replace inheritance by composition
    public:
    static const uint8_t FACTORY_ID = 0x27;
    struct __attribute__((__packed__)) Data {
        uint16_t hum;
        uint16_t temp;
    };

    struct Measure {
        float   hum;
        float   temp;
    };

    enum __attribute__((__packed__)) {
        /* Honeywell status code */
        NORMAL_OPERATION        = 0b00000000,
        STALE_DATA              = 0b01000000,
        DEVICE_IN_COMMAND_MODE  = 0b10000000,

        UNUSED                  = 0b11000000,

        /* Library status code */
        READ_ERROR              = 0b00000001,
    };

    HIH6120(uint8_t id = FACTORY_ID) : TWI::Device(id) {}

    uint8_t acquire(Data& dest) {
        uint8_t    buffer[4];
        if (!this->read(sizeof(buffer), buffer)) {
            return READ_ERROR;
        }

        // swap endianess
        dest.hum = ((buffer[0] & 0x3F) << 8 ) | buffer[1];
        dest.temp = ( buffer[2] << 8 ) | ( buffer[1] & 0xC0 );

        return buffer[0] & 0xC0;
    }

    uint8_t acquire(Measure& dest) {
        Data binaryData;
        uint8_t status = acquire(binaryData);

        if (status)
            return status;

        dest.hum = float(binaryData.hum)*100./16382.;

        // '*4' below to discard non significant 2 bits
        dest.temp = float(binaryData.temp)*165./(16382.*4)-40.; 
    }
};

#endif

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
#if !defined AVR_TINO_TOUCAN_H
#define AVR_TINO_TOUCAN_H

/*
    ''touCAN'' is the ''truly open CAN'' specification for home automation.

    touCAN specifies standard OID (object ID) and common messages formats
*/


// bit 31                          bit 0
// |                              |
// < SIDH >< SIDL >< EID8 >< EID0 >
// <   SID   >xxx<       EID      >
// 10987654321098765432109876543210
// PR< CID>CMDxxxGR<     DEST     >

// touCAN OID is build on top of CAN 29bits SID/EID
// touCAN OID contains:
// - a priority level (PR) (11) critical (10) important (00) normal
// - a class identifier (CID) common to all (functionnally) equivalent devices
// - a destination address (DEST). Destination address is either a group address or a device address
// - a command field (CMD)

/*
    Each touCAN device MAY belongs to (at least) one groupe
    Each touCAN devide MUST have one unique address on the network
    While (re)set to factory defaults, on initial connexion, or while comming
    back online, each touCAN device SHOULD start an address discovery sequence.

*/

#define TOUCAN_OID(PR, CID, CMD, GR, DEST) { \
    uint8_t((PR << 6) | CID), \
    uint8_t((CMD << 5) | GR), \
    uint8_t(DEST >> 8), \
    uint8_t(DEST) \
    }

#define TOUCAN_PR(OID) (OID.sidh >> 6)
#define TOUCAN_CID(OID) (OID.sidh & 0x3F)
#define TOUCAN_CMD(OID) (OID.sidl >> 5)
#define TOUCAN_GR(OID) (OID.sidl & 0x03)
#define TOUCAN_DEST(OID) uint16_t((OID.eid8 << 8) | OID.eid0)

#define TOUCAN_PR_CRITICAL	    0b11
#define TOUCAN_PR_IMPORTANT	    0b10
#define TOUCAN_PR_NORMAL	    0b00

#define TOUCAN_CID_ANY	    0x00 // Match any device

// reserced CID
#define TOUCAN_CID_RESERVED_START   0x01
// All CID between those two are reserved
#define TOUCAN_CID_RESERVED_END	    0x10

// generic devices
#define TOUCAN_CID_ANALOG_SENSOR    0x11
#define TOUCAN_CID_DIGITAL_SENSOR   0x12
#define TOUCAN_CID_ANALOG_OUTPUT    0x13
#define TOUCAN_CID_DIGITAL_OUTPUT   0x14


#define TOUCAN_GRP_ADDR	    0b00
#define TOUCAN_DEV_ADDR	    0b11

#define TOUCAN_CMD_PING	    0b000
#define TOUCAN_CMD_RESET    0b001
#define TOUCAN_CMD_INFO	    0b100 // PING response
#define TOUCAN_CMD_GET	    0b010
#define TOUCAN_CMD_SET	    0b011
#define TOUCAN_CMD_DATA	    0b110 // GET response

namespace TouCAN {

struct Frame __attribute__ ((__packed__)) {
    CAN::ID	    oid;
    uint8_t	    dlc;
    union {
	uint8_t	    data[8];

	struct {

	} ping;

	struct {

	} reset;

	struct {
	    uint8_t	id;
	    uint32_t	value;
	} info;

	struct {
	    uint8_t	id;
	} get;

	struct {
	    uint8_t	id;
	    uint32_t	value;
	} data;

	struct {
	    uint8_t	id;
	    uint32_t	value;
	} set;
    };
};

}; // namespace

#endif

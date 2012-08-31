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

#include "avr-tino/can.h"

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
    uint8_t(CMD | GR), \
    uint8_t(DEST >> 8), \
    uint8_t(DEST) \
    }

#define TOUCAN_PR(OID) (OID.sidh >> 6)
#define TOUCAN_CID(OID) (OID.sidh & 0x3F)
#define TOUCAN_CMD(OID) (OID.sidl & (0b111 << 5) )
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
#define TOUCAN_CID_ANALOG_INPUT	    0x11
#define TOUCAN_CID_DIGITAL_INPUT    0x12
#define TOUCAN_CID_ANALOG_OUTPUT    0x13
#define TOUCAN_CID_DIGITAL_OUTPUT   0x14

#define TOUCAN_CID_MASK	    0x3F // 


#define TOUCAN_GRP_ADDR	    0b00
#define TOUCAN_DEV_ADDR	    0b10
#define TOUCAN_SRC_ADDR	    0b01
#define TOUCAN_DST_ADDR	    0b00

#define TOUCAN_CMD_PING	    (0b000 << 5)
#define TOUCAN_CMD_ECHO	    (0b100 << 5) // PING response
#define TOUCAN_CMD_RESET    (0b001 << 5)
#define TOUCAN_CMD_GET	    (0b010 << 5)
#define TOUCAN_CMD_SET	    (0b011 << 5)
#define TOUCAN_CMD_DATA	    (0b110 << 5) // GET response


namespace TouCAN {

typedef CAN::ID	    OID;

inline bool match(OID a, OID b) {
    return (TOUCAN_CID(a) == TOUCAN_CID(b))
	    && (a.eid8 == b.eid8)
	    && (a.eid0 == b.eid0);
}

template<class PL, uint8_t CMD>
struct __attribute__ ((__packed__)) Frame {
    inline Frame<PL, CMD>(OID theOid, const PL& thePl) 
	: oid(theOid),
	  dlc(sizeof(PL)),
	  pl(thePl) { oid.sidl |= CMD;  }
    OID		    oid;
    uint8_t	    dlc;
    PL		    pl;
};

struct __attribute__ ((__packed__)) PingPL {
    PingPL(OID theFrom) : from(theFrom) {}

    OID		    from;
};

#define TOUCAN_PING(frame) (*(::TouCAN::PingPL*)(frame.data))

Frame<PingPL, TOUCAN_CMD_PING> ping(OID oid, OID from) {
    return Frame<PingPL, TOUCAN_CMD_PING>(oid, from);
}

struct __attribute__ ((__packed__)) EchoPL {
    EchoPL(OID theFrom) : from(theFrom) {}

    OID		    from;
};

#define TOUCAN_ECHO(frame) (*(::TouCAN::EchoPL*)(frame.data))

Frame<EchoPL, TOUCAN_CMD_ECHO> echo(OID oid, OID from) {
    return Frame<EchoPL, TOUCAN_CMD_ECHO>(oid, from);
}

struct __attribute__ ((__packed__)) SelfPL {
    OID		    oid;
};

}; // namespace

static const TouCAN::OID    TOUCAN_OID_DEV_MASK = 
    TOUCAN_OID(0, TOUCAN_CID_MASK, 0, TOUCAN_DEV_ADDR, 0xFFFF);

#endif


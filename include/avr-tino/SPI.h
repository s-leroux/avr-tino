#if !defined AVR_TINO_SPI_H
#define AVR_TINO_SPI_H

class SPIMaster {
    public:
    SPIMaster();
    uint8_t transfert(uint8_t data) const;
};

#endif


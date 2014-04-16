#include "avr-tino/RF12.h"

template<class SPI, class SS, class IRQ>
volatile uint8_t RF12Module<SPI,SS,IRQ>::mode = IDLE_MODE;

template<class SPI, class SS, class IRQ>
volatile uint8_t RF12Module<SPI,SS,IRQ>::buff_pos = 0;

template<class SPI, class SS, class IRQ>
volatile uint8_t RF12Module<SPI,SS,IRQ>::buff_len = 0;

template<class SPI, class SS, class IRQ>
volatile uint8_t          RF12Module<SPI,SS,IRQ>::buff[RX_BUFF_SIZE];

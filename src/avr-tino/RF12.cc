#include "avr-tino/RF12.h"

template<class SPI, class SS>
volatile uint8_t RF12Module<SPI,SS>::mode = IDLE_MODE;

template<class SPI, class SS>
volatile uint8_t RF12Module<SPI,SS>::buff_pos = 0;

template<class SPI, class SS>
volatile uint8_t RF12Module<SPI,SS>::buff_len = 0;

template<class SPI, class SS>
uint8_t          RF12Module<SPI,SS>::buff[RX_BUFF_SIZE];

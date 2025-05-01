#ifndef __TM1637_H__
#define __TM1637_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>

void tm1637_init(GPIO_TypeDef* clk_port, uint16_t clk_pin, GPIO_TypeDef* data_port, uint16_t data_pin);
void tm1637_set_brightness(uint8_t brightness);
void tm1637_show_digits(const char *str);

#endif // __TM1637_H__

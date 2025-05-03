#ifndef __TM1637_H__
#define __TM1637_H__

#include "stm32f4xx_hal.h"
#include <stdint.h>

#define SEG_A 0x77  // A: abcdef
#define SEG_B 0x7C  // b: cdefg
#define SEG_C 0x39  // C: adef
#define SEG_D 0x5E  // d: bcdeg
#define SEG_E 0x79  // E: adefg
#define SEG_F 0x71  // F: aefg
#define SEG_G 0x3D  // G: acdef
#define SEG_H 0x76  // H: bcefg
#define SEG_I 0x06  // I: bc
#define SEG_J 0x1E  // J: bcde
#define SEG_K 0x75  // K: aefg (approx)
#define SEG_L 0x38  // L: def
#define SEG_M 0x37  // M: abce (approx)
#define SEG_N 0x54  // N: abce (approx)
#define SEG_O 0x3F  // O: abcdef
#define SEG_P 0x73  // P: abefg
#define SEG_Q 0x67  // Q: abcfg (approx)
#define SEG_R 0x50  // R: eg (approx)
#define SEG_S 0x6D  // S: acdfg
#define SEG_T 0x78  // T: defg
#define SEG_U 0x3E  // U: bcdef
#define SEG_V 0x1C  // V: cde (approx)
#define SEG_W 0x2A  // W: bdf (approx)
#define SEG_X 0x76  // X: bcefg (same as H)
#define SEG_Y 0x6E  // Y: bcdfg
#define SEG_Z 0x5B  // Z: abdeg
#define SEG_DASH 0x40  // -
#define SEG_SPACE 0x00  // (blank)

void tm1637_init(GPIO_TypeDef* clk_port, uint16_t clk_pin, GPIO_TypeDef* data_port, uint16_t data_pin);
void tm1637_set_brightness(uint8_t brightness);
void tm1637_show_digits(const char *str);
void tm1637_delay_long(uint32_t how_long);

#endif // __TM1637_H__

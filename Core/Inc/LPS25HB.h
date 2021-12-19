/*
 * LPS25HB.h
 *
 *  Created on: 16. 12. 2021
 *      Author: Lenka
 */

#ifndef LPS25HB_LPS25HB_H_
#define LPS25HB_LPS25HB_H_
#include "stdint.h"
#include "i2c.h"
#define LPS25HB_I2C_ADDRESS_0 0b10111000 //na zem
#define LPS25HB_I2C_ADDRESS_1 0b10111010 //na napajanie
#define LPS25HB_REG_WHO_AM_I_ADDR 0x0F //adresa konkrétneho registra v snimači
#define LPS25HB_WHO_AM_I_VALUE 0b10111101
#define LPS25HB_ADDRESS_CTRL1 0x20
#define LPS25HB_ADDRESS_PRESSOUTXL 0x28
#define LPS25HB_ADDRESS_PRESSOUTL 0x29
#define LPS25HB_ADDRESS_PRESSOUTH 0x2A

void lps25hb_write_byte(uint8_t reg_address, uint8_t data);

uint8_t lps25hb_read_byte(uint8_t reg_address);

uint8_t lps25hb_init();

float lps25hb_get_pressure();
float lps25hb_get_alltitude(float pressure);



#endif /* LPS25HB_LPS25HB_H_ */

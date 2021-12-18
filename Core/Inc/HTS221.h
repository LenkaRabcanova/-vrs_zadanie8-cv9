/*
 * HTS221.h
 *
 *  Created on: 16. 12. 2021
 *      Author: Lenka
 */

#ifndef HTS221_HTS221_H_
#define HTS221_HTS221_H_
#include "stdint.h"
#include "i2c.h"

#define HTS221_I2C_ADDRESS 0b1011111
#define HTS221_REG_WHO_AM_I_ADDR 0x0F  //adresa konkrétneho registra v snimači
#define HTS221_WHO_AM_I_VALUE 0b10111100
#define HTS221_ADDRESS_CTRL1 0x20

#define HTS221_ADDRESS_HUMIDITYOUTL 0x28
#define HTS221_ADDRESS_HUMIDITYOUTH 0X29

#define HTS221_ADDRESS_TEMPOUTL 0x2A
#define HTS221_ADDRESS_TEMPOUTH 0X2B

void hts221_write_byte(uint8_t reg_address, uint8_t data);

uint8_t hts221_read_byte(uint8_t reg_address);

uint8_t hts221_init();

float hts221_get_temperature();

float hts221_get_relative_humidity();


#endif /* HTS221_HTS221_H_ */

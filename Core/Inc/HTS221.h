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

#define HTS221_I2C_ADDRESS 0b10111110
#define HTS221_REG_WHO_AM_I_ADDR 0x0F  //adresa konkrétneho registra v snimači
#define HTS221_WHO_AM_I_VALUE 0b10111100
#define HTS221_ADDRESS_CTRL1 0x20

#define HTS221_ADDRESS_HUMIDITYOUTL 0x28
#define HTS221_ADDRESS_HUMIDITYOUTH 0X29

#define HTS221_ADDRESS_TEMPOUTL 0x2A
#define HTS221_ADDRESS_TEMPOUTH 0X2B

//kalibracne registre
#define HTS221_ADDRESS_H0rHx2 0x30
#define HTS221_ADDRESS_H1rHx2 0x31
#define HTS221_ADDRESS_T0degCx8 0x32
#define HTS221_ADDRESS_T1degCx8 0x33
#define HTS221_ADDRESS_T1T0MSB 0x35
#define HTS221_ADDRESS_H0T0OUTL 0x36
#define HTS221_ADDRESS_H0T0OUTH 0x37
#define HTS221_ADDRESS_H1T0OUTL 0x3A
#define HTS221_ADDRESS_H1T0OUTH 0x3B
#define HTS221_ADDRESS_T0OUTL 0x3C
#define HTS221_ADDRESS_T0OUTH 0x3D
#define HTS221_ADDRESS_T1OUTL 0x3E
#define HTS221_ADDRESS_T1OUTH 0x3F

void hts221_write_byte(uint8_t reg_address, uint8_t data);

uint8_t hts221_read_byte(uint8_t reg_address);

uint8_t hts221_init();

float hts221_get_temperature();

float hts221_get_relative_humidity();


#endif /* HTS221_HTS221_H_ */

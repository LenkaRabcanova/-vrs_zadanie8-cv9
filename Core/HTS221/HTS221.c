/*
 * HTS221.c
 *
 *  Created on: 16. 12. 2021
 *      Author: Lenka
 */

#include "hts221.h"

static uint8_t address = HTS221_I2C_ADDRESS;

void hts221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, address, 0);
}

uint8_t  hts221_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address, 0));
}

uint8_t hts221_init(void) {

	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = hts221_read_byte(HTS221_REG_WHO_AM_I_ADDR);

	if(val == HTS221_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		status = 0;

	}
	//nastavenie registrov
	uint8_t ctrl1 = 0b10000011; //PD active mode, reserved, BDU cont update, 12.5Hz rate
	hts221_write_byte(HTS221_ADDRESS_CTRL1, ctrl1);

	return status;
}

float hts221_get_temperature()  {

	uint8_t l_temperature = hts221_read_byte(HTS221_ADDRESS_TEMPOUTL);
	uint8_t h_temperature = hts221_read_byte(HTS221_ADDRESS_TEMPOUTH);

	int16_t temperature = l_temperature | (h_temperature << 8);

	float final_temperature;

	final_temperature = temperature/8;

	return final_temperature;
}

float hts221_get_relative_humidity() {

	uint8_t l_humidity = hts221_read_byte(HTS221_ADDRESS_HUMIDITYOUTL);
	uint8_t h_humidity = hts221_read_byte(HTS221_ADDRESS_HUMIDITYOUTH);

	int16_t humidity = l_humidity | (h_humidity  << 8);

	float final_humidity;

	final_humidity = humidity/2;

	return final_humidity;
}





/*
 * LPS25HB.c
 *
 *  Created on: 16. 12. 2021
 *      Author: Lenka
 */


#include "lps25hb.h"
#include "math.h"

static uint8_t address = LPS25HB_I2C_ADDRESS_1;

void lps25hb_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, address, 0);
}


uint8_t  lps25hb_read_byte(uint8_t reg_addr)
{
	uint8_t data = 0;
	return *(i2c_master_read(&data, 1, reg_addr, address, 0));
}

uint8_t lps25hb_init(void) {

	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_REG_WHO_AM_I_ADDR);

	if(val == LPS25HB_WHO_AM_I_VALUE)
	{
		status = 1;
	}
	else			//if the device is not found on one address, try another one
	{
		address = LPS25HB_I2C_ADDRESS_1;
		val = lps25hb_read_byte(LPS25HB_WHO_AM_I_VALUE);
		if(val == LPS25HB_REG_WHO_AM_I_ADDR)
		{
			status = 1;
		}
		else
		{
			status = 0;
		}
	}
	//nastavenie registrov

	uint8_t ctrl1 = 0b11000000; // power enable,  ODR 25Hz, interupt off, BDU off, autozero off
	lps25hb_write_byte(LPS25HB_ADDRESS_CTRL1, ctrl1);

	return status;

}

float lps25hb_get_pressure() {

	uint8_t xl_pressure = lps25hb_read_byte(LPS25HB_ADDRESS_PRESSOUTXL);
	uint8_t l_pressure = lps25hb_read_byte(LPS25HB_ADDRESS_PRESSOUTL);
	uint8_t h_pressure = lps25hb_read_byte(LPS25HB_ADDRESS_PRESSOUTH);

	uint32_t pressure = xl_pressure | (l_pressure << 8) | (h_pressure << 16);

	float final_pressure;

	final_pressure = pressure/4096;

	return final_pressure;
}

float lps25hb_get_alltitude(float final_pressure) {

	//H = 44330 * [ 1 - (P / p0) ^ (1 / 5,255) ]
	//H = altitude (-100~44330 m)
	//P = measured pressure from the sensor (0~107900 Pa)
	//p0 = reference pressure at sea level (87000~107900 Pa)

	float alltitude;

	alltitude = 44330*(1-pow(final_pressure/1013.25, 1/5.255));

	return alltitude;
}


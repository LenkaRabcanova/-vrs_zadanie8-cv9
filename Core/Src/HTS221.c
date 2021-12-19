/*
 * HTS221.c
 *
 *  Created on: 16. 12. 2021
 *      Author: Lenka
 */

#include "hts221.h"

static uint8_t address = HTS221_I2C_ADDRESS;

//hodnoty pre kalibraciu teploty (final_temperature)
static float T0deg = 0;
static float T1deg = 0;
static float T0out = 0;
static float T1out = 0;

//hodnoty pre kalibraciu vlhkosti (final humidity)
static float H0rh = 0;
static float H1rh = 0;
static float H0T0out = 0;
static float H1T0out = 0;

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



	//nastavenie kalibracnych registrov
	uint8_t H0rHx2 = hts221_read_byte(HTS221_ADDRESS_H0rHx2);
	uint8_t H1rHx2 = hts221_read_byte(HTS221_ADDRESS_H1rHx2);



	uint8_t T0degCx8 = hts221_read_byte(HTS221_ADDRESS_T0degCx8);
	uint8_t T1degCx8 = hts221_read_byte(HTS221_ADDRESS_T1degCx8);

	uint8_t T1T0MSB = hts221_read_byte(HTS221_ADDRESS_T1T0MSB);

	uint16_t T0degCx8MSB =  T1T0MSB & 0x3;
	uint16_t T1degCx8MSB =  (T1T0MSB & (0x3 << 2)) >> 2;

	int16_t T0degCx8_final = T0degCx8  | (T0degCx8MSB << 8);
	int16_t T1degCx8_final = T1degCx8  | (T1degCx8MSB << 8);




	uint8_t H0T0OUTLOW = hts221_read_byte(HTS221_ADDRESS_H0T0OUTL);
	uint8_t H0T0OUTHIGH = hts221_read_byte(HTS221_ADDRESS_H0T0OUTH);

	int16_t H0T0OUT = H0T0OUTLOW | (H0T0OUTHIGH << 8);



	uint8_t H1T0OUTLOW = hts221_read_byte(HTS221_ADDRESS_H1T0OUTL);
	uint8_t H1T0OUTHIGH = hts221_read_byte(HTS221_ADDRESS_H1T0OUTH);

	int16_t H1TOUT = H1T0OUTLOW  | (H1T0OUTHIGH << 8);




	uint8_t T0OUTLOW = hts221_read_byte(HTS221_ADDRESS_T0OUTL);
	uint8_t T0OUTHIGH = hts221_read_byte(HTS221_ADDRESS_T0OUTH);

	int16_t T0OUT = T0OUTLOW | (T0OUTHIGH << 8);




	uint8_t T1OUTLOW = hts221_read_byte(HTS221_ADDRESS_T1OUTL);
	uint8_t T1OUTHIGH = hts221_read_byte(HTS221_ADDRESS_T1OUTH);

	int16_t T1OUT =  T1OUTLOW | (T1OUTHIGH << 8);


	//HODNOTY POTREBNE PRE KALIBRACIU VYSLEDNEJ HODNOTY TEPLOTY (TEMPETATURE)
	T0deg = T0degCx8_final/8.0f;
	T1deg = T1degCx8_final/8.0f;

	T0out = T0OUT;
	T1out = T1OUT;

	//HODNOTY POTREBNE PRE KALIBRACIU VYSLEDNEJ HODNOTY VLHKOSTI (RELATIVE HUMIDITY)
	H0rh = H0rHx2/2.0f;
	H1rh = H1rHx2/2.0f;

	H0T0out = H0T0OUT;
	H1T0out = H1TOUT;

	return status;
}

float hts221_get_temperature()  {

	//LINEARNA INTERPOLACIA VZOREC: y = y1 + ((x – x1) / (x2 – x1)) * (y2 – y1)

	//TEPLOTA: x = temperature out(measured), x1 = T0out, x2 = T1out
	// y = temperature deg(measured), y1 = T0deg, y2 = T1deg

	uint8_t l_temperature = hts221_read_byte(HTS221_ADDRESS_TEMPOUTL);
	uint8_t h_temperature = hts221_read_byte(HTS221_ADDRESS_TEMPOUTH);

	int16_t temperature = l_temperature | (h_temperature << 8);

	float final_temperature = T0deg + ((temperature - T0out) / (T1out-T0out)) * (T1deg - T0deg);

	return final_temperature;
}

float hts221_get_relative_humidity() {

	//LINEARNA INTERPOLACIA VZOREC: y = y1 + ((x – x1) / (x2 – x1)) * (y2 – y1)

	//VLHKOST: x = humidity out(measured), x1 = H0T0out, x2 = H1T0out
    // y = humidity rh(measured), y1 = H0rh, y2 = H1rh

	uint8_t l_humidity = hts221_read_byte(HTS221_ADDRESS_HUMIDITYOUTL);
	uint8_t h_humidity = hts221_read_byte(HTS221_ADDRESS_HUMIDITYOUTH);

	int16_t humidity = l_humidity | (h_humidity  << 8);

	float final_humidity = H0rh + ((humidity - H0T0out) / (H1T0out-H0T0out)) * (H1rh - H0rh);

	return final_humidity;
}





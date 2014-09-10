/*
  MS5806_02BA52.cpp 
  - Arduino library for controlling the Measurement Specialities MS5806_02BA52 in I2C mode.
  
  Copyright 2014 by AIW Industries, LLC (Richard Vogel)
  
  This program is free software: you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  GNU General Public License for more details. 

  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "MS5806_02BA52.h"
#include "math.h"
//
//  Global Pressure variable
//
float pressure = 0.00;
float p_temperature = 0.00;
//
//
//**********Class Variables*************//

uint16_t MS5806_02BA52::C1 = constrain(C1, 0, 65535);
uint16_t MS5806_02BA52::C2 = constrain(C2, 0, 65535);
uint16_t MS5806_02BA52::C3 = constrain(C3, 0, 65535);
uint16_t MS5806_02BA52::C4 = constrain(C4, 0, 65535);
uint16_t MS5806_02BA52::C5 = constrain(C5, 0, 65535);
uint16_t MS5806_02BA52::C6 = constrain(C6, 0, 65535);
uint16_t MS5806_02BA52::C7 = constrain(C7, 0, 65535);
//
//  Pressure and Temp variables.
//
uint32_t MS5806_02BA52::DP1 = constrain(DP1, 0, 16777216UL);
uint32_t MS5806_02BA52::DP2 = constrain(DP2, 0, 16777216UL);

float MS5806_02BA52::temperature = 0.00;

int32_t MS5806_02BA52::dT = constrain(dT, -16776960L, 16777216L);
int32_t MS5806_02BA52::TEMP = constrain(TEMP, -4000L, 8500L);

int64_t MS5806_02BA52::OFF = constrain(OFF, -17179344900LL, 25769410560LL);
int64_t MS5806_02BA52::SENS = constrain(SENS, -8589672450LL, 12884705280LL);
int32_t MS5806_02BA52::P = constrain(P, 1000L, 120000L);

int64_t MS5806_02BA52::T2 = 0LL;
int64_t MS5806_02BA52::OFF2 = 0LL;
int64_t MS5806_02BA52::SENS2 = 0LL;

//
//  Class I2C buffer variable.
//
uint8_t MS5806_02BA52::p_Response[8];
uint16_t MS5806_02BA52::pPROM[] = pPROM_READ;
//
// Begin using the Si4707.
//
void MS5806_02BA52::begin(void)
{
	sendCommand(RESET);  
	delay(2);
	getPROM();
}    
//
//  Performs calculation of all data from module
//
void MS5806_02BA52::getData()
{
	getTemperature();
	getPressure();
}	
//
//  Gets the calculation coefficients from PROM.
//
void MS5806_02BA52::getPROM(void)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		sendCommand(pPROM[i]);
		readBytes(2);
		uint16_t a = 0x0000;
		switch(i)
		{
			case 1:
				a = (a | p_Response[0]) << 8;
				a = a | p_Response[1];
				C1 = a;
				break;
			
			case 2:
				a = (a | p_Response[0]) << 8;
				a = a | p_Response[1];
				C2 = a;
				break;
			
			case 3:
				a = (a | p_Response[0]) << 8;
				a = a | p_Response[1];
				C3 = a;
				break;
			
			case 4:
				a = (a | p_Response[0]) << 8;
				a = a | p_Response[1];
				C4 = a;
				break;
			
			case 5:
				a = (a | p_Response[0]) << 8;
				a = a | p_Response[1];
				C5 = a;
				break;
			
			case 6:
				a = (a | p_Response[0]) << 8;
				a = a | p_Response[1];
				C6 = a;
				break;
				
			case 7:
				a = (a | p_Response[0]) << 8;
				a = a | p_Response[1];
				a = 0x0000 | (a >> 4);
				C7 = a;
				break;
			
			default:
				break;
		}
	}		
} 
//
//  Request Temperature Data
//
void MS5806_02BA52::getTemperature(void)
{
	uint32_t tempT= 0x00000000;
	sendCommand(Dp2_4096);
	readData();
	tempT = (tempT | ((0x00000000 | p_Response[0]) << 16) | ((0x00000000 | p_Response[1]) << 8) | p_Response[2]);
	DP2 = uint32_t(tempT);
	dT = DP2 - (C5 * 256);
	TEMP = 2000 + dT * (C6 / 8388608);
	if (TEMP >= 2000)
	{
		T2 = 0LL;
		OFF2 = 0LL;
		SENS2 = 0LL;
	}
	else if (TEMP < 2000)
	{
		calcLowTemp();	
		if (TEMP < -1500)
		{
			calcV_LowTemp();
		}
	}
	TEMP = TEMP - T2;
	OFF = OFF - OFF2;
	SENS = SENS - SENS2;
	temperature = float(TEMP) / 100;
	p_temperature = temperature;
}
//
// Calculate Low Temperature Temp/Pressure Compensation
//
void MS5806_02BA52::calcLowTemp()
{
	T2 = pow(dT, 2) / 2147483648LL;
	OFF2 = 61 * pow((TEMP - 2000), 2) / 16;
	SENS2 = 2 * pow((TEMP - 2000), 2);
}
//
// Calculate Very Low Temperature Temp/Pressure Compensation
//
void MS5806_02BA52::calcV_LowTemp()
{
	OFF2 = OFF2 + 20 * pow((TEMP + 1500), 2);
	SENS2 = SENS2 + 12 * pow((TEMP + 1500), 2);
}
//
//  Request Pressure Data
//
void MS5806_02BA52::getPressure(void)
{
	uint32_t tempP = 0x0000;
	sendCommand(Dp1_4096);
	readData();
	for (int i = 0; i < 3; i++)
	{
	    Serial.println(p_Response[i], HEX);
	}
	tempP = (tempP | ((0x0000 | p_Response[0]) << 16) | ((0x0000 | p_Response[1]) << 8) | p_Response[2]);
	DP1 = uint32_t(tempP);  // base pressure value
	Serial.print(F("prelim pressure: "));
	Serial.println(DP1, DEC);
	
	/*****************calc temp compensated pressure****************/
	
	OFF = (C2 * 131072) + ((C4 * dT) / 64);
	SENS = (C1 * 65536) + ((C3 * dT) / 128);
	P = ((DP1 * SENS / 2097152 - OFF) / 32768);
	pressure = float(P) / 100;	
}

//
//  Send a single command.
//
void MS5806_02BA52::sendCommand(uint8_t cmd)
{
	Wire.beginTransmission(P_Address);
	Wire.write(cmd);
	Wire.endTransmission();
	delay(2);
}
//
//  Read a single byte.
//
void MS5806_02BA52::readByte(void)
{
	Wire.requestFrom(P_Address, 1);
	delay(2);
	if(Wire.available() > 0)
	{
		p_Response[0] = Wire.read();
	}
	delay(2);  
}
//
//  Reads multiple bytes defined by quantity.
//
void MS5806_02BA52::readBytes(int quantity)
{
	uint8_t i = 0;
  
	Wire.requestFrom(P_Address, quantity);
	delay(2);  
	while(Wire.available() > 0)
    {
      p_Response[i] =  Wire.read(); 
      i++;
    }  
	delay(2);
}
//
//  Read Conversion Data.
//
void MS5806_02BA52::readData(void)
{
	sendCommand(ADC_READ);
	readBytes(3);
}
//
//
//
MS5806_02BA52 PTU_P;

/*
  MS5806_02BA52.h 
  - Arduino library for controlling the Measurement Specialities MS5806_02BA52
    Pressure Sensor in I2C mode.
  
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
#ifndef MS5806_02BA52_h
#define MS5806_02BA52_h
//
#include "application.h"
//
#define P_Address						0x76		// I2C Address w/CSB pin tied to ground
//
//  MS5806_02BA52 Pressure Sensor Command Definitions.
//
#define	RESET							0x1E		//  Reset device.

#define	pPROM_READ {0xA0, 0xA2, 0xA4, 0xA6, 0xA8, 0xAA, 0xAC, 0xAE} //Address commands to read PROM coefficients 0-7.

#define	D1_256							0x40		//  Command to initiate pressure conversion OSR = 256.
#define	D1_512							0x42		//  Command to initiate pressure conversion OSR = 512.
#define	D1_1024							0x44		//  Command to initiate pressure conversion OSR = 1024.
#define	D1_2048							0x46		//  Command to initiate pressure conversion OSR = 2048.
#define	D1_4096							0x48		//  Command to initiate pressure conversion OSR = 4096.
#define	D2_256							0x50		//  Command to initiate temperature conversion OSR = 256.
#define	D2_512							0x52		//  Command to initiate temperature conversion OSR = 512.
#define	D2_1024							0x54		//  Command to initiate temperature conversion OSR = 1024.
#define	D2_2048  						0x56		//  Command to initiate temperature conversion OSR = 2048.
#define	D2_4096  						0x58		//  Command to initiate temperature conversion OSR = 4096.

#define ADC_READ						0x00 		//  Command to read requested data.
//
//  Global Pressure and Temp variables.
//
extern float pressure;
extern float p_temperature;
//
//  MS5806_02BA52 Class.
//
class MS5806_02BA52 
{
  public: 

    void begin(void);
    void getData(void);
      
  private:
	//  Calibration and CRC Values.
    static uint16_t C1;
	static uint16_t C2;
	static uint16_t C3;
	static uint16_t C4;
	static uint16_t C5;
	static uint16_t C6;
	static uint16_t C7;
	//  Pressure variables within the class
	static uint32_t D1;
	static int32_t P;
	//  Temperature variables within the class
	static uint32_t D2;
	static int32_t dT;
	static float temperature;
	static int32_t TEMP;
	static int64_t OFF;
	static int64_t SENS;
	static int64_t T2;
	static int64_t OFF2;
	static int64_t SENS2;
	//  I2C response buffer and array to hold PROM Address commands
	static uint8_t p_Response[];
    static uint16_t pPROM[];

    void getPROM(void);
    void getPressure(void);
    void getTemperature(void);
    void calcLowTemp(void);
    void calcV_LowTemp(void);
    void readData(void);
    void sendCommand(uint8_t cmd);
    void readByte(void);
    void readBytes(int quantity);
};

extern MS5806_02BA52 PTU_P;

#endif  //  End of MS5806_02BA52.h


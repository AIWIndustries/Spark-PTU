#include "MS5806_02BA52.h"



void setup()
{
    Wire.begin();
    Serial.begin(115200);
    while(!Serial.available()) // Just chill until we Rx a character. Don't chill
    {
        delay(100);
    }
    /******************************************************************************************************************************/
    delay(100);
    Serial.println(F("Starting up the Pressure Sensor......."));
    Serial.println(F(""));
    delay(10);
    PTU_P.begin();
}

void loop()
{
    PTU_P.getData();
    Serial.print(F("Pressure: "));
    Serial.print(pressure, 2);
    Serial.println(F(" mBar\n"));
  
    Serial.print(F("P_Sensor Temp: "));
    Serial.print(p_temperature, 2);
    Serial.println(F(" degrees C"));
  
    delay(5000);
}

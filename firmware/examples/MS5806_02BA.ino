#include <Wire.h>
#include <MS5806_02BA52.h>


void setup()
{
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
  
  delay(1000);
}

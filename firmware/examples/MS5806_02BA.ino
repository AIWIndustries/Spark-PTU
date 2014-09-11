#include "MS5806_02BA52.h"

void setup()
{
    Wire.begin();
    Serial.begin(115200);
    
    //uncomment the block comment below if you want the code to wait for you to enter a keyboard
    //stroke before it starts.
    
    /*while(!Serial.available()) 
    {
        delay(100);
    }*/
   
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
    Serial.println(F(" mBar"));
    float p_Hg = pressure * 0.02952998751; //mBar to inHg conversion
    Serial.print(F("Pressure: "));
    Serial.print(p_Hg, 2);
    Serial.println(F(" inHg\n"));
    
    Serial.print(F("P_Sensor Temp: "));
    Serial.print(p_temperature, 2);
    Serial.println(F(" degrees C"));
    float tF = (p_temperature * 1.8) + 32; // C to F conversion
    Serial.print(F("P_Sensor Temp: "));
    Serial.print(tF, 2);
    Serial.println(F(" degrees F\n"));
    
  
    delay(5000);
}

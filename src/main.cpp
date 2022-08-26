#include <Arduino.h>
#include <ArduinoJson.h>
#include "EEPROM.h"
DynamicJsonDocument doc(1024); // set json

// ============== setup eeprom ============//
void setup_eeprom()
{
   Serial.println("Testing EEPROMClass\n");
  if (!EEPROM.begin(1000))
  {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
}

// ============== setup eeprom ============//

void setup()
{
  Serial.begin(115200);
  delay(1000);
  setup_eeprom();
 
}

void handle_json_serial()
{

  deserializeJson(doc, Serial);
  JsonObject object = doc.as<JsonObject>();
  if (!object.isNull())
  {
    String set_config = doc["SET_CONFIG"];
    String read_config = doc["READ_CONFIG"];

    if (set_config != "null")
    {
      String data;
      serializeJson(doc, data);
      EEPROM.writeString(0, data);
      EEPROM.commit();
    }

    if (read_config != "null")
    {
      DynamicJsonDocument jsonRead(1024);

      String dataJsonRead = EEPROM.readString(0);
      deserializeJson(jsonRead, dataJsonRead);
      JsonObject object = jsonRead.as<JsonObject>();

      if (!object.isNull())
      {
        String result = jsonRead["SET_CONFIG"];
        Serial.println(result);
      }

      Serial.print("read config: ");
      Serial.println();
    }
  }
}

void loop()
{
  handle_json_serial();
}
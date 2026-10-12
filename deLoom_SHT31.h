#include <Adafruit_SHT31.h>
#include "deLoom_core.h"
#include <ArduinoJson.h>

#define SHT31_address 0x44

// does not need to do power up or power down function stuff

void SHT31_init(Adafruit_SHT31* sht);

void SHT31_measure(JsonObject contentsObject, Adafruit_SHT31* sht);
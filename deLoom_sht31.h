#include <Adafruit_SHT31.h>
#include "deLoom_core.h"
#include <ArduinoJson.h>

#define SHT31_address 0x44

// does not need to do power up or power down function stuff

void SHT31_init();

void SHT31_measure(JsonArray contentsArray);
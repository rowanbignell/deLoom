#include "SensirionI2cSen66.h"
#include <Wire.h>
#include <bitset>
#include <ArduinoJson.h>

#include "deLoom_core.h"

#define PM_AVERAGE_COUNT 10    // Number of times to read the pm values then average them over
#define SEN66_I2C_ADDRESS 0x6B // Standard I2C address for SEN66

void SEN66_init(SensirionI2cSen66* sen66);

void SEN66_measure(JsonObject contentsObject, SensirionI2cSen66* sen66);

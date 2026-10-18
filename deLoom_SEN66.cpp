#include "deLoom_SEN66.h"

void SEN66_init(SensirionI2cSen66* sen66){
    Serial.println(F("Attempting to initialize SEN66..."));

    Wire.begin();
    sen66->begin(Wire, SEN66_I2C_ADDRESS);

    if (sen66.deviceReset()){
        Serial.println(F("Failed to reset SEN66."));
    } else {
        Serial.println(F("Successfully reset SEN66!"));
    }
    delay(1200);

    if(sen66->startContinuousMeasurement(startContinuousMeasurement)){
        Serial.println(F("Failed to start SEN66."));
    } else {
        Serial.println(F("Started SEN66!"));
        delay(5000);
    }


}

void SEN66_measure(JsonObject contentsObject, SensirionI2cSen66* sen66){

}
#include "deLoom_SEN66.h"

void SEN66_init(SensirionI2cSen66* sen66){
    Serial.println(F("Attempting to initialize SEN66..."));

    Wire.begin();
    sen66->begin(Wire, SEN66_I2C_ADDRESS);

    if (sen66->deviceReset()){
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
    Serial.println(F("Attempting to measure SEN66..."));

    JsonObject sen66_obj = conentsObject["sen66"].as<JsonObject>();
    //setup temp variables
    float tmpPm1p0, tmpPm2p5, tmpPm4p0, tmpPm10p0;
    float tmpHum, tmpTemp, tmpVoc, tmpNox;
    uint16_t tmpCo2;
    float tmpNumPm0p5, tmpNumPm1p0, tmpNumPm2p5, tmpNumPm4p0, tmpNumPm10p0;

    int validSamples = 0;


    //loop to read values and put it in the object
    for (int i = 0; i < PM_AVERAGE_COUNT; i++) {
        delay(1000);

        uint8_t padding;
        bool dataReady = false;
        sen66->getDataReady(padding, dataReady); // Check if new data is ready

        //check if ready
        if(dataReady){
            //put it in temp
            // filter temp
            if (!sen66->readMeasuredValues(tmpPm1p0, tmpPm2p5, tmpPm4p0, tmpPm10p0, tmpHum,
                                             tmpTemp, tmpVoc, tmpNox, tmpCo2) && tmpPm2p5 < 6000.0 && tmpCo2 < 60000) {
                // add to object
                sen66_obj["PM1_0"] = sen66_obj["PM1_0"] + tmpPm1p0;
                sen66_obj["PM2_5"] = sen66_obj["PM2_5"] + tmpPm2p5;
                sen66_obj["PM4_0"] = sen66_obj["PM4_0"] + tmpPm4p0;
                sen66_obj["PM10_0"] = sen66_obj["PM10_0"] + tmpPm10p0;
                sen66_obj["AmbientHumidity"] = sen66_obj["AmbientHumidity"] + tmpHum;
                sen66_obj["AmbientTemperature"] = sen66_obj["AmbientTemperature"] + tmpTemp;
                sen66_obj["VocIndex"] = sen66_obj["VocIndex"] + tmpVoc;
                sen66_obj["NoxIndex"] = sen66_obj["NoxIndex"] + tmpNox;
                sen66_obj["C02"] = sen66_obj["C02"] + tmpCo2;

                // iterate
                validSamples++;
            }
        }
    }
    //divide by num samples
    sen66_obj["PM1_0"] = sen66_obj["PM1_0"] / PM_AVERAGE_COUNT;
    sen66_obj["PM2_5"] = sen66_obj["PM2_5"] / PM_AVERAGE_COUNT;
    sen66_obj["PM4_0"] = sen66_obj["PM4_0"] / PM_AVERAGE_COUNT;
    sen66_obj["PM10_0"] = sen66_obj["PM10_0"] / PM_AVERAGE_COUNT;
    sen66_obj["AmbientHumidity"] = sen66_obj["AmbientHumidity"] / PM_AVERAGE_COUNT;
    sen66_obj["AmbientTemperature"] = sen66_obj["AmbientTemperature"] / PM_AVERAGE_COUNT;
    sen66_obj["VocIndex"] = sen66_obj["VocIndex"] / PM_AVERAGE_COUNT;
    sen66_obj["NoxIndex"] = sen66_obj["NoxIndex"] / PM_AVERAGE_COUNT;
    sen66_obj["C02"] = sen66_obj["C02"] / PM_AVERAGE_COUNT;

    //if valid samples wasn't iterated, print error
    if(!validSamples){
        Serial.println(F("No Valid sen66 samples!"));
    } else {
            Serial.println(F("Sen66 Measured!"));
    }
}
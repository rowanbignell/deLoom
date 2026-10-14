#include "deLoom_SHT31.h"

void SHT31_init(Adafruit_SHT31* sht){
    Serial.println(F("Attempting to initialize SHT31..."));
    if(!sht->begin(SHT31_address)){
        Serial.println(F("Failed to initialize SHT31! Check connections and try again..."));
    }
    else{
        Serial.println(F("Successfully initialized SHT31!"));
    }
}

void SHT31_measure(JsonObject contentsObject, Adafruit_SHT31* sht){
    Serial.println(F("Attempting to measure from the SHT31"));
    contentsObject["sht31"]["Temperature_C"] = sht->readTemperature();
    contentsObject["sht31"]["Humidity_%RH"] = sht->readHumidity();
    Serial.println(F("Measured from the SHT31"));
}
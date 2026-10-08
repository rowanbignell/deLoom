#include "deLoom_SHT31.h"

void SHT31_init(){
    if(!sht.begin(SHT31_address)){
        Serial.println(F("Failed to initialize SHT31! Check connections and try again..."));
    }
    else{
        Serial.println(F("Successfully initialized SHT31!"));
    }
}

void SHT31_measure(JsonObject contentsArray){
    contentsObject["sht31"]["Temperature_C"] = sht.readTemperature();
    contentsObject["sht31"]["Humidity_%RH"] = sht.readHumidity();
}
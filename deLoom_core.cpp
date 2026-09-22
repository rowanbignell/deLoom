#include <ArduinoJson.h>
#include "Arduino.h"

#include <deLoom_core.h>
#include <deLoom_Hypnos.h>
#include <deLoom_SD.h>


void power_down(){
    //power down the sensors
}

void power_up(){
    //power up the sensors
}

void measure(bool display){
    //pull measure data from the sensors
    Serial.println(F("Ran measure()"));

    //create package exterior
    DynamicJsonDocument doc(JSON_SIZE);

    //run measure on the submodules giving them the exterior

    //display?
    if (display){
        //display finished packet
    }

    //log finished packet?
    if(enableSD){

    }
}

void begin_serial(bool waitForSerial){
    long startMillis = millis();

    Serial.begin(BAUD_RATE);

    //wait for the serial monitor to start
    if(waitForSerial){
        while(!Serial){
            // If it has been 20 seconds break out of the loop
            if(millis() >= (startMillis+20000)){
                break;
            }
        }
    }
}

void initialize(){
    //do any initialization tasks that the sensors require
}

void package(){
    //package the data from measure (might simplify for now idk)
}

void display(){
    //display data from package (might simplify away for now idk)
}
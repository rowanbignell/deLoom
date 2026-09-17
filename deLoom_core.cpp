#include <deLoom_core.h>

void power_down(){
    //power down the sensors
}

void power_up(){
    //power up the sensors
}

void measure(){
    //pull measure data from the sensors
}

void begin_serial(){
    Serial.begin(BAUD_RATE);
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
#pragma once

#include "Arduino.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <stdio.h>
#include <string.h>

#define BAUD_RATE 115200        // Serial interface baud rate

void power_down();

void power_up();

void measure(bool display);

void begin_serial(bool waitForSerial);

void initialize();

void package();

void display();


struct deLoom_Instance{
    char deviceName[100];                                   // Name of the device
    uint32_t instanceNumber;                                // Instance number of the device
    uint32_t packetNumber = 1;                              // Tracks the current packet number
    DynamicJsonDocument doc;
    char serial_num[33];
};
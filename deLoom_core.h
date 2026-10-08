#pragma once

#include "Arduino.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <stdio.h>
#include <string.h>
#include <SdFat.h>

#define BAUD_RATE 115200        // Serial interface baud rate
#define MAX_JSON_SIZE 2000

void power_down();

void power_up();

void deLoom_measure(bool display, SdFat* sd, char* deviceName, char* serialNum, uint32_t instanceNum, uint32_t* packetNum);

void begin_serial(bool waitForSerial);

void deLoom_initialize(char* serialNum);

void read_serial_num(char* serialNum);

void deLoom_package();

void deLoom_display();

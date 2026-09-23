#pragma once

#include "Arduino.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <stdio.h>
#include <string.h>
#include <SdFat.h>

#define BAUD_RATE 115200        // Serial interface baud rate

void power_down();

void power_up();

void deLoom_measure(bool display, SdFat* sd, char* deviceName);

void begin_serial(bool waitForSerial);

void deLoom_initialize(char* serial_num);

void read_serial_num(char* serial_num);


void deLoom_package();

void deLoom_display();

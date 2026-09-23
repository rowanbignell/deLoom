#pragma once

#include "Arduino.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <stdio.h>
#include <string.h>

#define BAUD_RATE 115200        // Serial interface baud rate

void power_down();

void power_up();

void deLoom_measure(bool display);

void begin_serial(bool waitForSerial);

void deLoom_initialize();

void deLoom_package();

void deLoom_display();

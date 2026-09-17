#include "Arduino.h"
#include <Wire.h>
#include <stdio.h>
#include <string.h>

#define BAUD_RATE 115200        // Serial interface baud rate

void power_down();

void power_up();

void measure();

void begin_serial();

void initialize();

void package();

void display();
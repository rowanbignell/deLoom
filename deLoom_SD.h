#pragma once

#include <SPI.h>
#include <SdFat.h>
#include <OPEnS_RTC.h>
#include <ArduinoJson.h>
#include <OPEnS_RTC.h>
#include <deLoom_Hypnos.h>


#include "deLoom_core.h"

#define sd_chip_select 11

bool write_line_to_file(const char* filename, const char* content);

void write_header_to_file(const char* filename);

void write_headers(File* myFile, DynamicJsonDocument* doc, char* serialNum);

void update_modified_date(File* myFile);

void log_to_sd(const char* filename);

void initialize_sd();

void sd_begin(SdFat* sd);
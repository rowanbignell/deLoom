#pragma once

#include <SPI.h>
#include <SdFat.h>
#include <OPEnS_RTC.h>

#include "deLoom_core.h"

#define sd_chip_select 11

bool write_line_to_file(const char* filename, const char* content);

void write_header_to_file(const char* filename);

void log_to_sd(const char* filename);

void initialize_sd();
void sd_begin();

struct sdManager_Instance{ 
    File myFile;
    File scanningFile;
    File root;

    SdFat sd;
    int chip_select;                                        // Chip select pin for the SD card
    char device_name[100];                                  // Device name of the whole thing used as the starting point of the SD file name

    char batchFileName[260];                                // File name to log batches to
    char fileName[260];                                     // Current file name that data is being logged to
    char fileNameNoExtension[260];                          // Current file name that data is being logged to without the file extension
    char overrideFileName[260];

    int batch_size = -1;                                    // How many packets to log per batch
    int current_batch = 0;                                  // Current count of the batch
    int file_count = 0;                                     // What file number are we logging to

    bool sdInitialized = false;                             // If the SD card actually initialized
    char* headers[2];                                       // Contains the main and sub headers that are added to the top of the CSV files

};
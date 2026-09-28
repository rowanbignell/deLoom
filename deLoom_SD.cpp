#include <deLoom_SD.h>

bool write_line_to_file(const char* filename, const char* content){

}

void write_header_to_file(const char* filename){

}

void log_to_sd(const char* filename){

}

void initialize_sd(){

}

void write_headers(File* myFile, DynamicJsonDocument* doc){
    DateTime currentTime = RTC_DS.now();

    myFile->timestamp(T_CREATE, currentTime.year(), currentTime.month(), currentTime.day(), currentTime.hour(), currentTime.minute(), currentTime.second());

}


void sd_begin(SdFat* sd){
    digitalWrite(8, HIGH);  // Disable LoRa

    Serial.println((F("** Initializing SD Card **")));

    // Start the SD card with the fastest SPI speed
    if(!sd->begin(sd_chip_select, SD_SCK_MHZ(50))){
        Serial.println((F("** Failed to initialize SD card **")));
    }
    else{
        Serial.println((F("** Successfully initialized SD card **")));
    }
}
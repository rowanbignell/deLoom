#include <deLoom_SD.h>

bool write_line_to_file(const char* filename, const char* content){

}

void write_header_to_file(const char* filename){

}

void log_to_sd(const char* filename){

}

void initialize_sd(){

}

void write_headers(File* myFile, DynamicJsonDocument* doc, char* serialNum, uint32_t* packetNum){
    DateTime currentTime = RTC_DS.now();
    //set the created timestamp
    myFile->timestamp(T_CREATE, currentTime.year(), currentTime.month(), currentTime.day(), currentTime.hour(), currentTime.minute(), currentTime.second());

    char header1[513];
    char header2[513];

    // Append the serial number to the top of the CSV file, reset the header1 array
    snprintf_P(header1, 512, PSTR("%s\n"), serialNum);
    myFile->println(header1);

    // Clear both arrays
    memset(header1, '\0', 512);
    memset(header2, '\0', 512);

    JsonObject document = doc->as<JsonObject>();
    strncat(header1, "ID,,packet,", 512);
    strncat(header2, "name,instance,number,", 512);
    
    // If there is a key that contains timestamp data when need to include that separately 
    if(document.containsKey("timestamp")){
        strncat(header1, "timestamp,", 512);
    }
    
    // Get the contents containing the reset of the sensor data
    JsonArray contentsArray = document["contents"].as<JsonArray>();

    // Loop over each 
    for(JsonVariant v : contentsArray) {
        // Get the module name
        strncat(header1, v.as<JsonObject>()["module"].as<const char*>(), 512);

        // Get all JSON keys  
        for(JsonPair keyValue : v.as<JsonObject>()["data"].as<JsonObject>()){
            strncat(header2, keyValue.key().c_str(), 512);
            strncat(header2, ",", 512);
            strncat(header1, ",", 512);
        }
    }

    // Write the headers to the file
    myFile->println(header1);
    myFile->println(header2);
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

void update_modified_date(File* myFile){
    DateTime currentTime = RTC_DS.now();
    myFile->timestamp(T_WRITE , currentTime.year(), currentTime.month(), currentTime.day(), currentTime.hour(), currentTime.minute(), currentTime.second());
}

#include <deLoom_core.h>
#include <deLoom_Hypnos.h>
#include <deLoom_SD.h>

void power_down(){
    //power down the sensors
}

void power_up(){
    //power up the sensors
}

void deLoom_measure(bool display, SdFat* sd, char* deviceName, char* serialNum, uint32_t instanceNum, uint32_t* packetNum){
    //pull measure data from the sensors
    DynamicJsonDocument doc(MAX_JSON_SIZE);

    //create package exterior
    Serial.println(F("** Packaging **"));
    
    // Clear the document so that we don't get null characters after too many updates
    doc.clear();
    doc[F("type")] = F("data");
    doc["id"]["name"] = deviceName;
    doc["id"]["instance"] = instanceNum;
    doc["Packet"]["Number"] = *packetNum;

    // Get the contents of the JSON document (i feel like this sucks)
    JsonArray contentsArray = doc["contents"];
    if(contentsArray.isNull())
        contentsArray = doc.createNestedArray("contents");

    // Add the packet number to the JSON document
    //JsonObject json = get_data_object("Packet");
    //json["Number"] = packetNum;

    // TODO:
    //run measure on the submodules giving them the exterior
    Serial.println(F("** Measuring **"));

    //display?
    if (display){
        //display finished packet
        char jsonStr[MAX_JSON_SIZE];
        serializeJsonPretty(doc, jsonStr, MAX_JSON_SIZE);
        Serial.println(jsonStr);
    }

    //log finished packet?
    if(enableSD){
        char fileName[260];
        snprintf_P(fileName, 260, PSTR("%s%i.csv"), deviceName, instanceNum); 

        File myFile;
        myFile = sd->open(fileName, O_RDWR | O_CREAT | O_APPEND);

        if(myFile){
            //if this is the first time opening the file, then need to add header
            if(myFile.available() <= 3){
                write_headers(&myFile, &doc, serialNum, packetNum);
            }

            //
            
            //do the thing
            char output[MAX_JSON_SIZE + 1];

            // Write the Instance data that isn't included in the JSON packet
            snprintf_P(output, MAX_JSON_SIZE, PSTR("%s,%i,%i,"), deviceName, instanceNum, *packetNum);
            myFile.print(output);
            memset(output, '\0', MAX_JSON_SIZE); // Clear array

            // If there is a key that contains timestamp data when need to include that separately 
            if(doc.containsKey("timestamp")){
                char utcArr[21];
                memset(utcArr, '\0', 21);
                strncpy(utcArr, doc["timestamp"]["time_utc"].as<const char*>(), 21);

                // Format date with spaces when logging to SD
                char *indexPointer = strchr(utcArr, 'Z');
                if(indexPointer != nullptr){
                    utcArr[10] = ' ';
                    utcArr[indexPointer-utcArr] = '\0';
                }

                // Format the time stamp in the CSV file
                strncat(output, utcArr, MAX_JSON_SIZE);
                strncat(output, ",", MAX_JSON_SIZE);
            }


            //module data

            // Loop over each 
            for(JsonVariant v : contentsArray) {

                // Get all JSON keys  
                for(JsonPair keyValue : v.as<JsonObject>()["data"].as<JsonObject>()){
                    strncat(output, keyValue.value().as<String>().c_str(), MAX_JSON_SIZE);
                    strncat(output, ",", MAX_JSON_SIZE);
                }
            }

            // Write the matching data into the CSV file
            myFile.println(output);

            // Set the last modified date
            update_modified_date(&myFile);

            // Close the file
            myFile.close();

            // Inform the user that we have successfully written to the file
            Serial.println("Successfully written to file.");

        }
    }

    //post-measure
    ++*packetNum;

}

void begin_serial(bool waitForSerial){
    long startMillis = millis();

    Serial.begin(BAUD_RATE);

    //wait for the serial monitor to start
    if(waitForSerial){
        while(!Serial){
            // If it has been 20 seconds break out of the loop
            if(millis() >= (startMillis+20000)){
                break;
            }
        }
    }
}

void deLoom_initialize(char* serialNum){
    //do any initialization tasks that the sensors require
    //grab the serial num and put it in
    read_serial_num(serialNum);
}

void read_serial_num(char* serialNum){
    char serial_no[33];
    // Serial numbers are made up of four words located at these specific registers (see datasheet)
	uint32_t sn_words[4];
	sn_words[0] = *(volatile uint32_t *)(0x0080A00C);
	sn_words[1] = *(volatile uint32_t *)(0x0080A040);
	sn_words[2] = *(volatile uint32_t *)(0x0080A044);
	sn_words[3] = *(volatile uint32_t *)(0x0080A048);

    // Take these raw values and convert them into a string of hex characters
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			snprintf_P(serial_no + (i * 8) + (j * 2), 33, PSTR("%02X"), (uint8_t)(sn_words[i] >> ((3 - j) * 8)));
		}
	}

    // Copy the contents of the calculated char array into the member variable
    strncpy(serialNum, serial_no, 33);
}

void deLoom_package(){
    //package the data from measure (might simplify for now idk)
}

void deLoom_display(){
    //display data from package (might simplify away for now idk)
}
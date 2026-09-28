#include <deLoom_core.h>
#include <deLoom_Hypnos.h>
#include <deLoom_SD.h>

void power_down(){
    //power down the sensors
}

void power_up(){
    //power up the sensors
}

void deLoom_measure(bool display, SdFat* sd, char* deviceName){
    //DateTime currentTime = RTC_DS.now();
    //pull measure data from the sensors
    Serial.println(F("Ran measure()"));

    DynamicJsonDocument doc(2000);

    //create package exterior

    //run measure on the submodules giving them the exterior

    //display?
    if (display){
        //display finished packet
    }

    //log finished packet?
    if(enableSD){
        File myFile;
        myFile = sd->open(deviceName, O_RDWR | O_CREAT | O_APPEND);

        //if this is the first time opening the file, then need to add header
        if(myFile.available() <= 3){
            write_headers(&myFile, &doc);
        }
        
        //do the thing

    }
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

void deLoom_initialize(char* serial_num){
    //do any initialization tasks that the sensors require
    //grab the serial num and put it in
    read_serial_num(serial_num);
}

void read_serial_num(char* serial_num){
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
    strncpy(serial_num, serial_no, 33);
}

void deLoom_package(){
    //package the data from measure (might simplify for now idk)
}

void deLoom_display(){
    //display data from package (might simplify away for now idk)
}
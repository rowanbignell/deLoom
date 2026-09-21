#include <deLoom_Hypnos.h>
#include <deLoom_core.h>

/** 
* hypnos initialization tasks
*/
void hypnos_init(){
    // Set the pins to write mode
    pinMode(5, OUTPUT);                     // 3.3v power rail
    pinMode(6, OUTPUT);                     // 5v power rail
    pinMode(LED_BUILTIN, OUTPUT);           // Status LED

    initializeRTC();
}

/** 
* pre-sleep tasks
*/
void pre_sleep(){
    Serial.println((F("** Going to Sleep **")));

    //power down the devices
    power_down();

    // Close the serial connection and detach
    Serial.end();
    delay(50);

    // Disable the power rails
    hypnos_disable(USE_33, USE_5);

}

/** 
* post-sleep startup tasks
*/
void post_sleep(){
    //enable the power rails
    hypnos_enable(USE_33, USE_5);

    //start the serial monitor
    begin_serial(true);

    Serial.println((F("** Woke Up **")));

    // power on the devices
    power_up();

}
/** 
* sleep for given amount of seconds
* @param seconds amount of seconds to sleep
* @param waitForSerial whether to wait for the serial monitor
*/
void sleep(uint32_t seconds, bool waitForSerial){
    pre_sleep();

    //set up alarm then sleep
    shouldPowerUp = false;
    LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, wakeup, 0);
    LowPower.sleep(seconds*1000);

    //if it's not time to wake up yet, go back to sleep
    while (!shouldPowerUp) {
        LowPower.sleep();
    }

    post_sleep();  // Wake up

    long startMillis = millis();

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

/**
 * On wakeup placeholder
*/
static void wakeup(){
    shouldPowerUp = true;

}

/** 
* re-enable the power rails on the hypnos
* @param enable33 if the 3.3v rail should be enabled
* @param enable5 if the 5v rail should be enabled
*/
void hypnos_enable(bool enable33, bool enable5){
    // Enable the 3.3v and 5v rails on the Hypnos
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
}

/** 
* disable the power rails on the hypnos
* @param disable33 if the 3.3v rail should be disabled
* @param disable5 if the 5v rail should be disabled
*/
void hypnos_disable(bool disable33, bool disable5){
    // Disable the 3.3v and 5v rails on the Hypnos
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(LED_BUILTIN, LOW);
}

/**
 * RTC initialization tasks
*/
void initializeRTC() {
    Serial.println("Initializing DS3231....");

    // If the RTC failed to start inform the user and hang
    if(!RTC_DS.begin()){
        Serial.println(F("Couldn't start RTC! Check your connections... Execution will now hang as this is likely a fatal error"));
        return;
    }

    //set a custom time if the serial monitor is active
    if(Serial){
        set_custom_time();
    }

    RTC_DS.writeSqwPinMode(DS3231_OFF);

    // We successfully started the RTC
    Serial.println(F("DS3231 Real-Time Clock Initialized Successfully!"));
    RTC_initialized = true;

}

/**
 * Set custom time for RTC from user input
*/
void set_custom_time(){
   	// initialized variable for user input
	String computer_year = "";
	String computer_month = "";
	String computer_day = "";
	String computer_hour = "";
	String computer_min = "";
	String computer_sec = "";
	// Let the user know that they should NOT enter local time
	Serial.println(F("Please use UTC time, not local!"));

	// Entering the year
	Serial.println(F("Enter the Year (Four digits, e.g. 2020)"));
	while(computer_year == ""){
		computer_year = Serial.readStringUntil('\n');
	}

	// Entering the month
	Serial.println(F("Enter the Month (1 ~ 12)"));
	while(computer_month == ""){
		computer_month = Serial.readStringUntil('\n');
	}

	// Entering the day
	Serial.println(F("Enter the Day (1 ~ 31)"));
	while(computer_day  == ""){
		computer_day = Serial.readStringUntil('\n');
	}

	// Entering the hour
	Serial.println(F("Enter the Hour (0 ~ 23)"));
	while(computer_hour == ""){
		computer_hour = Serial.readStringUntil('\n');
	}

	// Entering the minute
	Serial.println(F("Enter the Minute (0 ~ 59)"));
	while(computer_min == ""){
		computer_min = Serial.readStringUntil('\n');
	}

	// Entering the second
	Serial.println(F("Enter the Second (0 ~ 59)"));
	while(computer_sec == ""){
		computer_sec = Serial.readStringUntil('\n');
	}

    // Set the RTC to the custom time
    RTC_DS.adjust(DateTime(computer_year.toInt(), computer_month.toInt(), computer_day.toInt(), computer_hour.toInt(), computer_min.toInt(), computer_sec.toInt()));
    RTC_initialized = true;

    // Output
    Serial.println(F("Custom Time Set."));
}

void dateTime_toString(DateTime time, char array[21]){
    // Formatted as: YYYY-MM-DDTHH:MM:SSZ
    snprintf_P(array, 21, PSTR("%04u-%02u-%02uT%02u:%02u:%02uZ"), time.year(), time.month(), time.day(), time.hour(), time.minute(), time.second());
}


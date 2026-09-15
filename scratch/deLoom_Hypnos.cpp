#include "deLoom_Hypnos.h"

void hypnos_init(){
    // Set the pins to write mode
    pinMode(5, OUTPUT);                     // 3.3v power rail
    pinMode(6, OUTPUT);                     // 5v power rail
    pinMode(LED_BUILTIN, OUTPUT);           // Status LED
}

void pre_sleep(){
    Serial.println((F("** Going to Sleep **")));
    delay(50);

    bool disable5 = is5VDisabled(DEVICE_STATE::ENTERING_SLEEP);
    bool disable33 = is3VDisabled(DEVICE_STATE::ENTERING_SLEEP);

    // Close the serial connection and detach
    Serial.end();

    // Disable the power rails
    hypnos_disable(disable33, disable5);

}

void post_sleep(){
    // Check if they are not disabled to see if they should be enabled
    bool enable5 = !is5VDisabled(DEVICE_STATE::EXITING_SLEEP);
    bool enable33 = !is3VDisabled(DEVICE_STATE::EXITING_SLEEP);

    hypnos_enable(enable33, enable5); // Checks if the 3.3v or 5v are disabled and re-enables them

    Serial.println((F("** Woke Up **")));
}

void sleep(uint32_t seconds, bool waitForSerial){
    power_down();

    pre_sleep();
    shouldPowerUp = false;

    LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, wakeup, 0);
    LowPower.sleep(seconds);

    while (!shouldPowerUp) {
        LowPower.sleep();
    }

    post_sleep();  // Wake up

    power_up();

    if (waitForSerial){
        while(!Serial);
    }
}

static void wakeup(){

}

void hypnos_enable(bool enable33, bool enable5){
    // Enable the 3.3v and 5v rails on the Hypnos
    digitalWrite(5, (enable33) ? LOW : HIGH);
    digitalWrite(6, (enable5) ? HIGH : LOW);
    digitalWrite(LED_BUILTIN, HIGH);
}

void hypnos_disable(bool disable33, bool disable5){
    // Disable the 3.3v and 5v rails on the Hypnos
    digitalWrite(5, (disable33) ? HIGH : LOW);
    digitalWrite(6, (disable5) ? LOW : HIGH);
    digitalWrite(LED_BUILTIN, LOW);
}

void initializeRTC() {

}

bool is3VDisabled(DEVICE_STATE deviceState){
   
    switch (deviceState)
    {
        case ENTERING_SLEEP:
            switch (sleepModePowerConfig)
            {
                case PR_3V_ON_5V_ON:
                    return false;
                case PR_3V_ON_5V_OFF:
                    return false;
                case PR_3V_OFF_5V_ON:
                    return true;
                case PR_3V_OFF_5V_OFF:
                    return true;
            }
            break;
        case EXITING_SLEEP:
            switch (wakeModePowerConfig)
                {
                    case PR_3V_ON_5V_ON:
                        return false;
                    case PR_3V_ON_5V_OFF:
                        return false;
                    case PR_3V_OFF_5V_ON:
                        return true;
                    case PR_3V_OFF_5V_OFF:
                        return true;
                }
            break;
    }

    // We should never make it here but enable the rail if we do
    return false;
}

bool is5VDisabled(DEVICE_STATE deviceState){
    switch (deviceState)
    {
        case ENTERING_SLEEP:
            switch (sleepModePowerConfig)
            {
                case PR_3V_ON_5V_ON:
                    return false;
                case PR_3V_OFF_5V_ON:
                    return false;
                case PR_3V_ON_5V_OFF:
                    return true;
                case PR_3V_OFF_5V_OFF:
                    return true;
            }
            break;
        case EXITING_SLEEP:
            switch (wakeModePowerConfig)
                {
                    case PR_3V_ON_5V_ON:
                        return false;
                    case PR_3V_OFF_5V_ON:
                        return false;
                    case PR_3V_ON_5V_OFF:
                        return true;
                    case PR_3V_OFF_5V_OFF:
                        return true;
                }
            break;
    }

    // We should never make it here but enable the rail if we do
    return false;
}

void set_custom_time(){
   	// initialized variable for user input
	String computer_year = "";
	String computer_month = "";
	String computer_day = "";
	String computer_hour = "";
	String computer_min = "";
	String computer_sec = "";
    char output[OUTPUT_SIZE];

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

DateTime getCurrentTime(){

}

void dateTime_toString(DateTime time, char array[21]){
    
}


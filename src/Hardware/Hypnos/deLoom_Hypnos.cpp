#include "deLoom_Hypnos.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
deLoom_Hypnos::deLoom_Hypnos(Manager& man, bool use_custom_time) : Module("Hypnos"), custom_time(use_custom_time){
    manInst = &man;

    // Set the pins to write mode
    pinMode(5, OUTPUT);                     // 3.3v power rail
    pinMode(6, OUTPUT);                     // 5v power rail
    pinMode(LED_BUILTIN, OUTPUT);           // Status LED

    // Add the Hypnos to the module register
    manInst->registerModule(this);
    manInst->useHypnos();   // Enable the use of the hypnos
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
deLoom_Hypnos::~deLoom_Hypnos(){}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::package(){
    JsonObject json = manInst->getDocument().createNestedObject("timestamp");
    char timeStr[21];

    timeUtc = RTC_DS.now();

    dateTime_toString(timeUtc, timeStr);
    json["time_utc"] = timeStr;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

/* Power Rail Control Functionality */

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::enable(bool enable33, bool enable5){
    // Enable the 3.3v and 5v rails on the Hypnos
    digitalWrite(5, (enable33) ? LOW : HIGH);
    digitalWrite(6, (enable5) ? HIGH : LOW);
    digitalWrite(LED_BUILTIN, HIGH);

    // If the RTC hasn't already been initialized then do so now
    if(!RTC_initialized)
        initializeRTC();

    manInst->setEnableState(true);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::disable(bool disable33, bool disable5){
    // Disable the 3.3v and 5v rails on the Hypnos
    digitalWrite(5, (disable33) ? HIGH : LOW);
    digitalWrite(6, (disable5) ? LOW : HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    manInst->setEnableState(false);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool deLoom_Hypnos::is3VDisabled(DEVICE_STATE deviceState){
   
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
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool deLoom_Hypnos::is5VDisabled(DEVICE_STATE deviceState){
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
//////////////////////////////////////////////////////////////////////////////////////////////////////

/* Interrupt Functionality */

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool deLoom_Hypnos::registerInterrupt(InterruptCallbackFunction isrFunc, int interruptPin, int triggerState){
    pinMode(interruptPin, INPUT_PULLUP);  //  Set interrupt pin input mode
    Serial.println(F("Registering interrupt..."));

    // If the RTC hasn't already been initialized then do so now if we are trying to schedule an RTC interrupt
    if(!RTC_initialized && interruptPin == 12)
        initializeRTC();

    // Make sure a callback function was supplied
    if(isrFunc != nullptr){
        // If the interrupt we registered is for sleep we should set the interrupt to wake the device from sleep
        LowPower.attachInterruptWakeup(interruptPin, isrFunc, triggerState);
        Serial.println(F("Interrupt successfully attached!"));

        // Add the interrupt to the list of pin to interrupts
        pinToInterrupt.insert(std::make_pair(interruptPin, std::make_pair(isrFunc, triggerState)));
        return true;
    }
    else{
        detachInterrupt(digitalPinToInterrupt(interruptPin));
        Serial.println(F("Failed to attach interrupt! Interrupt callback evaluated to a null pointer, it is possible you forgot to supply a callback function"));
        return false;
    }
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool deLoom_Hypnos::reattachRTCInterrupt(int interruptPin){
    LowPower.attachInterruptWakeup(interruptPin, std::get<0>(pinToInterrupt[interruptPin]), std::get<1>(pinToInterrupt[interruptPin]));
    Serial.println(F("Interrupt successfully reattached!"));
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::wakeup(){
    detachInterrupt(pinToInterrupt.begin()->first);     // Detach the interrupt so it doesn't trigger again
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::initializeRTC(){
    char output[OUTPUT_SIZE];
    Serial.println("Initializing DS3231....");

    // If the RTC failed to start inform the user and hang
    if(!RTC_DS.begin()){
        Serial.println(F("Couldn't start RTC! Check your connections... Execution will now hang as this is likely a fatal error"));
        return;
    }

    // If we want to set a custom time
    if(Serial){
        set_custom_time();
    }

	// Clear any pending alarms
	RTC_DS.clearAlarm(1);
    RTC_DS.clearAlarm(2);


    RTC_DS.writeSqwPinMode(DS3231_OFF);

    // We successfully started the RTC
    Serial.println(F("DS3231 Real-Time Clock Initialized Successfully!"));
    RTC_initialized = true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::dateTime_toString(DateTime time, char array[21]){
    // Formatted as: YYYY-MM-DDTHH:MM:SSZ
    snprintf_P(array, 21, PSTR("%04u-%02u-%02uT%02u:%02u:%02uZ"), time.year(), time.month(), time.day(), time.hour(), time.minute(), time.second());
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::set_custom_time(){
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
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::setInterruptDuration(const TimeSpan duration){
    // The time in the future that the alarm will be set for
    timeAlarm = RTC_DS.now() + duration;
    RTC_DS.setAlarm1(timeAlarm, DS3231_A1_Date);
    Serial.println("Next interrupt alarm set.");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

/* Sleep Functionality */

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::sleep(uint32_t seconds, bool waitForSerial){
    bool hasAlarmTriggered = false;

    // Try to power down the active modules
    if (shouldPowerUp) {
        manInst->power_down();

        // After powering down the devices check if the alarmed time is less than the current time, this means that the alarm may have already triggered
        // this means that the alarm may have already triggered Adafruit getAlarm1() returns alarm
        // day/hour/min/sec with placeholder year/month; build comparable time from current date
        DateTime now = RTC_DS.now();
        DateTime alarmReg = RTC_DS.getAlarm1();
        DateTime alarmDateTime(now.year(), now.month(), alarmReg.day(), alarmReg.hour(),
                               alarmReg.minute(), alarmReg.second());
        uint32_t alarmedTime = alarmDateTime.unixtime();
        uint32_t currentTime = now.unixtime();
        hasAlarmTriggered = alarmedTime <= currentTime;
        
        // 50ms delay allows this last message to be sent before the bus disconnects
        Serial.println("Entering Standby Sleep...");
        delay(50);
    }

    // If it hasn't we should preform our sleep as before
    if(!hasAlarmTriggered){
        pre_sleep();                                            // Pre-sleep cleanup
        shouldPowerUp = true;
        LowPower.sleep(seconds / 1000);                                       // Go to sleep and hang
    }
    // If it has we want to trigger a resample which requires powering the sensors back up
    else{
        Serial.println("Alarm triggered during sample, specified sample duration was too short! Resampling...");
        reattachRTCInterrupt();
        if(shouldPowerUp){
            manInst->power_up();
        }
    }

    // If the alarm hadn't triggered last time we want to wake up like normal
    if(!hasAlarmTriggered)
        post_sleep(waitForSerial);         // Wake up
   
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::pre_sleep(){
    bool disable5 = is5VDisabled(DEVICE_STATE::ENTERING_SLEEP);
    bool disable33 = is3VDisabled(DEVICE_STATE::ENTERING_SLEEP);
    char output[OUTPUT_SIZE];
    delay(1000);

    // Close the serial connection and detach
    Serial.end();
    USBDevice.detach();

    // Reattach the interrupt to the RTC interrupt pin
    attachInterrupt(digitalPinToInterrupt(pinToInterrupt.begin()->first), std::get<0>(pinToInterrupt.begin()->second), std::get<1>(pinToInterrupt.begin()->second));

    // Disable the power rails
    disable(disable33, disable5);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
void deLoom_Hypnos::post_sleep(bool waitForSerial){
    // Enable the Watchdog timer when waking up
    
    if(shouldPowerUp){
        USBDevice.attach();
        Watchdog.reset();
        Serial.begin(115200);
        Watchdog.reset();

        // Check if they are not disabled to see if they should be enabled
        bool enable5 = !is5VDisabled(DEVICE_STATE::EXITING_SLEEP);
        Watchdog.reset();
        bool enable33 = !is3VDisabled(DEVICE_STATE::EXITING_SLEEP);
        Watchdog.reset();

        enable(enable33, enable5); // Checks if the 3.3v or 5v are disabled and re-enables them
        Watchdog.reset();
        delay(1000);
        Watchdog.reset();

        LOG(F("Device has awoken from sleep!"));
        Watchdog.reset();

        // Re-init the modules that need it
        manInst->power_up();

        //this version doesn't clear pending alarms here?
        // Clear any pending alarms
        RTC_DS.clearAlarm(1);
        RTC_DS.clearAlarm(2);

        // We want to wait for the user to re-open the serial monitor before continuing to see readouts
        if(waitForSerial){
            TIMER_DISABLE;
            while(!Serial);
            TIMER_ENABLE;
        }        
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////


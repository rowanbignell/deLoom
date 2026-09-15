#include "deLoom_Hypnos.h"


void pre_sleep(){
    Serial.println((F("** Going to Sleep **")));


}

void post_sleep(){


    Serial.println((F("** Woke Up **")));
}

void sleep(uint32_t seconds, bool waitForSerial){
    power_down();

    pre_sleep();
    shouldPowerUp = false;

    LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, wakeup, 0);
    LowPower.sleep(seconds);  // Go to sleep and hang

    Serial.println((F("** Going to Sleep **")));
}

static void wakeup(){

}

void hypnos_enable(bool enable33, bool enable5){
    // Set the pins to write mode
    pinMode(5, OUTPUT);                     // 3.3v power rail
    pinMode(6, OUTPUT);                     // 5v power rail
    pinMode(LED_BUILTIN, OUTPUT);           // Status LED

    // Enable the 3.3v and 5v rails on the Hypnos
    digitalWrite(5, (enable33) ? LOW : HIGH);
    digitalWrite(6, (enable5) ? HIGH : LOW);
    digitalWrite(LED_BUILTIN, HIGH);
}

void initializeRTC() {

}

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
    disable(disable33, disable5);

}

void post_sleep(){
    // Check if they are not disabled to see if they should be enabled
    bool enable5 = !is5VDisabled(DEVICE_STATE::EXITING_SLEEP);
    bool enable33 = !is3VDisabled(DEVICE_STATE::EXITING_SLEEP);

    enable(enable33, enable5); // Checks if the 3.3v or 5v are disabled and re-enables them

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

void disable(bool disable33, bool disable5){
    // Disable the 3.3v and 5v rails on the Hypnos
    digitalWrite(5, (disable33) ? HIGH : LOW);
    digitalWrite(6, (disable5) ? LOW : HIGH);
    digitalWrite(LED_BUILTIN, LOW);
}

void initializeRTC() {

}

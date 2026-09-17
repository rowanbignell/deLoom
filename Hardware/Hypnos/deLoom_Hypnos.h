#include <OPEnS_RTC.h>
#include <ArduinoLowPower.h>

#include "Arduino.h"


/**
 * Enum to easily see if we are going to sleep or waking up from sleep
 */
enum DEVICE_STATE{
    ENTERING_SLEEP,
    EXITING_SLEEP
};


/**
 * Enum to represent all power rail configurations
 */
enum POWERRAIL_CONFIG{
    PR_3V_ON_5V_ON,            // Both the 3v and 5v rails are enabled
    PR_3V_ON_5V_OFF,            // The 3v rail is enabled and the 5v rail is disabled
    PR_3V_OFF_5V_ON,            // The 3v rail is disabled and the 5v rail is enabled
    PR_3V_OFF_5V_OFF           // The 3v rail and the 5v rail are both disabled
};

/**
 * Drops the Feather M0 and Hypnos board into a low power sleep waiting for an interrupt to wake it up and pull it out of sleep
 * @param seconds Duration to sleep for
 * @param waitForSerial Whether or not we should wait for the user to open the serial monitor before continuing execution
 */
void sleep(uint32_t seconds, bool waitForSerial = false);

void hypnos_enable(bool enable33 = true, bool enable5 = true);

void hypnos_disable(bool disable33 = true, bool disable5 = true);

void hypnos_init();

void initializeRTC();

/**
 * Convert the current time to a ISO 8601 compatible time string
 *
 * @param time The current time as a DateTime object
 * @param array The buffer to write the string to (size 21)
*/
void dateTime_toString(DateTime time, char array[21]);

/**
 * Set a custom time on startup for the RTC to use
*/
void set_custom_time();

DateTime time;                                                                      // UTC time
DateTime alarmTime;                                                                 // Time the alarm has been set for

/* Sleep functionality */
void pre_sleep();                            // Called just before the hypnos enters sleep, this disconnects the power rails and the serial bus
void post_sleep();                           // Called just after the hypnos wakes up, this reconnects the power rails and the serial bus

bool is5VDisabled(DEVICE_STATE deviceState);
bool is3VDisabled(DEVICE_STATE deviceState);


/**
 * Handle interrupt when waking from sleep
 */
static void wakeup();
static volatile bool shouldPowerUp;
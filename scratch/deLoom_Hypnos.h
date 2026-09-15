#include <OPEnS_RTC.h>
#include <ArduinoLowPower.h>

#include "Arduino.h"


/**
 * Drops the Feather M0 and Hypnos board into a low power sleep waiting for an interrupt to wake it up and pull it out of sleep
 * @param seconds Duration to sleep for
 * @param waitForSerial Whether or not we should wait for the user to open the serial monitor before continuing execution
 */
 void sleep(uint32_t seconds, bool waitForSerial = false);

void hypnos_enable(bool enable33, bool enable5);

void initializeRTC();


/**
 * Get the current time from the RTC
 */
DateTime getCurrentTime();

/**
 * Convert the current time to a ISO 8601 compatible time string
 *
 * @param time The current time as a DateTime object
 * @param array The buffer to write the string to (size 21)
*/
void dateTime_toString(DateTime time, char array[21], bool isLocal = false);

/**
 * Set a custom time on startup for the RTC to use
*/
void set_custom_time();

void initializeRTC();                                                               // Initialize RTC

DateTime time;                                                                      // UTC time
DateTime alarmTime;                                                                 // Time the alarm has been set for

/* Sleep functionality */
void pre_sleep();                            // Called just before the hypnos enters sleep, this disconnects the power rails and the serial bus
void post_sleep();                           // Called just after the hypnos wakes up, this reconnects the power rails and the serial bus

/**
 * Handle interrupt when waking from sleep
 */
static void wakeup();
static volatile bool shouldPowerUp;
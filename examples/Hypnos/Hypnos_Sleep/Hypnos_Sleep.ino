/**
 * This is an example use case for the Hypnos board's sleep functionality
 * This allows the user to put the Feather into a deep sleep disabling power to all sensors and then resume operation after a given length of time
 */

#include <deLoom_core.h>
#include <deLoom_Hypnos.h>
#include <deLoom_SD.h>
#include <SdFat.h>


char deviceName[100] = "Test";                                // Name of the device
uint32_t instanceNumber = 1;                                // Instance number of the device
uint32_t packetNumber = 1;                              // Tracks the current packet number
char serial_num[33];
SdFat sd;


void setup() {

  // Start the serial interface
  begin_serial(true);

  // Enable the rails
  hypnos_enable();

  // initialize the devices
  deLoom_initialize();
  
  // initalize the hypnos
  hypnos_init();
}

void loop() {
  //measure from the sensors
  deLoom_measure(true, &sd, deviceName);
  
  // Put the device into a deep sleep, operation HALTS here until the interrupt is triggered
  sleep(5, true);
}
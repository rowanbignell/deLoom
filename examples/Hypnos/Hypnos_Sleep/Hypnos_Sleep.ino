/**
 * This is an example use case for the Hypnos board's sleep functionality
 * This allows the user to put the Feather into a deep sleep disabling power to all sensors and then resume operation after a given length of time
 * 
 * MANAGER MUST BE INCLUDED FIRST IN ALL CODE
 */

#include <scratch/core.h>
#include <scratch/deLoom_Hypnos.h>


void setup() {

  // Start the serial interface
  beginSerial();

  // Enable the hypnos rails
  hypnos_enable();

  // initialize the devices
  //manager.initialize();
  
}

void loop() {
  manager.measure();

  manager.package();

  manager.display();
  
  // Put the device into a deep sleep, operation HALTS here until the interrupt is triggered
  sleep(5, true);
}
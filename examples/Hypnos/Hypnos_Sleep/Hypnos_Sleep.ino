/**
 * This is an example use case for the Hypnos board's sleep functionality
 * This allows the user to put the Feather into a deep sleep disabling power to all sensors and then resume operation after a given length of time
 */

#include <deLoom_core.h>
#include <Hardware/Hypnos/deLoom_Hypnos.h>


void setup() {

  // Start the serial interface
  begin_serial();

  // Enable the rails
  hypnos_enable();

  // initialize the devices
  initialize();
  
  hypnos_init();
}

void loop() {
  measure();

  package();

  display();
  
  // Put the device into a deep sleep, operation HALTS here until the interrupt is triggered
  sleep(5, true);
}
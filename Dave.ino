/* Dave.ino
 * 
 * Arduino Nano project to annoy Dave */

#include <PCM.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "samples.h"

#define LED_PIN (2)

#define LED_MS (200)
uint16_t light_counter;
uint8_t light_state;

// ISR for Timer0 comp
SIGNAL(TIMER0_COMPA_vect)
{
  if(++light_counter >= LED_MS)
  {
    digitalWrite(LED_PIN, (light_state)?LOW:HIGH);
    light_state = !light_state;
    light_counter = 0;
  }
}

void setup() {
  // Get a unique seed each time
  uint8_t seed = EEPROM.read(0);
  EEPROM.write(0, seed + 1);
  srand(seed);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);     // Start with LED on

  digitalWrite(LED_BUILTIN, LOW); // Turn off builtin LED to save some powah

  // Setup Timer 0 to count for the LED
  // Timer0 is already set up for the millisecond counter,
  // so setting OCR0A to pretty much anything will work.
  // Most important part is TIMSK0
  light_counter = 0;
  light_state = 0;
  OCR0A = 0x01;
  TIMSK0 |= _BV(OCIE0A);
}

void loop() {
  uint8_t i = rand() % 20;
  char * sample;
  uint16_t len;
  uint16_t delay_len;

  // Chance of each sample is pretty much arbitrary...
  if(i < 5)
  {
    sample = &davidHelp[0];
    len = sizeof(davidHelp);
  }
  else if (i < 8)
  {
    sample = &please[0];
    len = sizeof(please);
  }
  else
  {
    sample = &david[0];
    len = sizeof(david);
  }
  startPlayback(sample, len);

  // Sample rate is 8000Hz -> 8 samples/ms
  delay_len = (len / 8);
  delay(delay_len);

  // Don't bother calling stopPlayback, startPlayback
  // doesn't rely on registers being re-initialized
}

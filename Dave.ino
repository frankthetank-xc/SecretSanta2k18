#include <PCM.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "samples.h"

#define LED_MS 200
uint16_t light_counter;
uint8_t light_state;


SIGNAL(TIMER0_COMPA_vect)
{
  if(++light_counter >= LED_MS)
  {
    digitalWrite(2, (light_state)?LOW:HIGH);
    light_state = !light_state;
    light_counter = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  srand(analogRead(0));
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);           // Start with LED on
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
  int i = rand() % 20;
  // put your main code here, to run repeatedly:
  char * sample;
  unsigned int len;
  unsigned int delay_len;
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
    sample = &please[0];
    len = sizeof(david);
  }
  startPlayback(sample, len);
  // Sample rate is 8000Hz -> 8 samples/ms
  delay_len = (len / 8);
  delay(delay_len);
}

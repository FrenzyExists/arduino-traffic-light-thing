#include <Arduino.h>
#include <Eventually.h>

/**
 * This here is a simple example on turning a yellow light ON and OFF with a push button
 * The library deals with the debouncing for us
 */

#define PED_BTN       10
#define YELLOW_LIGHT  8
#define RED_LIGHT     4
#define GREEN_LIGHT   2

int state = LOW;
int counter = 0;  // 0 -> run | 1 -> stop | 2 -> off
int seconds = 0;
EvtManager mgr;         // create event manager



bool blink() {
  Serial.println(counter); 
  if (counter >= 2) {
    digitalWrite(RED_LIGHT, LOW);
    digitalWrite(YELLOW_LIGHT, LOW);
    digitalWrite(GREEN_LIGHT, LOW);
    return false;
  } else {
      seconds ++;
      if (seconds == 0 || seconds <= 30) {
      digitalWrite(RED_LIGHT, LOW);
      digitalWrite(GREEN_LIGHT, HIGH);
    } else if (seconds <= 40) {
      digitalWrite(GREEN_LIGHT, LOW);
      digitalWrite(YELLOW_LIGHT, HIGH);
    } else if (seconds <= 60) {
      digitalWrite(YELLOW_LIGHT, LOW);
      digitalWrite(RED_LIGHT, HIGH);
    } else {
      seconds = 0;
    }
    return false;
  }
}

bool startBlink() {
  Serial.println("IN START EVENT");
  counter++;
  mgr.resetContext();
  mgr.addListener(new EvtTimeListener(1000, true, (EvtAction) blink));

  // Stop Blink
  mgr.addListener(new EvtPinListener(PED_BTN, (EvtAction) +[] {
    Serial.println("IN STOP EVENT");
    if (counter >= 2) {
          counter = 0;
        }
    mgr.resetContext();
    if (counter == 0) {
      startBlink();
    } else {
      mgr.addListener(new EvtPinListener(PED_BTN, (EvtAction) startBlink));
    }
    return true;
  }));
  return true;
}


void setup() {
  pinMode(PED_BTN, INPUT);
  pinMode(YELLOW_LIGHT, OUTPUT);
  pinMode(RED_LIGHT, OUTPUT);
  pinMode(GREEN_LIGHT, OUTPUT);
  
  startBlink();
  Serial.begin(9600);
}

USE_EVENTUALLY_LOOP(mgr);     // Use event loop instead of regular loop
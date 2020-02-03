/* Copyright 2020, nickthecook, GNU GPL v3 */

#include "Keyboard.h"

#define NUM_BTNS 5
// the index of the toggle switch; we need to treat it a little differently; see below
#define TOGGLE_IDX 4

// the current state of buttons
int state[NUM_BTNS] = {HIGH, HIGH, HIGH, HIGH, HIGH};
// the previous state, so we can check if anything changed
int previous_state[NUM_BTNS] = {HIGH, HIGH, HIGH, HIGH, HIGH};
// the gpio pins to use; these are the five on the Micro that support interrupts
int pins[NUM_BTNS] = {0, 1, 2, 3, 7};
// for the interrupt handler to tell the loop that something was pressed
volatile int pressed = 0;

void setup() {
  for (int i=0; i<NUM_BTNS; i++) {
    pinMode(pins[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pins[i]), hdlr_press, CHANGE);
  }

  Keyboard.begin();
}

void loop() {
  if (pressed) {
    // debounce
    delay(50);
    pressed = 0;

    copy_previous_state();
    get_current_state();

    for (int i=0; i<NUM_BTNS; i++) {
      // handle a state change in either direction for the toggle switch
      if (i == TOGGLE_IDX && previous_state[i] == LOW && state[i] == HIGH)
        command(NUM_BTNS);
      // a button press makes a pin go from HIGH to LOW
      else if (previous_state[i] == HIGH && state[i] == LOW)
        command(i);
    }
  }
}

void hdlr_press() {
  pressed = 1;
}

void copy_previous_state() {
  for (int i=0; i<NUM_BTNS; i++)
    previous_state[i] = state[i];
}

void get_current_state() {
  for (int i=0; i<NUM_BTNS; i++)
    state[i] = digitalRead(pins[i]);
}

void command(int cmdidx) {
  Keyboard.press(KEY_RIGHT_SHIFT);
  switch (cmdidx) {
    case 0:
      Keyboard.press('a');
      break;
    case 1:
      Keyboard.press('b');
      break;
    case 2:
      Keyboard.press('c');
      break;
    case 3:
      Keyboard.press('d');
      break;
    case 4:
      Keyboard.press('e');
      break;
    case 5:
      // this is the case for toggle switch off; using the same key allows
      // using this switch as a toggle in a game
      Keyboard.press('e');
      break;
  }
  delay(100);
  Keyboard.releaseAll();
}

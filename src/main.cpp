#include <Arduino.h>

#define BOOSTER 10
// #define SWITCH_LEFT 22
#define SWITCH_LEFT 4
// #define SWITCH_LEFT 14
// #define SWITCH_RIGHT 21
// #define SWITCH_RIGHT 5
#define SWITCH_RIGHT 14
#define TRANSISTOR_LEFT 7
#define TRANSISTOR_RIGHT 8

#define BLINK_INTERVAL 500
#define DEBOUNCE_TIME 50

enum BlinkState
{
  OFF,
  LEFT,
  RIGHT
};

String printState(BlinkState current_state)
{
  if (current_state == OFF)
  {
    return "OFF";
  }
  if (current_state == LEFT)
  {
    return "LEFT";
  }
  if (current_state == RIGHT)
  {
    return "RIGHT";
  }
  return "ERROR";
}

BlinkState current_state = OFF;
bool blink_on = false;
unsigned long last_blink = 0;
unsigned long last_left_press = 0;
unsigned long last_right_press = 0;
int last_left_state = HIGH;
int last_right_state = HIGH;

void set_outputs(bool doBlink)
{
  if (doBlink)
  {
    digitalWrite(BOOSTER, HIGH);
  }
  else
  {
    digitalWrite(BOOSTER, LOW);
  }

  Serial.println("Current State:" + printState(current_state));
  digitalWrite(TRANSISTOR_LEFT, (current_state == LEFT && doBlink) ? HIGH : LOW);
  digitalWrite(TRANSISTOR_RIGHT, (current_state == RIGHT && doBlink) ? HIGH : LOW);
}

void toggle_state(BlinkState target)
{
  current_state = (current_state == target) ? OFF : target;
  last_blink = millis();
  set_outputs(current_state != OFF);
}

void update_buttons()
{
  int left = digitalRead(SWITCH_LEFT);
  int right = digitalRead(SWITCH_RIGHT);

  // Falling-Edge HIGH -> LOW = Tastendruck
  if (left == LOW && last_left_state == HIGH &&
      millis() - last_left_press > DEBOUNCE_TIME)
  {
    Serial.println("Toggling Left");
    toggle_state(LEFT);
    last_left_press = millis();
  }
  last_left_state = left;

  if (right == LOW && last_right_state == HIGH &&
      millis() - last_right_press > DEBOUNCE_TIME)
  {
    Serial.println("Toggling Right");
    toggle_state(RIGHT);
    last_right_press = millis();
  }
  last_right_state = right;
}

void update_blink()
{
  if (current_state == OFF)
    return;

  if (millis() - last_blink >= BLINK_INTERVAL)
  {
    Serial.println("Blinking" + printState(current_state));
    // last_blink = millis();
    set_outputs(false);
  }
  if (millis() - last_blink >= BLINK_INTERVAL * 2)
  {
    Serial.println("Blinking" + printState(current_state));
    last_blink = millis();
    set_outputs(true);
  }
}

void setup()
{
  pinMode(SWITCH_LEFT, INPUT_PULLUP);
  pinMode(SWITCH_RIGHT, INPUT_PULLUP);
  pinMode(TRANSISTOR_LEFT, OUTPUT);
  pinMode(TRANSISTOR_RIGHT, OUTPUT);
  pinMode(BOOSTER, OUTPUT);

  // Boost-Converter 3.3V -> 12V

  digitalWrite(BOOSTER, LOW);

  digitalWrite(TRANSISTOR_LEFT, LOW);
  digitalWrite(TRANSISTOR_RIGHT, LOW);
}

void loop()
{
  update_buttons();
  update_blink();
  delay(10);
}
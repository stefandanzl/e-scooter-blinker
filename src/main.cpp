#include <Arduino.h>

#define BOOSTER 10
#define SWITCH_LEFT 7
#define SWITCH_RIGHT 6
#define TRANSISTOR_LEFT 9
#define TRANSISTOR_RIGHT 8

#define BLINK_INTERVAL 500
#define HOLD_TIME 80
// #define HOLD_TIME 300

enum BlinkState
{
  OFF,
  LEFT,
  RIGHT
};

String printState(BlinkState s)
{
  if (s == OFF)
    return "OFF";
  if (s == LEFT)
    return "LEFT";
  if (s == RIGHT)
    return "RIGHT";
  return "ERROR";
}

BlinkState current_state = OFF;
bool blink_on = false;
unsigned long last_blink = 0;

unsigned long left_low_since = 0;
unsigned long right_low_since = 0;
bool left_handled = false;
bool right_handled = false;

void set_outputs(bool doBlink)
{
  digitalWrite(BOOSTER, doBlink ? HIGH : LOW);
  digitalWrite(TRANSISTOR_LEFT, (current_state == LEFT && doBlink) ? HIGH : LOW);
  digitalWrite(TRANSISTOR_RIGHT, (current_state == RIGHT && doBlink) ? HIGH : LOW);
}

void toggle_state(BlinkState target)
{
  if (current_state == OFF)
  {
    current_state = target; // OFF → Ziel aktivieren
  }
  else
  {
    current_state = OFF; // Aktiv → cancel (egal welche Taste)
  }
  blink_on = (current_state != OFF);
  last_blink = millis();
  set_outputs(blink_on);
}

void update_buttons()
{
  int left = digitalRead(SWITCH_LEFT);
  int right = digitalRead(SWITCH_RIGHT);

  // LEFT
  if (left == LOW)
  {
    if (left_low_since == 0)
    {
      left_low_since = millis();
    }
    else if (!left_handled && millis() - left_low_since >= HOLD_TIME)
    {
      Serial.println("Toggle Left");
      toggle_state(LEFT);
      left_handled = true;
    }
  }
  else
  {
    left_low_since = 0;
    left_handled = false;
  }

  // RIGHT
  if (right == LOW)
  {
    if (right_low_since == 0)
    {
      right_low_since = millis();
    }
    else if (!right_handled && millis() - right_low_since >= HOLD_TIME)
    {
      Serial.println("Toggle Right");
      toggle_state(RIGHT);
      right_handled = true;
    }
  }
  else
  {
    right_low_since = 0;
    right_handled = false;
  }
}

void update_blink()
{
  if (current_state == OFF)
    return;

  if (millis() - last_blink >= BLINK_INTERVAL)
  {
    blink_on = !blink_on;
    last_blink = millis();
    set_outputs(blink_on);
  }
}

void setup()
{
  pinMode(SWITCH_LEFT, INPUT_PULLUP);
  pinMode(SWITCH_RIGHT, INPUT_PULLUP);
  pinMode(TRANSISTOR_LEFT, OUTPUT);
  pinMode(TRANSISTOR_RIGHT, OUTPUT);
  pinMode(BOOSTER, OUTPUT);

  digitalWrite(BOOSTER, LOW);
  digitalWrite(TRANSISTOR_LEFT, LOW);
  digitalWrite(TRANSISTOR_RIGHT, LOW);

  Serial.begin(115200);
}

void loop()
{
  update_buttons();
  update_blink();
  delay(10);
}
#include <Arduino.h>

#define PWM_PIN 10
#define SWITCH_LEFT 3
#define SWITCH_RIGHT 4
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

BlinkState current_state = OFF;
bool blink_on = false;
unsigned long last_blink = 0;
unsigned long last_left_press = 0;
unsigned long last_right_press = 0;
int last_left_state = HIGH;
int last_right_state = HIGH;

void set_outputs()
{
  digitalWrite(TRANSISTOR_LEFT, (current_state == LEFT && blink_on) ? HIGH : LOW);
  digitalWrite(TRANSISTOR_RIGHT, (current_state == RIGHT && blink_on) ? HIGH : LOW);
}

void toggle_state(BlinkState target)
{
  current_state = (current_state == target) ? OFF : target;
  blink_on = (current_state != OFF); // sofort an bei Aktivierung
  last_blink = millis();
  set_outputs();
}

void update_buttons()
{
  int left = digitalRead(SWITCH_LEFT);
  int right = digitalRead(SWITCH_RIGHT);

  // Falling-Edge HIGH -> LOW = Tastendruck
  if (left == LOW && last_left_state == HIGH &&
      millis() - last_left_press > DEBOUNCE_TIME)
  {
    toggle_state(LEFT);
    last_left_press = millis();
  }
  last_left_state = left;

  if (right == LOW && last_right_state == HIGH &&
      millis() - last_right_press > DEBOUNCE_TIME)
  {
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
    blink_on = !blink_on;
    last_blink = millis();
    set_outputs();
  }
}

void setup()
{
  pinMode(SWITCH_LEFT, INPUT_PULLUP);
  pinMode(SWITCH_RIGHT, INPUT_PULLUP);
  pinMode(TRANSISTOR_LEFT, OUTPUT);
  pinMode(TRANSISTOR_RIGHT, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);

  // Boost-Converter 3.3V -> 12V
  analogWriteFreq(50000);
  analogWriteRange(255);
  analogWrite(PWM_PIN, 250);

  digitalWrite(TRANSISTOR_LEFT, LOW);
  digitalWrite(TRANSISTOR_RIGHT, LOW);
}

void loop()
{
  update_buttons();
  update_blink();
  delay(10);
}
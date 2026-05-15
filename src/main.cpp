#include <Arduino.h>

#define PWM_PIN 10
#define SWITCH_LEFT 3
#define SWITCH_RIGHT 4
#define TRANSISTOR_LEFT 7
#define TRANSISTOR_RIGHT 8

#define BLINK_INTERVAL 500 // 500ms = 0,5s Frequenz

bool left_blink = false;
bool right_blink = false;
unsigned long last_left_press = 0;
unsigned long last_right_press = 0;
unsigned long last_blink = 0;
const unsigned long DEBOUNCE_TIME = 50;

enum BlinkState
{
  OFF,
  LEFT,
  RIGHT
};
BlinkState current_state = OFF;

void update_blink_state()
{
  int left = digitalRead(SWITCH_LEFT);
  int right = digitalRead(SWITCH_RIGHT);

  // Toggle Left
  if (left == LOW && millis() - last_left_press > DEBOUNCE_TIME)
  {
    current_state = (current_state == LEFT) ? OFF : LEFT;
    digitalWrite(TRANSISTOR_RIGHT, LOW);
    last_left_press = millis();
  }

  // Toggle Right
  if (right == LOW && millis() - last_right_press > DEBOUNCE_TIME)
  {
    current_state = (current_state == RIGHT) ? OFF : RIGHT;
    digitalWrite(TRANSISTOR_LEFT, LOW);
    last_right_press = millis();
  }
}

void blink()
{
  if (millis() - last_blink >= BLINK_INTERVAL)
  {
    if (current_state == LEFT)
    {
      digitalWrite(TRANSISTOR_LEFT, !digitalRead(TRANSISTOR_LEFT));
    }
    else if (current_state == RIGHT)
    {
      digitalWrite(TRANSISTOR_RIGHT, !digitalRead(TRANSISTOR_RIGHT));
    }
    else
    {
      digitalWrite(TRANSISTOR_LEFT, LOW);
      digitalWrite(TRANSISTOR_RIGHT, LOW);
    }
    last_blink = millis();
  }
}

void setup()
{
  pinMode(SWITCH_LEFT, INPUT_PULLUP);
  pinMode(SWITCH_RIGHT, INPUT_PULLUP);
  pinMode(TRANSISTOR_LEFT, OUTPUT);
  pinMode(TRANSISTOR_RIGHT, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);

  analogWriteFreq(50000);
  analogWriteRange(255);
  analogWrite(PWM_PIN, 250);

  digitalWrite(TRANSISTOR_LEFT, LOW);
  digitalWrite(TRANSISTOR_RIGHT, LOW);
}

void loop()
{
  update_blink_state();
  blink();
  delay(10);
}
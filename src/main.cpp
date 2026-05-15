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

void blink()
{
  if (millis() - last_blink >= BLINK_INTERVAL)
  {
    if (left_blink)
    {
      digitalWrite(TRANSISTOR_LEFT, !digitalRead(TRANSISTOR_LEFT));
    }
    if (right_blink)
    {
      digitalWrite(TRANSISTOR_RIGHT, !digitalRead(TRANSISTOR_RIGHT));
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
  int left = digitalRead(SWITCH_LEFT);
  int right = digitalRead(SWITCH_RIGHT);

  // Toggle Left
  if (left == LOW && millis() - last_left_press > DEBOUNCE_TIME)
  {
    left_blink = !left_blink;
    right_blink = false;
    digitalWrite(TRANSISTOR_LEFT, LOW);
    digitalWrite(TRANSISTOR_RIGHT, LOW);
    last_left_press = millis();
  }

  // Toggle Right
  if (right == LOW && millis() - last_right_press > DEBOUNCE_TIME)
  {
    right_blink = !right_blink;
    left_blink = false;
    digitalWrite(TRANSISTOR_LEFT, LOW);
    digitalWrite(TRANSISTOR_RIGHT, LOW);
    last_right_press = millis();
  }

  blink();
  delay(10);
}
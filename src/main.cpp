#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>
// #include <time.h>
#include <TinySoftwareSerial.h>

soft_ring_buffer buffer;

// TinySoftwareSerial mySerial(&buffer, 2, 1); // RX, TX
SoftwareSerial mySerial(2, 1); // RX, TX

unsigned long last_time = 0;
unsigned long INTERVAL = 1000; // 1 Sekunde
unsigned long BREAKER = 2;
unsigned long counter = 0;

void setup()
{
  mySerial.begin(9600);
}

void loop()
{
  if (mySerial.available())
  {
    char c = mySerial.read();

    if (c == '+')
    {
      INTERVAL += 100; // +100ms
      mySerial.print("Interval: ");
      mySerial.println(INTERVAL);
    }
    else if (c == '-')
    {
      if (INTERVAL > 100)
      {                  // Min 100ms
        INTERVAL -= 100; // -100ms
      }
      mySerial.print("Interval: ");
      mySerial.println(INTERVAL);
    }
  }

  unsigned long currentTime = millis();
  if (currentTime - last_time >= INTERVAL)
  {
    mySerial.println("Hello!");
    last_time = currentTime;
  }
}
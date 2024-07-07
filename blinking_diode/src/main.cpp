#include <Arduino.h>

int delayTime = 1000;

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED ON");
  delay(delayTime);

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED OFF");
  delay(delayTime);
}
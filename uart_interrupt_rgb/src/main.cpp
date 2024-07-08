#include <Arduino.h>

#define ERROR_DIODE A0

#define RED_DIODE D4   // OK
#define GREEN_DIODE D3 // OK
#define BLUE_DIODE D2  // OK

void setup()
{
  pinMode(ERROR_DIODE, OUTPUT);
  pinMode(BLUE_DIODE, OUTPUT);
  pinMode(GREEN_DIODE, OUTPUT);
  pinMode(RED_DIODE, OUTPUT);
}

void loop()
{
  digitalWrite(ERROR_DIODE, HIGH);

  analogWrite(BLUE_DIODE, 255);
  analogWrite(GREEN_DIODE, 255);
  analogWrite(RED_DIODE, 255);

  delay(1000);
  digitalWrite(ERROR_DIODE, LOW);

  analogWrite(BLUE_DIODE, 0);
  analogWrite(GREEN_DIODE, 0);
  analogWrite(RED_DIODE, 0);
  delay(1000);
}

#include <Arduino.h>

struct Button
{
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool isPressed;
  bool isOn;
  unsigned long lastDebounceTime;
  unsigned long debounceDelay;
};

Button button = {D4, 0, false, false, 0, 500};

void IRAM_ATTR isr()
{
  unsigned long currentTime = millis();

  if ((currentTime - button.lastDebounceTime) > button.debounceDelay)
  {

    button.numberKeyPresses++;
    button.isPressed = true;
    button.lastDebounceTime = currentTime;
  }
}

void setup()
{

  Serial.begin(9600);

  pinMode(LED_BLUE, OUTPUT);

  pinMode(button.PIN, INPUT_PULLUP);

  attachInterrupt(button.PIN, isr, FALLING);
}

void loop()
{

  if (button.isPressed)
  {
    Serial.printf("Button has been pressed %u times\n", button.numberKeyPresses);
    button.isPressed = false;

    if (digitalRead(LED_BLUE) == HIGH)
    {
      digitalWrite(LED_BLUE, LOW);
      button.isOn = false;
    }
    else
    {
      digitalWrite(LED_BLUE, HIGH);
      button.isOn = true;
    }
  }
}
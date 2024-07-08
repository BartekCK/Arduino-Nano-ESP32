#include <Arduino.h>
#include <HardwareSerial.h>

#define ERROR_DIODE A0

#define RED_DIODE D4   // OK
#define GREEN_DIODE D3 // OK
#define BLUE_DIODE D2  // OK

// Global variables
volatile bool ledState = false;
hw_timer_t *timer = NULL;

// Function prototypes
void IRAM_ATTR onUartReceive();
void IRAM_ATTR onTimer();

void setup()
{
  Serial.begin(9600);

  pinMode(ERROR_DIODE, OUTPUT);
  pinMode(BLUE_DIODE, OUTPUT);
  pinMode(GREEN_DIODE, OUTPUT);
  pinMode(RED_DIODE, OUTPUT);

  Serial1.begin(115200, SERIAL_8N1, D11, D12);
  Serial1.onReceive(onUartReceive);

  timer = timerBegin(0, 80, true); // 80 prescaler => 1 tick = 1 microsecond
  timerAttachInterrupt(timer, &onTimer, true);

  Serial.println("Lights initialised...");
}

void loop()
{
  analogWrite(BLUE_DIODE, 255);
  analogWrite(GREEN_DIODE, 255);
  analogWrite(RED_DIODE, 255);

  delay(1000);

  analogWrite(BLUE_DIODE, 0);
  analogWrite(GREEN_DIODE, 0);
  analogWrite(RED_DIODE, 0);
  delay(1000);
}

void IRAM_ATTR onUartReceive()
{
  // Clear UART receive interrupt flag
  Serial1.read(); // Clear the receive buffer (assuming a message is received)

  // Turn on the LED
  digitalWrite(ERROR_DIODE, HIGH);
  ledState = true;

  // Start the timer for 3 seconds (3000000 microseconds)
  timerWrite(timer, 0);                   // Reset timer counter
  timerAlarmWrite(timer, 3000000, false); // Set alarm to 3 seconds (non-repeating)
  timerAlarmEnable(timer);                // Enable the timer alarm
}

void IRAM_ATTR onTimer()
{
  // Turn off the LED
  digitalWrite(ERROR_DIODE, LOW);
  ledState = false;

  // Disable the timer alarm
  timerAlarmDisable(timer);
}
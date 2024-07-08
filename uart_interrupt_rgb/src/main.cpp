#include <Arduino.h>
#include <HardwareSerial.h>

#define ERROR_DIODE A0

#define RED_DIODE D4   // OK
#define GREEN_DIODE D3 // OK
#define BLUE_DIODE D2  // OK

// Global variables

hw_timer_t *timer = NULL;
volatile bool messageReceived = false;
String receivedMessage;

// Function prototypes
void IRAM_ATTR onUartReceive();
void IRAM_ATTR onTimer();

void setup()
{
  Serial.begin(9600);

  while (!Serial)
  {
    ; // wait for serial port to connect.
  }

  pinMode(ERROR_DIODE, OUTPUT);
  pinMode(BLUE_DIODE, OUTPUT);
  pinMode(GREEN_DIODE, OUTPUT);
  pinMode(RED_DIODE, OUTPUT);

  Serial1.begin(115200, SERIAL_8N1, D11, D12);
  Serial1.onReceive(onUartReceive);

  timer = timerBegin(0, 80, true); // 80 prescaler => 1 tick = 1 microsecond
  timerAttachInterrupt(timer, &onTimer, true);

  Serial.println("All initialised...");
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

  if (messageReceived)
  {
    noInterrupts();

    Serial.print("Received message: ");
    Serial.println(receivedMessage);

    receivedMessage = "";
    messageReceived = false;

    interrupts();
  }
}

void IRAM_ATTR onUartReceive()
{

  while (Serial1.available())
  {
    char c = (char)Serial1.read();

    receivedMessage += c;

    if (c == '\n')
    {
      break; // Exit the while loop when a full message is received
    }
  }

  messageReceived = true;

  // Turn on the LED
  digitalWrite(ERROR_DIODE, HIGH);

  // Start the timer for 3 seconds (3000000 microseconds)
  timerWrite(timer, 0);                   // Reset timer counter
  timerAlarmWrite(timer, 3000000, false); // Set alarm to 3 seconds (non-repeating)
  timerAlarmEnable(timer);                // Enable the timer alarm
}

void IRAM_ATTR onTimer()
{

  digitalWrite(ERROR_DIODE, LOW);

  // Disable the timer alarm
  timerAlarmDisable(timer);
}
bool stringComplete = false;
String msg = "";

int LED = 2;                       // pin number of LED
bool ledState = true;              // LED init state
int LASER = 3;                     // pin number of LASER
bool laserState = false;           // laser init state
bool pulse = false;                // LED pulsing state
bool tog = true;                   // toggle switch for LEDs
int ledPulseRate_ms = 100;         // rate between LED pulses (ms)
unsigned long ledToggle_min = 720; // toggle LEDs every x minutes
unsigned long previousMillis = 0;  // millis() returns an unsigned long.

void setup()
{
  pinMode(LED, OUTPUT);            // digital pin 2 as output
  pinMode(LASER, OUTPUT);          // digital pin 3 as output
  Serial.begin(115200);            // connect to serial port
  digitalWrite(LED, ledState);     // LED state at init
  digitalWrite(LASER, laserState); // Laser state at init
}

void loop()
{
  // Schedule LEDs on/off
  unsigned long currentMillis = millis(); // grab current time
  // check if "interval" time has passed (1000 milliseconds)
  if ((unsigned long)(currentMillis - previousMillis) >= (ledToggle_min * 60 * 1000))
  {
    tog = !tog;                // "toggles" the state
    digitalWrite(LED, tog);    // sets the LED based on toggle
    previousMillis = millis(); // save the "current" time
  }

  if (pulse)
  { // pulse condition
    digitalWrite(LED, HIGH);
    delay(ledPulseRate_ms);
    digitalWrite(LED, LOW);
    delay(ledPulseRate_ms);
  }
  if (stringComplete)
  {

    stringComplete = false;

    if (msg.indexOf("leds") >= 0)
    { // toggle LEDs
      pulse = false;
      ledState = !digitalRead(LED);
      digitalWrite(LED, ledState);
      msg = ledState ? "on" : "off";
      Serial.print("LEDs are ");
      Serial.println(msg);
    }
    else if (msg.indexOf("laser") >= 0)
    { // toggle LASER
      laserState = !digitalRead(LASER);
      digitalWrite(LASER, laserState);
      msg = laserState ? "on" : "off";
      Serial.print("Laser is ");
      Serial.println(msg);
    }
    else if (msg.indexOf("pulse") >= 0)
    { // pulse LEDs
      char rate = msg.charAt(5);
      switch (rate)
      {
      case '1':
        ledPulseRate_ms = 20;
        pulse = true;
        break;
      case '2':
        ledPulseRate_ms = 50;
        pulse = true;
        break;
      case '3':
        ledPulseRate_ms = 200;
        pulse = true;
        break;
      case '4':
        ledPulseRate_ms = 500;
        pulse = true;
        break;
      case '5':
        ledPulseRate_ms = 1000;
        pulse = true;
        break;
      default:
        ledPulseRate_ms = 100;
        pulse = false;
        break;
      }
      msg = pulse ? "pulsing" : "off";
      Serial.print("LEDs are ");
      Serial.println(msg);
    }
    else if (msg.indexOf("status") >= 0)
    { // status info
      Serial.print("timer counts (ms): ");
      Serial.println(currentMillis, DEC);
      msg = tog ? "off" : "on";
      int timeLeft = ledToggle_min - ((currentMillis - previousMillis) / 1000 / 60);
      int hours = timeLeft / 60;
      int min = timeLeft % 60;
      Serial.print("LEDs " + msg + " in ");
      Serial.print(hours, DEC);
      Serial.print(" hours and ");
      Serial.print(min, DEC);
      Serial.println(" min");
      Serial.print("pulse rate (ms): ");
      Serial.println(ledPulseRate_ms, DEC);
      msg = ledState ? "on" : "off";
      Serial.println("LEDs are " + msg);
      msg = laserState ? "on" : "off";
      Serial.println("Laser is " + msg);
    }
    else
    {
      Serial.println("not a valid command");
    }
  }
  msg = "";
  delay(50);
}

void serialEvent()
{ // receives msg from user input
  while (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    msg += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
      msg.trim();
    }
  }
}
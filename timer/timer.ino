bool stringComplete = false;
String msg = "";

int LED = 2;                       // pin number of LED
bool ledState = true;              // LED init state
bool tog = true;                   // toggle switch for LEDs
unsigned long ledToggle_min = 0.2; // toggle LEDs every x minutes
unsigned long previousMillis = 0;  // millis() returns an unsigned long.

void setup()
{
  pinMode(LED, OUTPUT);            // digital pin 2 as output
  Serial.begin(9600);            // connect to serial port
  digitalWrite(LED, ledState);     // LED state at init
}

void loop()
{
  // Schedule LEDs on/off
  unsigned long currentMillis = millis(); // grab current time
  // check if "interval" time has passed
  if ((unsigned long)(currentMillis - previousMillis) >= (ledToggle_min * 60 * 1000))
  {
    tog = !tog;                // "toggles" the state
    digitalWrite(LED, tog);    // sets the LED based on toggle
    previousMillis = millis(); // save the "current" time
  }

  if (stringComplete)
  {

    stringComplete = false;
    
    if (msg.indexOf("leds") >= 0)
    { // toggle LEDs
      ledState = !digitalRead(LED);
      digitalWrite(LED, ledState);
      msg = ledState ? "on" : "off";
      Serial.print("LEDs are ");
      Serial.println(msg);
    }
    else if (msg.indexOf("ledoff") >= 0)
    { // force LEDs off
      digitalWrite(LED, false);
      ledState = digitalRead(LED);
      msg = ledState ? "on" : "off";
      Serial.print("LEDs are ");
      Serial.println(msg);
    }
    else if (msg.indexOf("ledon") >= 0)
    { // force LEDs on
      digitalWrite(LED, true);
      ledState = digitalRead(LED);
      msg = ledState ? "on" : "off";
      Serial.print("LEDs are ");
      Serial.println(msg);
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
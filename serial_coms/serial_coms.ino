int led = 2; //pin number of led
int laser = 3;//pin number of laser
bool pulse = false; //led pulsing state
int pulseRate = 200; //rate between LED pulses (ms)

void setup(){
  pinMode(led, OUTPUT); //digital pin 2 as output
  pinMode(laser, OUTPUT); //digital pin 3 as output
  digitalWrite(led, HIGH); //LED on at init
  Serial.begin(9600);
}

void loop(){
  if(pulse){ //pulse condition
    digitalWrite(led, HIGH);
    delay(pulseRate);
    digitalWrite(led, LOW);
    delay(pulseRate);
  }
  if(Serial.available() > 0){
    String msg = Serial.readString();
    if(msg == "leds"){ //toggle LEDs
      pulse = false;
      int state =  !digitalRead(led);
      msg = state? "LEDs are on" : "LEDs are off";
      digitalWrite(led, state);
    }
    else if(msg == "laser"){ //toggle laser
      int state =  !digitalRead(laser);
      msg = state? "Laser is on" : "Laser is off";
      digitalWrite(laser, state);
    }
    else if(msg == "pulse"){ //pulse LEDs
      pulse = true;
      Serial.println("LEDs are pulsing");
    }
    else{
      Serial.println("not a valid command");
    }
    Serial.println(msg);
  }
}

int led = 2; //pin number of output
bool pulse = false; //led pulsing state
int pulseRate = 200; //rate between LED pulses (ms)

void setup(){
  pinMode(led, OUTPUT); //digital pin 2 as output
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
    pulse = false;
    String msg = Serial.readString();
    if(msg == "on"){ //turn on LEDs
      digitalWrite(led, HIGH);
      Serial.println("LEDs are now on");
    }
    else if(msg == "off"){ //turn off LEDs
      digitalWrite(led, LOW);
      Serial.println("LEDs are now off");
    }
    else if(msg == "pulse"){ //pulse LEDs
      pulse = true;
      Serial.println("LEDs are pulsing");
    }
    else{
      Serial.println("not a valid command");
    }
  }
}
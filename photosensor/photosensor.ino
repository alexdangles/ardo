// Functionaliy _____________________________________________________________________________
// Incrementally change the light intensity of each RGB LED lead from 0 to 255 in 5 unit increments. 
// Capture a photoresistor data point at each increment indicating the LED lead (e.g. “R”), 
// the intensity value (e.g. “100”), and the photoresistor datapoint (e.g. “405”)
// Author: A. D'Angelo      Oct 28, 2023
//________________________________________________________________________________________
// Constants
const byte ledPins[3] = {9, 10, 11}; // LED at digital pins 9, 10 and 11 are all PWM compitable
const int pResistor = A0; // Photocell at analog pin A0
const char rgb[3] = {'R', 'G', 'B'}; // Characters for use in output data

// Variables
int pValue = 0; // Photocell value 0-1023
int pNum = 0; // LED pin index from rgb[] array

void setup() { // Setup serial port and pin operation modes
    Serial.begin(115200);
    pinMode(pResistor, INPUT);
    for(int i = 0; i <= 2; i++){
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop() { // Cycle through each color, increment intensity by 0-255 by 5 units
  for(int i = 0; i <= 255; i = i + 5){
        analogWrite(ledPins[pNum], i);
        delay(500);
        pValue = analogRead(pResistor);
        // Output color, index and photocell value to serial port (.csv format)
        Serial.print(rgb[pNum]);
        Serial.print(", ");
        Serial.print(i);
        Serial.print(", ");
        Serial.println(pValue);
    }
    // Set back to off state and begin next color cycle
    analogWrite(ledPins[pNum], 0);
    delay(10000);
    pNum = pNum==2? 0 : pNum + 1;
}
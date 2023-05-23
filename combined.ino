#include <FastLED.h>

#define LEDS1_PIN 6
#define LEDS2_PIN 7
#define LED_COUNT 34

CRGB leds1[LED_COUNT];
CRGB leds2[LED_COUNT];

// Pin definitions for 6 linear Hall sensors
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // SoftwareSerial object to communicate with HC-05 module

const int SENSOR1_PIN = A0;
const int SENSOR2_PIN = A1;
const int SENSOR3_PIN = A2;
const int SENSOR4_PIN = A3;
const int SENSOR5_PIN = A4;
//const int SENSOR6_PIN = A5;

// Variables to store the sensor values
int sensor1Value;
int sensor2Value;
int sensor3Value;
int sensor4Value;
int sensor5Value;
//int sensor6Value;

// Variables to store the previous sensor states
int previousSensor1State = 0;
int previousSensor2State = 0;
int previousSensor3State = 0;
int previousSensor4State = 0;
int previousSensor5State = 0;
//int previousSensor6State = 0;

int sensorPin = A5;   // Define analog input pin for the sensor
int sensorValue;      // Variable to store the sensor value
int magnetThreshold = 500;
int magnetDetected = 0; // Variable to track magnet detection
int previouslydetected = 0;

#include <string.h>

// Declare struct
struct MessageValue {
  String message;
  String value;
};

// Declare function that parse message format
struct MessageValue getMessage(String inputtedStr) {
  struct MessageValue result;

  char charArr[50];
  inputtedStr.toCharArray(charArr, 50);
  char* ptr = strtok(charArr, "||");
  result.message = String(ptr);
  ptr = strtok(NULL, "||");

  if (ptr == NULL) {
    result.value = String("");
    return result;
  }

  result.value = String(ptr);

  return result;
}

// Declare MessageValue struct's instance
struct MessageValue receivedData;

int aangezet = 0;
int verbonden = 0;
int tracker = 0;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize the serial communication for debugging
  Serial.begin(9600);
  BTSerial.begin(9600); // Initialize software serial communication for HC-05 module

  // Set the pins as inputs
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);
  pinMode(SENSOR3_PIN, INPUT);
  pinMode(SENSOR4_PIN, INPUT);
  pinMode(SENSOR5_PIN, INPUT);
  //pinMode(SENSOR6_PIN, INPUT);
  
  Serial.setTimeout(10);

  FastLED.addLeds<WS2812B, LEDS1_PIN, GRB>(leds1, LED_COUNT);
  FastLED.addLeds<WS2812B, LEDS2_PIN, GRB>(leds2, LED_COUNT);

  
  //standby -> gedimd wit
  while(aangezet == 0){
    on(5, 4, 0);
    pauze(20);
  }
  
  //*aanzetten* -> trage witte pulse (3tal keer, daarna automatisch naar volgende stap)
  pulse(3, 3, 0);
  //zoeken naar bluetooth -> sonar blauw
  while(verbonden == 0){
    sonar(1, 60, 1);
  }
  //Als verbinden gelukt is -> snel pulsen groen
  pulse(3, 10, 2); 
}

void loop() {
  //standby -> gedimd wit
  on(5, 4, 0);
  while(tracker == 0){
    pauze(20); 
  }
  off(100, 0);

  //Als tracker x gepakt moet worden -> locate (tot tracker weggenomen is)
  if(tracker==1){
    while(previousSensor1State == 1){
      locate(1, 60, 1);
    }
  }
  if(tracker==2){
    while(previousSensor2State == 1){
      locate(1, 60, 2);
    }

  }

  if(tracker==3){
    while(previousSensor3State == 1){
      locate(1, 60, 3);
    }
  }
  if(tracker==4){
    while(previousSensor4State == 1){
      locate(1, 60, 4);
    }
  }
  if(tracker==5){
    while(previousSensor5State == 1){
      locate(1, 60, 5);
    }
  }
  on(5, 4, 0);
  while (tracker != 0){
    pauze(20);
  }
  //pauze(20);
}

  /* // blauwe sonar, 1 rotation per 2 seconds
  
  
  locate(5, 60, 2);
  */

  /* 
  codes combineren

  standby -> gedimd wit

  *aanzetten* -> trage witte pulse (5tal keer, daarna automatisch naar volgende stap)
  zoeken naar bluetooth -> sonar blauw
  Als verbinden gelukt is -> snel pulsen groen

  Als tracker x gepakt moet worden -> locate (tot tracker weggenomen is)
  als tracker weggenomen is -> melding naar protopie
  */

void on(int increment, int brightness0, int color){
  // Gradually increase brightness
  if (increment < brightness0 + 1){
    for (int brightness = 0; brightness < brightness0; brightness+=increment) {
      for (int i = 1; i < LED_COUNT; i++) {
        updateledcolor(brightness, i, color);
      } 
      FastLED.show();
      pauze(30);
    }
  }
  else{
    for (int i = 1; i < LED_COUNT; i++) {
        updateledcolor(brightness0, i, color);
      }
      FastLED.show();
  }
}

void off(int increment, int color){
  // Gradually decrease brightness
  if (increment<100){
    for (int brightness = 100; brightness > -1; brightness-=increment) {
      for (int i = 0; i < LED_COUNT; i++) {
        updateledcolor(brightness, i, color);
      }
      FastLED.show();
      pauze(30);
    }
  }
  else{
    for (int i = 0; i < LED_COUNT; i++) {
        updateledcolor(0, i, color);
      }
      FastLED.show();
  }
}

void pulse(int repeat, int increment, int color){
  for (int z = 0; z < repeat; z++){
    on(increment, 80, color);
    off(increment, color);
  }
}

void sonar(int repeat, int pause, int color){ 
  for(int e = 0; e < repeat; e++){
    // problemen bij activeled 1-4
    int wave[] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    /*
    Different waves, replace array to change waves:

    1 wave, 6 leds
    {90, 75, 60, 45, 30, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    1 wave, 10 leds
    {100, 90, 80, 70, 60, 50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    3 waves, 6 leds
    {90, 75, 60, 45, 30, 15, 0, 0, 0, 0, 0, 90, 75, 60, 45, 30, 15, 0, 0, 0, 0, 0, 90, 75, 60, 45, 30, 15, 0, 0, 0, 0, 0};
    */
    for (int activeled = 1; activeled < LED_COUNT; activeled++) {
      for (int i = 1; i < LED_COUNT; i++) {

            int index = activeled + i;
            if(index > 33){
              index = index - 33;
            }
            int brightness = wave[index];
              
            updateledcolor(brightness, i, color);
          }
          FastLED.show();
          pauze(pause);
    }
  }
}

void locate(int repeat, int pause, int opening){ //werkt nog niet met colors!
  int strip = 0;
  
  if(opening == 1 or opening == 4){
    strip = 2;
    Serial.println("strip|| 2");
  }
  else{
    strip = 1;
    Serial.println("strip|| 1");
  }
  
  //int locations[] = {7, 18, 29};
  int locations[] = {0, 29, 29, 7, 7, 18};
  int location = locations[opening];
  int wave[] = {100, 80, 60, 40, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  //int actualledcw[] = {24, 2, 13};
  //int actualledacw[] = {42, 20, 31};
  int actualledcw[] = {0, 13, 13, 24, 24, 2};
  int actualledacw[] = {0, 31, 31, 42, 42, 20};
  
  for(int e = 0; e < repeat; e++){
    // alle leds af, behalve de 3 boven een opening
    off(100, 0);
    for (int ledaan = location-1; ledaan < location + 2; ledaan++){
      updateled(100, ledaan, strip);
    }
    FastLED.show();
    
    //wave effect 
    for (int activeled = 0; activeled < 11; activeled++) {
      for (int p = 14; p > -1; p--) {    
        int index = activeled + (14-p) + 4; // laatste getal wijzigt waar wave begint bij geselecteerde leds
        if(index > 14){
          index = index - 14;
        }
        int brightness = wave[index];  
        int actualled = p + actualledcw[opening]; //getal wijzigt welke leds geselecteerd worden
        while(actualled > 33){
          actualled = actualled - 33;
        }
        updateled(brightness, actualled, strip); 
      }
      for (int m = 14; m > -1; m--) {  
        int index = activeled + m + 4;
        if(index > 14){
          index = index - 14;
        }
        int brightness = wave[index];  
        int actualled = m + actualledacw[opening];
        while(actualled > 33){
          actualled = actualled - 33;
        }  
        updateled(brightness, actualled, strip);
      }
      FastLED.show();
      pauze(pause);
    }
    pauze(300);
  }
}

void locatecolor(int repeat, int pause, int opening, int color){ //werkt nog niet met colors!
  int locations[] = {7, 18, 29};
  int location = locations[opening];
  int wave[] = {100, 80, 60, 40, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int actualledcw[] = {24, 2, 13};
  int actualledacw[] = {42, 20, 31};
  
  for(int e = 0; e < repeat; e++){
    // alle leds af, behalve de 3 boven een opening
    off(100, color);
    for (int ledaan = location-1; ledaan < location + 2; ledaan++){
      updateledcolor(100, ledaan, color);
    }
    FastLED.show();
    pauze(500);
    
    //wave effect 
    for (int activeled = 0; activeled < 11; activeled++) {
      for (int i = 14; i > -1; i--) {    
        int index = activeled + (14-i);
        if(index > 14){
          index = index - 14;
        }
        int brightness = wave[index+4];  //getal wijzigt waar wave begint bij geselecteerde leds
        int actualled = i + actualledcw[opening]; //getal wijzigt welke leds geselecteerd worden
        while(actualled > 33){
          actualled = actualled - 33;
        }
        updateledcolor(brightness, actualled, color); 
      }
      for (int i = 14; i > -1; i--) {  
        int index = activeled + i;
        if(index > 14){
          index = index - 14;
        }
        int brightness = wave[index+4];  
        int actualled = i + actualledacw[opening];
        while(actualled > 33){
          actualled = actualled - 33;
        }  
        updateledcolor(brightness, actualled, color);
      }
      FastLED.show();
      pauze(pause);
    }
  }
}

void updateled(int brightness, int i, int strip){
  int kleurfactor[] = {204, 204, 204}; //rgb 0-255
  if(strip == 1 or strip == 0){ //onderste leds
    leds1[i].r = brightness * kleurfactor[0] / 100; 
    leds1[i].g = brightness * kleurfactor[1] / 100;
    leds1[i].b = brightness * kleurfactor[2] / 100;
  }
  if(strip == 2 or strip == 0){ //bovenste leds
    leds2[i].r = brightness * kleurfactor[0] / 100;
    leds2[i].g = brightness * kleurfactor[1] / 100;
    leds2[i].b = brightness * kleurfactor[2] / 100;  
  }
}

void updateledcolor(int brightness, int i, int color){

  int kleurfactor[3][3] = {{204, 204, 204},{0, 0, 255}, {0, 255, 0}}; //0white, 1blue, 2groen

  leds1[i].r = brightness * kleurfactor[color][0] / 100;
  leds1[i].g = brightness * kleurfactor[color][1] / 100;
  leds1[i].b = brightness * kleurfactor[color][2] / 100;
  leds2[i].r = brightness * kleurfactor[color][0] / 100;
  leds2[i].g = brightness * kleurfactor[color][1] / 100;
  leds2[i].b = brightness * kleurfactor[color][2] / 100;
}

void sensor(){
  	// Take out strings until Serial buffer is empty
	while (Serial.available() > 0) {
    // From ProtoPie Connect 1.9.0, We can use '\0' as delimiter in Arduino Serial
		String receivedString = Serial.readStringUntil('\0'); 
	
		receivedData = getMessage(receivedString);
  }
   int sensorValue = analogRead(sensorPin); // Read hall sensor value
    // Check if a magnet is near each sensor and send a signal to Protopie Connect pairing
 if (sensorValue < 450) {
  if (previouslydetected == 0) {
    Serial.println("paired||1");
    BTSerial.println("Pairing||1");
    // send signal to Protopie Connect for Sensor 1 with value of 1
  }
  previouslydetected = 1;
  } 

  
  // Read the sensor values trackers
  sensor1Value = analogRead(SENSOR1_PIN);
  sensor2Value = analogRead(SENSOR2_PIN);
  sensor3Value = analogRead(SENSOR3_PIN);
  sensor4Value = analogRead(SENSOR4_PIN);
  sensor5Value = analogRead(SENSOR5_PIN);
  //sensor6Value = analogRead(SENSOR6_PIN);

  // Check if a magnet is near each sensor and send a signal to Protopie Connect
 if (sensor1Value < 50) {
  if (previousSensor1State == 0) {
    Serial.println("Sensor 1||1");
    BTSerial.println("Sensor 1||1");
    // send signal to Protopie Connect for Sensor 1 with value of 1
  }
  previousSensor1State = 1;
  } else {
    if (previousSensor1State == 1) {
      Serial.println("Sensor 1||0");
      BTSerial.println("Sensor 1||0");
      // send signal to Protopie Connect for Sensor 1 with value of 0
    }
    previousSensor1State = 0;
  }

  if (sensor2Value < 50) {
    if (previousSensor2State == 0) {
      Serial.println("Sensor 2||1");
      BTSerial.println("Sensor 2||1");
      // send signal to Protopie Connect for Sensor 1 with value of 1
    }
    previousSensor2State = 1;
  } else {
    if (previousSensor2State == 1) {
      Serial.println("Sensor 2||0");
      BTSerial.println("Sensor 2||0");
      // send signal to Protopie Connect for Sensor 1 with value of 0
    }
    previousSensor2State = 0;
  }

  if (sensor3Value < 50) {
    if (previousSensor3State == 0) {
      Serial.println("Sensor 3||1");
      BTSerial.println("Sensor 3||1");
      // send signal to Protopie Connect for Sensor 1 with value of 1
    }
    previousSensor3State = 1;
  } else {
    if (previousSensor3State == 1) {
      Serial.println("Sensor 3||0");
      BTSerial.println("Sensor 3||0");
      // send signal to Protopie Connect for Sensor 1 with value of 0
    }
    previousSensor3State = 0;
  }

  if (sensor4Value < 50) {
    if (previousSensor4State == 0) {
      Serial.println("Sensor 4||1");
      BTSerial.println("Sensor 4||1");
      // send signal to Protopie Connect for Sensor 1 with value of 1
    }
    previousSensor4State = 1;
  } else {
    if (previousSensor4State == 1) {
      Serial.println("Sensor 4||0");
      BTSerial.println("Sensor 4||0");
      // send signal to Protopie Connect for Sensor 1 with value of 0
    }
    previousSensor4State = 0;
  }

  if (sensor5Value < 50) {
    if (previousSensor5State == 0) {
      Serial.println("Sensor 5||1");
      BTSerial.println("Sensor 5||1");
      // send signal to Protopie Connect for Sensor 1 with value of 1
    }
    previousSensor5State = 1;
  } else {
    if (previousSensor5State == 1) {
      Serial.println("Sensor 5||0");
    BTSerial.println("Sensor 5||0");
      // send signal to Protopie Connect for Sensor 1 with value of 0
    }
    previousSensor5State = 0;
  }
}

void pauze(int time){
  for(int i=0; i<time; i=i+10){
    delay(10);
    sensor();
    readdata();
  }
}

void readdata(){
  while (Serial.available() > 0) { // Take out strings until Serial is empty
    String receivedString = Serial.readStringUntil('\0'); // From 1.9.0 version, We can use '\0' as delimiter in Arduino Serial
    receivedData = getMessage(receivedString);
  }
  while (BTSerial.available() > 0) { // Take out strings until Serial is empty
    String receivedString = BTSerial.readStringUntil('\0'); // From 1.9.0 version, We can use '\0' as delimiter in Arduino Serial
    receivedData = getMessage(receivedString);
  }

  if (receivedData.message.equals("aangezet")) {
    aangezet = receivedData.value.toInt();
  } else if (receivedData.message.equals("verbonden")) {
    verbonden = receivedData.value.toInt();
  } else if (receivedData.message.equals("tracker")) {
    tracker = receivedData.value.toInt();
  } 
}
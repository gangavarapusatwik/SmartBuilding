#include <WiFi.h>

const char* wifi_name = "saat"; // Your Wifi network name here
const char* wifi_pass = "maximaxi";    // Your Wifi network password here


#define timeSeconds 60
const int relay_pin4 = 2;
const int relay_pin3 = 0;
const int relay_pin2 = 4;
const int relay_pin1 = 15;
const int motionSensor = 22;
const int acs712=13;

long lastSample=0;
long sampleSum=0;
int sampleCount=0;

float vpc=4.8828125;

unsigned long now = millis();
unsigned long lastTrigger = 0;
bool relay=true;
//boolean startTimer = false;


void setup() {
  Serial.begin (115200);
  pinMode (relay_pin4, OUTPUT);
  pinMode (relay_pin3, OUTPUT);
  pinMode (relay_pin2, OUTPUT);
  pinMode (relay_pin1, OUTPUT);
  pinMode(motionSensor, INPUT_PULLUP); 

  Serial.print ("Connecting to ");
  Serial.print (wifi_name);
  WiFi.begin (wifi_name, wifi_pass);     // Connecting to the wifi network

  while (WiFi.status() != WL_CONNECTED) // Waiting for the response of wifi network
  {
    delay (500);
    Serial.print (".");
  }
  Serial.println("");
  Serial.println("Connection Successful");
    
} 

void loop() {
  now = millis();
  bool isDetected = digitalRead(motionSensor);
  Serial.println(isDetected);
  if(isDetected==HIGH)
  {
    lastTrigger=millis();
    Serial.println(lastTrigger);
  }
  delay(1000);
  if(isDetected==HIGH and relay==true)
  {
    Serial.println("MOTION DETECTED...");
    digitalWrite(relay_pin4,LOW);
    digitalWrite(relay_pin3,LOW);
    digitalWrite(relay_pin2,LOW);
    digitalWrite(relay_pin1,LOW);
    relay=false;
    lastTrigger = millis();
    Serial.println(now);
    Serial.println(lastTrigger);
  }
  delay(1000);
  now=millis();
 
    
  if(relay==false and ((now - lastTrigger) > (timeSeconds*1000))) 
  {
    Serial.println("Motion stopped...");
    Serial.println(now-lastTrigger);
    digitalWrite(relay_pin4, HIGH);
    digitalWrite(relay_pin3, HIGH);
    digitalWrite(relay_pin2, HIGH);
    digitalWrite(relay_pin1, HIGH);
    relay=true;
    //startTimer = false;
    delay(1000);
  }
  
 
  if(millis()>lastSample+1)
  {
    sampleSum +=sq(analogRead(2930-acs712));
    sampleCount++;
    lastSample = millis();
  }
  if(sampleCount==1000)
  {
    float mean=sampleSum/sampleCount;
    
    float value=sqrt(mean);
    
    float mv=value*5000;
    
    float amphere=mv/4096;
    
    //Serial.println("The final rms value is "+(value));
    Serial.println("The final rms amphere is :"+String(amphere)+"Approximate wattage is:"+String(amphere*230)+".");
    sampleSum=0;
    sampleCount=0;
  }
  
}

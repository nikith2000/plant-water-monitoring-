#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

String apiWritekey = "7KY8KLEGQ36YA8FN";
const char* server = "api.thingspeak.com";
char auth[] = "65TylrGTS8ob1RKTEUC5T-fbibTtPNcx";  //code sent via email
const int sensorPin = 4; 
int sensorlevel=A0;
int sensorState = 0;
int lastState = 0;
WiFiClient client;


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "American_club", "nikith123456");  //wifi name and password
  pinMode(sensorPin, INPUT);
}

void loop()
{ 
  Blynk.run();

  sensorState = digitalRead(sensorPin);
Serial.println(sensorState);

if (sensorState == 1 && lastState == 0) {
  Serial.println("needs water, send notification");
  Blynk.notify("Water your plants");
  lastState = 1;
  delay(1000);
//send notification
    
  } 
  else if (sensorState == 1 && lastState == 1) {
    //do nothing, has not been watered yet
  Serial.println("has not been watered yet");
  delay(1000);
  }
  else {
    //st
    Serial.println("does not need water");
    lastState = 0;
    delay(1000);
  }
    if (client.connect(server,80))
  {  
    String tsData = apiWritekey;
           tsData +="&field1=";
           tsData += String(sensorlevel);
           tsData += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(tsData.length());
     client.print("\n\n");  // the 2 carriage returns indicate closing of Header fields & starting of data
     client.print(tsData);
 
     Serial.print("moisture level: ");
     Serial.print(sensorlevel);
     Serial.println("uploaded to Thingspeak server....");
  }
  client.stop();
 
  Serial.println("Waiting to upload next reading...");
  Serial.println();
  delay(15000);
 delay(1000);
}

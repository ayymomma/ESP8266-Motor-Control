#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#define DHTPIN 2
#define DHTPIN_MOTOR 14
#define ANALOGPIN A0


const char *ssid =  "DIGI-T53k";    
const char *pass =  "TF359U9k";

const uint16_t port = 50100;
const char *host = "192.168.100.44";

float voltage_value = 0.0;
String output = "";
bool start_flag = false;


DHT dht_bridge(DHTPIN, DHT11);
DHT dht_motor(DHTPIN_MOTOR, DHT11);
WiFiClient client;
 
 void stop_test(){
    Serial.println("b'STOP'");
}

void temperature_test(){
    float h = dht_bridge.readHumidity();
    float t = dht_bridge.readTemperature(); 
    float mh = dht_motor.readHumidity();
    float mt = dht_motor.readTemperature();
    String s1 = "T=";
    String s2 = " ";
    String s3 = "H=";
    output = output + "T=" + String(t,2) + " H=" + String(h,2) + " T=" + String(mt,2) + " H=" + String(mh,2) + " ";
}

void voltage_test(){
    voltage_value = analogRead(ANALOGPIN);
    output = output + String(voltage_value, 2) + " ";
}

void speed_test(){
    output = output + "10000";
}

void setup() 
      {
      Serial.begin(115200);
      delay(10);
      dht_bridge.begin();
      dht_motor.begin();
      WiFi.begin(ssid, pass);  
      while (WiFi.status() != WL_CONNECTED) 
      {
            delay(500);
      }
      if(!client.connect(host, port)){
      Serial.println("Connection failed!");
      delay(1000);
      return;
      }
}
 
void loop() 
{
  if(client.available())
  {
    String input = client.readStringUntil(' ');
    if(input == "S")
    {
      start_flag = true;
      String rpm;
      String motor_direction;
      motor_direction = client.readStringUntil(' ');
      rpm = client.readStringUntil(' ');
      Serial.println("b'START + " + motor_direction + " + " + rpm + "'");
    }
    else if(input == "X")
    {
      start_flag = false;
      stop_test();
    }
    else{
      Serial.println(input);
    }
  }
  if(start_flag)
  {
   output = "";
   temperature_test();
   voltage_test();
   speed_test();
   client.print(output);
   delay(1000);
  }
}


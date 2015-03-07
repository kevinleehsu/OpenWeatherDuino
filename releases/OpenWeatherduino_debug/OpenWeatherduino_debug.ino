/*
OpenWeatherDuino by Benjamin Ritter
Ver. 1.4
Visit me at Github:
https://github.com/Benjamin3992/OpenWeatherDuino
*/

#include <DHT.h>
#include <SPI.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>

#define server "www.openweathermap.org"  //Do not change!
#define CredBase64 ""  //Enter here the BASE64 encoded Credentials in the form <Username>:<Password>
#define DhtPin 3  //DHT Temperature and Humidity Sensor PIN goes here
#define UploadCycle 10 //Time in minutes between two pushes goes here

const String StationName = "OpenWeatherDuino1";  //Enter the Station Name how it will be displayed on Openweathermap.org
const String lat = "10.0000000";  //Latitude goes here
const String lng = "10.00000000";  //Longitude goes here
const String alt = "0";  //Enter your Altitude in meters


byte mac[] = {0x46, 0x46, 0x46, 0x46, 0x46, 0x46};  //Enter a HEX-coded MAC-Adress here
const IPAddress ip(192, 168, 178, 200);  //Enter the IP your Arduino will get here
EthernetClient client;
DHT dht;

void setup() {
  dht.setup(DhtPin);
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  Serial.println("OpenWeatherduino by Benjamin Ritter");
  Serial.println("Ver. 1.1");
}

void loop() {
  delay(1000);
  Serial.println("connecting...");
  Serial.println("Collecting data...");
  int t=dht.getTemperature();  //getting the Temperature from the DHT11/DHT22 Sensor
  int h=dht.getHumidity();
  if (client.connect(server, 80)) {  //connecting to the Openweathermap server
    Serial.println("connected");
    Serial.println("Sending Data shortly!");
    client.println("POST /data/post HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.print("Authorization: Basic ");
    client.println(CredBase64);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(int(floor(log10(abs(t)))+floor(log10(abs(h))) + 1 + lat.length()+lng.length()+alt.length()+StationName.length()+38));  //Calculating the Length of the Data for the POST method
    client.println();
    client.print("temp=");
    client.print(t);
    client.print("&humidity=");
    client.print(h);
    client.print("&lat=");
    client.print(lat);
    client.print("&long=");
    client.print(lng);
    client.print("&alt=");
    client.print(alt);
    //client.print("&dewpoint="); //Not working
    //client.print(t-((100-h)/5));
    client.print("&name=");
    client.print(StationName);
    client.println();
    client.stop();
    for(int i=0; i>=UploadCycle*60; i++)  {
      delay(1000);
    }
  }
  else {
    Serial.println("connection failed");
  }
}

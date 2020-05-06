#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "Wire.h"
#include <SoftwareSerial.h>
#define Memory_disk1 0x50
SoftwareSerial btSerial(D5, D6); // Rx,Tx
int address = 0;
long int val = 0;
int outPin1 = 0;     //motor1
int outPin2 = 2;    //motor1
int outPin3 = 13;   //motor2
int outPin4 = 15;   //motor2
/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "Project_Arsenal"
#define WLAN_PASS       "12345678"
#define MQTT_SERVER      "10.42.0.1"  // give static address
#define MQTT_PORT         1883                    
#define MQTT_USERNAME    ""
#define MQTT_PASSWORD         ""
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD);
/****************************** Feeds ***************************************/
// Setup a feed called 'pi_led' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish pi_led = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "Pi");
// Setup a feed called 'esp8266_led' for subscribing to changes.
Adafruit_MQTT_Subscribe esp8266_led = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "esp8266");
/*************************** Sketch Code ************************************/
void MQTT_connect();

void setup() {
   delay(1000);
   Wire.begin(D1,D2);
  Serial.begin(115200);
  delay(10);
  Serial.println(F("RPi-ESP-MQTT"));
 // Connect to WiFi access point.
 Serial.println(); Serial.println();
 Serial.print("Connecting to ");
 Serial.println(WLAN_SSID);
 WiFi.begin(WLAN_SSID, WLAN_PASS);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
 Serial.println();
 Serial.println("WiFi connected");
 Serial.println("IP address: "); Serial.println(WiFi.localIP());
 // Setup MQTT subscription for esp8266_led feed.
 mqtt.subscribe(&esp8266_led);
   btSerial.begin(9600);     // bluetooth module baudrate
   pinMode(outPin1,OUTPUT);
   pinMode(outPin2,OUTPUT);
   pinMode(outPin3,OUTPUT);
   pinMode(outPin4,OUTPUT);  

}
uint32_t x=0;  

void loop() {
  if (btSerial.available() > 0) {    // check if bluetooth module sends some data to esp8266
            char bdata = btSerial.read();  // read the data from HC-06
            btexecute(bdata);
            Serial.println(bdata);}   
    else{
            MQTT_connect();
             // this is our 'wait for incoming subscription packets' busy subloop
             // try to spend your time here
             // Here its read the subscription
             char * red;
             Adafruit_MQTT_Subscribe *subscription;
             while ((subscription = mqtt.readSubscription())) {
             if (subscription == &esp8266_led) {
             Serial.print(F("\0"));
             Serial.println((char *)esp8266_led.lastread);
                }
               }
            char parkA[20]="A";char parkB[20]="B";
            char parkC[20]="C";char parkD[20]="D";
            char revA[20]="E"; char revB[20]="F";
            char revC[20]="G"; char revD[20]="H";
            if(strcmp(((char *)esp8266_led.lastread),parkA) == 0)
            {if (address<1000){
              for(address=0;address<=1000;address++)
              {execute(address);}}}
            else if(strcmp(((char *)esp8266_led.lastread),parkB) == 0)
            {if(address<2000){for(address=1000;address<=2000;address++)
              {execute(address); }}}
            else if(strcmp(((char *)esp8266_led.lastread),parkC) == 0)
            {if(address<3000){for(address=2000;address<=3000;address++)
              {execute(address);}}}
            else if(strcmp(((char *)esp8266_led.lastread),parkD) == 0)
            {if(address<4000){for(address=3000;address<=4000;address++)
              {execute(address);}}}
             else if(strcmp(((char *)esp8266_led.lastread),parkB) == 0)
            {if(address<5000){for(address=4000;address<=5000;address++)
              {execute(address); }}}
            else if(strcmp(((char *)esp8266_led.lastread),parkC) == 0)
            {if(address<6000){for(address=5000;address<=6000;address++)
              {execute(address);}}}
            else if(strcmp(((char *)esp8266_led.lastread),parkD) == 0)
            {if(address<7000){for(address=6000;address<=7000;address++)
              {execute(address);}}}
             else if(strcmp(((char *)esp8266_led.lastread),parkD) == 0)
            {if(address<8000){for(address=7000;address<=8000;address++)
              {execute(address);}}}
      
      }
}


/*void writeAddress(int address, long int val)
{
  Wire.beginTransmission(Memory_disk1);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  Wire.write(val);
  Wire.endTransmission();
  delay(5);
  return;
}*/

byte readAddress(int address)
{
  byte rData = 0xFF;
  Wire.beginTransmission(Memory_disk1);
  Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  Wire.endTransmission();  
  Wire.requestFrom(Memory_disk1, 1);  
  rData =  Wire.read();
  return rData;
}
// Function to connect and reconnect as necessary to the MQTT server.
void MQTT_connect() {
 int8_t ret;
 // Stop if already connected.
 if (mqtt.connected()) {
   return;
 }
 Serial.print("Connecting to MQTT... ");
 uint8_t retries = 3;
 while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
      Serial.println(mqtt.connectErrorString(ret));
      Serial.println("Retrying MQTT connection in 5 seconds...");
      mqtt.disconnect();
      delay(5000);  // wait 5 seconds
      retries--;
      if (retries == 0) {
        // basically die and wait for WDT to reset me
        while (1);
      }
 }
 Serial.println("MQTT Connected!");
}
char motorconver(byte dataval)
{
  char data;
  switch(dataval)
                {
                  case 25:
                      data='F';
                      break;
                  case 75:
                      data='S';
                      break;
                  case 50:
                      data ='B';
                      break;
                  case 100:
                      data ='R';
                      break;
                  case 125:
                      data ='L';
                      break;
                  case 150:
                      data ='G';
                      break;
                  case 175:
                      data ='I';
                      break;
                  case 200:
                      data ='H';
                      break;
                  case 225:
                      data ='J';
                      break;
                  default:
                  break;
                }
return data;
}

void execute(int addr)
{
                delay(107);
                byte readVal = readAddress(addr);
                Serial.println(addr);
                Serial.println(readVal);
                char data= motorconver(readVal);
                btexecute(data);
                Serial.println(data);                 
                return;
}

void btexecute(char sata){
   switch (sata)
                {
                  case 'F':         // forward
                    digitalWrite(outPin1,HIGH);
                    digitalWrite(outPin2,LOW);
                    digitalWrite(outPin3,LOW);
                    digitalWrite(outPin4,LOW);
                    //val=25;
                    //writeAddress(address, val);
                    break;
                  case 'B':              // reverse
                    digitalWrite(outPin1,LOW);
                    digitalWrite(outPin2,HIGH);
                    digitalWrite(outPin3,LOW);
                    digitalWrite(outPin4,LOW);
                    //val=50;
                    //writeAddress(address, val);
                    break;
                  case 'S':              // stop
                    digitalWrite(outPin1,LOW);
                    digitalWrite(outPin2,LOW);
                    digitalWrite(outPin3,LOW);
                    digitalWrite(outPin4,LOW);
                    //val=75;
                    //writeAddress(address, val);
                    break;
                  case 'R':              // right
                     digitalWrite(outPin3,LOW);
                     digitalWrite(outPin4,HIGH);
                     digitalWrite(outPin1,LOW);
                     digitalWrite(outPin2,LOW);
                    //val=100;
                    //writeAddress(address, val);
                    break;
                  case 'L':              // left
                     digitalWrite(outPin3,HIGH);
                     digitalWrite(outPin4,LOW);
                     digitalWrite(outPin1,LOW);
                     digitalWrite(outPin2,LOW);
                    //val=125;
                    //writeAddress(address, val);
                     break;
                  case 'G':              // f left
                    digitalWrite(outPin1,HIGH);
                    digitalWrite(outPin2,LOW);
                    digitalWrite(outPin3,HIGH);
                    digitalWrite(outPin4,LOW);
                    //val=150;
                    //writeAddress(address, val);
                    break;
                  case 'I':              // f right
                    digitalWrite(outPin1,HIGH);
                    digitalWrite(outPin2,LOW);
                    digitalWrite(outPin3,LOW);
                    digitalWrite(outPin4,HIGH);
                    //val=175;
                    //writeAddress(address, val);
                    break;
                  case 'H':              // b left
                    digitalWrite(outPin1,LOW);
                    digitalWrite(outPin2,HIGH);
                    digitalWrite(outPin3,HIGH);
                    digitalWrite(outPin4,LOW);
                    //val=200;
                    //writeAddress(address, val);
                    break;
                  case 'J':              // b right
                     digitalWrite(outPin1,LOW);
                     digitalWrite(outPin2,HIGH);
                     digitalWrite(outPin3,LOW);
                     digitalWrite(outPin4,HIGH);
                     //val=225;
                    //writeAddress(address, val);
                    break;
                    default:
                    break;
    }
return;
}

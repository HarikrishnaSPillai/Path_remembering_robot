#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "Wire.h"
#include <SoftwareSerial.h>
#define Memory_disk1 0x50
SoftwareSerial btSerial(D5, D6); // Rx,Tx
int k=0;
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
            int lo; int up;
            char parkA[20]="A";char parkB[20]="B";char parkC[20]="C";char parkD[20]="D";char revA[20]="E"; char revB[20]="F";char revC[20]="G"; char revD[20]="H";
            if(strcmp(((char *)esp8266_led.lastread),parkA) == 0)
            {if(k<1024) {lo=0;up=1024; k=execute(lo,up);}}
            else if(strcmp(((char *)esp8266_led.lastread),parkB) == 0)
            {if(k<2048) {lo=1024;up=2048;k=execute(lo,up);}}
            else if(strcmp(((char *)esp8266_led.lastread),parkC) == 0)
            {if(k<3072) {lo=2048;up=3072;k=execute(lo,up);}}
            else if(strcmp(((char *)esp8266_led.lastread),parkD) == 0)
            {if(k<4096) {lo=3072;up=4096;k=execute(lo,up);}}
             else if(strcmp(((char *)esp8266_led.lastread),parkB) == 0)
            {if(k<5120) {lo=4096;up=5120;k=execute(lo,up);}}
            else if(strcmp(((char *)esp8266_led.lastread),parkC) == 0)
            {if(k<6144) {lo=5120;up=6144;k=execute(lo,up);}}
            else if(strcmp(((char *)esp8266_led.lastread),parkD) == 0)
            {if(k<7168) {lo=6144;up=7168;k=execute(lo,up);}}
             else if(strcmp(((char *)esp8266_led.lastread),parkD) == 0)
            {if(k<8192) {lo=7168;up=8192;k=execute(lo,up);}}
      }
}
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

int execute(int lolimit,int uplimit)
{
                byte readVal;
                char data[32][32];
                char kata;
                int z=0;
                for(int i=0;i<=32;i++){
                  for(int j=0;j<=32;j++){
                    if(lolimit<=uplimit){
                    readVal = readAddress(lolimit);
                    data[i][j] = motorconver(readVal);
                    lolimit++;    }}}
                for(int i=0;i<=32;i++){
                  for(int j=0;j<=32;j++){
                     kata = data[i][j];
                     btexecute(kata);                     
                     Serial.println(z);
                     Serial.println(kata); 
                     z++; 
                     delay(100);}}               
                     return uplimit;
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

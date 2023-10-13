#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#define LED_PIN 2

/* Change the following configuration options */
const char* ssid = "";
const char* password = "";
const char* aws_iot_hostname = "";
const char* aws_iot_sub_topic = "";
const char* aws_iot_pub_topic = "";
const char* aws_iot_pub_message = "Acknowledged.";
const char* client_id = "MyIoT";

const char* ca_certificate = "";
const char* iot_certificate ="";
const char* iot_privatekey = "";
#define SSID_HAS_PASSWORD //comment this line if your SSID does not have a password

/* Global Variables */
Servo myservo;
int servoPin = 14;
int pos=0;
WiFiClientSecure client;
PubSubClient mqtt(client);
int aiopen=0;
const int potPin = 34;
int potValue = 0;

void unlock() {
   

  
    // in steps of 1 degree
    myservo.write(180);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
   
             
  

void lock()
{
  
    myservo.write(40);    // tell servo to go to position in variable 'pos'
    delay(15);  
}
  


/* Functions */
void sub_callback(const char* topic, byte* payload, unsigned int length) {
  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  for (int i = 0; i < length; i++)
    Serial.print((char) payload[i]);
  Serial.println();

  if ((char) payload[0] == '1')
  {
    unlock();
    delay(10000);
    lock();
  }
  else if ((char) payload[0] == '0')
    lock();
   else if ((char) payload[0] == '2')
   {  
    
      digitalWrite(5,LOW);
      delay(2000);
      digitalWrite(5,HIGH);
      
   }

  mqtt.publish(aws_iot_pub_topic, aws_iot_pub_message);
}

void setup() 
{
  //Initializations
  Serial.begin(9600);
  Serial.print("Attempting WiFi connection on SSID: ");
  Serial.print(ssid);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  myservo.setPeriodHertz(50);   
myservo.attach(servoPin, 1000, 2000);
  pinMode(5,OUTPUT);
  pinMode(12,INPUT);
  
  // WiFi
  #ifdef SSID_HAS_PASSWORD
  WiFi.begin(ssid, password);
  #else
  WiFi.begin(ssid);
  #endif

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.print("\nWiFi connection succeeded.\n");

  client.setCACert(ca_certificate);
  client.setCertificate(iot_certificate);
  client.setPrivateKey(iot_privatekey);

  // AWS IoT MQTT uses port 8883
  mqtt.setServer(aws_iot_hostname, 8883);
  mqtt.setCallback(sub_callback);
}

void loop() {

  
  // reconnect on disconnect
  while (!mqtt.connected()) {
    Serial.print("Now connecting to AWS IoT: ");
    if (mqtt.connect(client_id)) {
      Serial.println("connected!");
      mqtt.subscribe(aws_iot_sub_topic); //subscribe to the topic
    } else {
      Serial.print("failed with status code ");
      Serial.print(mqtt.state());
      Serial.println(" trying again in 5 seconds...");
      delay(5000);
    }
  }

  mqtt.loop();
  aiopen=digitalRead(12);
  potValue = analogRead(potPin);
  if(potValue>=3000)
  {
    unlock();
    Serial.println("it is open");
  }
  digitalWrite(5,HIGH);
  }

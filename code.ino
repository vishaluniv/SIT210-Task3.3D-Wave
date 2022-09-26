
#include <ArduinoMqttClient.h>

#include <WiFiNINA.h>


char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS; 

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.mqttdashboard.com";
int        port     = 1883;
const char topic[]  = "SIT210/wave";

int count = 0;
const long interval = 1000;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);//define arduino pin
  pinMode(4, INPUT);
    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(3000);
  }
 
  Serial.println("You're connected to the network");
  Serial.println();
  
  mqttClient.setId("");
  
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  
  
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }
  
    Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  
    Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
  Serial.println();

  
}  

String wave = "Vishal: A wave was detected";

void loop() {
  mqttClient.poll();
  
    
    int messageSize = mqttClient.parseMessage();
    
     if (messageSize) {
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    
    Blink();

    // use the Stream interface to print the contents
    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }
    Serial.println();

    Serial.println();
  }
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
    if(Wave() == 1){
      Serial.print("Sending message to topic: ");
    Serial.println(topic);
    
    mqttClient.beginMessage(topic);
    mqttClient.print(wave);
    mqttClient.endMessage();
  
    Serial.println("Message sent");

    Serial.println();
    
    count++;
    }
    
    
  }
  
 
  
    
    
}

void Blink(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
  
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(300);
}

int Wave(){
  //pulse output
  digitalWrite(2, LOW);
  delayMicroseconds(4);
  digitalWrite(2, HIGH);
  delayMicroseconds(10);
  digitalWrite(2, LOW);
  
  long t = pulseIn(4, HIGH);//input pulse and save it veriable
  
  long inches = t / 74 / 2; //time convert distance
  String inch;
  if(inches<9.0){
    return 1;
  
    delay(100);//delay
  }
  else{
    return 0;
  }
  
  
  
  
}

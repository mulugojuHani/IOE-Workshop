

//Headers or Packages

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Constant values

const char* ssid = "Sowjanya";
const char* password = "12345678";
const char* mqtt_server = "mqtt.dioty.co";
const char* email="bramhanimulugoju@gmail.com";
const char* e_password="a0360821";
int led_onoff_pin=2;
const char* led_topic="/bramhanimulugoju@gmail.com/Leds";
const char* led_topic_p="/bramhanimulugoju@gmail.com/Ledp";
//Non initialize values

WiFiClient espClient;
PubSubClient client(espClient);
int cc=0;
char check;
char mssg[175];
//Wifi Setup

void setup_wifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}

//simplify outcomes from callback

void reduce_cal(char state){
  if(cc == 0){
      check=state;
      cc=1;
      LED(state);
      }else{
        if( check != state){
          check=state;
          LED(state);
          }
      }
  }

//LED on-off function

void LED(char onoff){
  if(onoff == 't'){
     digitalWrite(led_onoff_pin,LOW);
    snprintf (mssg, 175, "LIGHT is OFF at #%ld pin",led_onoff_pin);
    Serial.println("oFF");
  
  }else{
     digitalWrite(led_onoff_pin,HIGH);
   snprintf (mssg, 175, "LIGHT is ON at #%ld pin",led_onoff_pin);
   Serial.println("on"); 
  }

      
      client.publish(led_topic_p,mssg );
}

//When subscribe to topic, we need to get the value of subscribe topic -- is here

void callback(char* topic, byte* payload, unsigned int length) {
   char val=(char)payload[0];
    reduce_cal(val);
}

//First setup block which executes at program start to settle loop function stable

void setup() {   
  Serial.begin(115200);
  pinMode(led_onoff_pin, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
    
}

//When mqtt broker is not connected,then here is re-again connection

void reconnect(){
  while (!client.connected()) {
    if (client.connect(email,email,e_password,led_topic,2,0,"f")) {
    client.subscribe(led_topic,1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// continuous execution means loop function

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

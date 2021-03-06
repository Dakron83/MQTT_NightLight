/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
    
    #define NUM_LEDS 155
    #define DATA_PIN 1
    #define COLOR_ORDER BRG
    #define LED_TYPE    WS2811
    #define BRIGHTNESS          30

    CRGB leds[NUM_LEDS];



// Update these with values suitable for your network.

const char* ssid = "YourSSID";
const char* password = "YourPassword";
const char* mqtt_server = "MQTT Server IP";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  //pinMode(D1, OUTPUT);     // Initialize the D1 pin as an output
  //pinMode(D2, OUTPUT);     // Initialize the D2 pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // prepare GPIO2 *********************************************************************
  pinMode(2, OUTPUT);   // i am using gpio2 as output to toggle an LED
  digitalWrite(2, 0);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    ledtest();
  } 

  else if ((char)payload[0] == '2') {
    ledTooEarly();
  } 

  else if ((char)payload[0] == '3') {
    ledAlmost();
  } 
  
  else if ((char)payload[0] == '4') {
    ledMorning();
  } 

  else {
    ledOFF();
    //digitalWrite(D1, LOW);  // Turn the LED off by making the voltage HIGH
    //digitalWrite(D2, LOW);  // Turn the LED off by making the voltage HIGH
  }
}

void ledtest(){
        // Turn the first led red for 1 second
          fill_solid(leds, NUM_LEDS, CRGB::Purple);
     FastLED.show();
  
}

void ledOFF(){
        // Turn the first led red for 1 second
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
  
}
void ledTooEarly(){
        // Turn the first led red for 1 second
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      FastLED.show();
  
}
void ledAlmost(){
        // Turn the first led red for 1 second
      fill_solid(leds, NUM_LEDS, CRGB::Orange);
      FastLED.show();
  
}
void ledMorning(){
        // Turn the first led red for 1 second
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      FastLED.show();
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", "USERNAME", "PASSWORD")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("home/girlsroom/mode");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 20000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
  
  
}

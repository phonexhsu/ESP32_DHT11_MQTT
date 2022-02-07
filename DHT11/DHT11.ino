#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h> //MQTT Client library
#include <SimpleDHT.h> //DHT11/20 library

int pinDHT11 = 4;  //定義DHT11的GPIO pin
const char* ssid = ""; //WiFi SSID
const char* password = ""; //WiFi password 
const char* mqtt_server = ""; //定義mqtt broker的IP
const char* mqttUserName = ""; //mqtt使用者名稱
const char* mqttPwd = ""; //mqtt密碼
unsigned long lastMsg = 0; //timer初始timestamp值
int interval = 5000; //timer interval(ms)

SimpleDHT11 dht11(pinDHT11);
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);  //設定WiFi模式
  WiFi.begin(ssid, password); //對WiFi AP發起連線
  
  //每0.5秒檢查一次是否連線成功，若尚未連線成功則在Console顯示"."，反覆檢查直到連線成功
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //提示WIFI Client已經連接成功
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //提示已取到的IP
}

//MQTT斷線重連功能
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttUserName, mqttPwd)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi(); //WiFi Client
  client.setServer(mqtt_server, 1883); //mqtt Client
  //client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //DHT11讀值
  byte temperature = 0;
  byte humidity = 0;
  unsigned long now = millis();
  if (now - lastMsg > interval) {
    lastMsg = now;

    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      //顯示error code
      Serial.print("Read DHT11 failed, err=");
      Serial.print(SimpleDHTErrCode(err));
      Serial.print(",");
      Serial.println(SimpleDHTErrDuration(err));
      return;
    } else {
      //以MQTT回傳溫濕度值
      client.publish("temperature", String(temperature).c_str());
      Serial.print((int)temperature); Serial.print(" *C, ");
      client.publish("humidity", String(humidity).c_str());
      Serial.print((int)humidity); Serial.println(" %, ");
    }
  }
}

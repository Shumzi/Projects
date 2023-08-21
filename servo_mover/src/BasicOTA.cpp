#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Servo.h>

#ifndef STASSID
#define STASSID "ashum"
#define STAPSK "valentine"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const int button_pin_io_2 = 2;
const int servo_pin_io_0 = 0;
// const int led_pin_io_0 = 0;
int pos = 0;
Servo myservo;  // create servo object to control a servo
String newHostname = "ESP8266Servo";

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.hostname(newHostname.c_str());
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }


  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  myservo.attach(servo_pin_io_0);
  // pinMode(led_pin_io_0, OUTPUT);
  pinMode(button_pin_io_2, INPUT_PULLUP);
}

void loop() {
  ArduinoOTA.handle();
  // digitalWrite(led_pin_io_0, HIGH);
  // delay(500);
  // digitalWrite(led_pin_io_0, LOW);
  // delay(500);
  int sensorVal = digitalRead(button_pin_io_2);

  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:

  // if pressed button
  if (sensorVal == HIGH) {
    myservo.write(0);
    // digitalWrite(led_pin_io_0, HIGH);
  }
  else
  {
    // digitalWrite(led_pin_io_0, LOW);
    
    myservo.write(180);
  }

}

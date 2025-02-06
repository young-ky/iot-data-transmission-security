#include <WiFi.h>
#include <DeviceValidator.h>
#include <SendLog.h>
#include <GetMe.h>

const char* ssid = "wifi ssid";
const char* password = "password";

const String deviceIdentifier = "identifier";
const String devicePassword = "password";

String output;
String token;
String trigger;
String response;

const int sensorPin = 36;

int percentage;
int sensorValue;

void setup() {
  Serial.begin(115200);
  delay(5000);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  token = validateDevice(deviceIdentifier, devicePassword);
  trigger = getMe(token);
}

void loop() {
  sensorValue = analogRead(sensorPin);

  // put calibrated min-max sensor value on second-third parameter
  percentage = map(sensorValue, 3400, 1500, 0, 100);
  percentage = constrain(percentage, 0, 100);

  output = "Value: " + String(sensorValue) + " | " + String(percentage) + "%";
  Serial.println(output);

  // sending sensor data
  response = sendLog(token, sensorValue);

  // get new trigger percentage
  trigger = getMe(token);

  delay(10000);
}
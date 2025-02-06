#include "SendLog.h"
#include "Certificate.h"

String sendLog(String token, int data) {
  Serial.println("Start sending sensor data...");

  String url = "https://url/api/save-log";
  String jsonParameter;

  HTTPClient http;

  WiFiClientSecure client;
  client.setCACert(rootCACertificate);

  StaticJsonDocument<128> document;
  StaticJsonDocument<64> requestBody;

  requestBody["sensorData"] = data;

  serializeJson(requestBody, jsonParameter);

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Device-Token", token);

  int statusCode = http.POST(jsonParameter);
  Serial.println(statusCode);

  if (statusCode == -1) {
    String errorMessage = http.errorToString(statusCode);
    Serial.println("Error: " + errorMessage);
  }

  String response = http.getString();
  Serial.println(response);
  deserializeJson(document, response);
  JsonObject object = document.as<JsonObject>();;

  statusCode = object["code"];

  if (statusCode == 200) {
    Serial.printf("Moisture data sent with status code %d\n", statusCode);
  }
  else if (statusCode == 401) {
    Serial.println("Unauthorized, attempt to validate again.");
  }
  else {
    Serial.printf("Data sending failed, HTTP request failed with status code: %d\n", statusCode);
  }

  http.end();
  return String(statusCode);
}
#include "DeviceValidator.h"
#include "Certificate.h"

String validateDevice(String deviceIdentifier, String devicePassword) {
  Serial.println("Start validating device...");

  String url = "https://url/api/device/verify";
  String token = "";
  String jsonParameter;

  JsonObject object;

  HTTPClient http;

  WiFiClientSecure client;
  client.setCACert(rootCACertificate);

  StaticJsonDocument<256> document;
  StaticJsonDocument<128> requestBody;

  requestBody["deviceIdentifier"] = deviceIdentifier;
  requestBody["password"] = devicePassword;
  serializeJson(requestBody, jsonParameter);

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  int statusCode = http.POST(jsonParameter);
  Serial.println(statusCode);

  if (statusCode == -1) {
    String errorMessage = http.errorToString(statusCode);
    Serial.println("Error: " + errorMessage);
  }

  String response = http.getString();
  Serial.println(response);
  deserializeJson(document, response);
  object = document.as<JsonObject>();

  statusCode = object["code"];

  if (statusCode == 200) {
    String data = object["data"];

    if (!data.isEmpty()) {
      deserializeJson(document, data);
      object = document.as<JsonObject>();
      token = object["token"] | "";
      Serial.println("Validation success.");
    }
    else {
      Serial.println("Error parsing data, validation failed.");
    }
  }
  else {
    Serial.printf("Validation failed, HTTP request failed with status code: %d\n", statusCode);
  }

  http.end();
  return token;
}
#include "GetMe.h"
// #include "Certificate.h"

String getMe(String token) {
  Serial.println("Getting trigger percentage...");

  String url = "http://url/api/device/me";
  String trigger = "";

  JsonObject object;

  HTTPClient http;

  WiFiClientSecure client;
  client.setCACert(rootCACertificate);

  StaticJsonDocument<128> document;

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Device-Token", token);

  int statusCode = http.GET();
  Serial.println(statusCode);

  if (statusCode == -1) {
    String errorMessage = http.errorToString(statusCode);
    Serial.println("Error: " + errorMessage);
  }

  String response = http.getString();
  Serial.println(response);
  deserializeJson(document, response);
  object = document.as<JsonObject>();;

  statusCode = object["code"];

  if (statusCode == 200) {
    String data = object["data"];

    if (!data.isEmpty()) {
      deserializeJson(document, data);
      object = document.as<JsonObject>();
      trigger = object["trigger"] | "";
      Serial.println("Trigger fetch success.");
    }
    else {
      Serial.println("Error parsing data, validation failed.");
    }
  }
  else if (statusCode == 401) {
    Serial.println("Unauthorized, attempt to validate again.");
    return String(statusCode);
  }
  else {
    Serial.printf("Data sending failed, HTTP request failed with status code: %d\n", statusCode);
  }

  http.end();
  return trigger;
}
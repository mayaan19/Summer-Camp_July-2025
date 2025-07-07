#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>
#include <ESP8266WebServer.h>

const char* ssid = "Loading...";
const char* password = "GuessItIfYouCan";

LiquidCrystal lcd(D1, D2, D3, D4, D5, D6);

ESP8266WebServer server(80);

void handleRequest() {
  String path = server.uri().substring(1);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shape:");
  lcd.setCursor(0, 1);
  lcd.print(path);
  server.send(200, "text/plain", "Displayed: " + path);
  Serial.println("Received and displayed: " + path);
}

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("AI and IoT-Based");
  lcd.setCursor(0, 1);
  lcd.print("Smart  Solutions");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP Address:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Shape:");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");

  server.onNotFound(handleRequest);
  server.begin();
  Serial.println("Server started at:");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}

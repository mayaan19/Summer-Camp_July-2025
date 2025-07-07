#include <ESP8266WiFi.h>

// Wi-Fi credentials
const char* ssid = "Loading...";
const char* password = "GuessItIfYouCan";

// LED pins
const int led1 = D1;
const int led2 = D2;
const int led3 = D3;
const int led4 = D4;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  // Set LED pins as output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) delay(1);

  String request = client.readStringUntil('\r');
  client.flush();

  // Parse LED control from URL
  if (request.indexOf("/led1/on") != -1) digitalWrite(led1, HIGH);
  if (request.indexOf("/led1/off") != -1) digitalWrite(led1, LOW);
  if (request.indexOf("/led2/on") != -1) digitalWrite(led2, HIGH);
  if (request.indexOf("/led2/off") != -1) digitalWrite(led2, LOW);
  if (request.indexOf("/led3/on") != -1) digitalWrite(led3, HIGH);
  if (request.indexOf("/led3/off") != -1) digitalWrite(led3, LOW);
  if (request.indexOf("/led4/on") != -1) digitalWrite(led4, HIGH);
  if (request.indexOf("/led4/off") != -1) digitalWrite(led4, LOW);

  // Web Page -----------------------------------------------
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<!DOCTYPE html><html><head>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println("<title>LED Control</title>");
  client.println("<style>");
  client.println("body { font-family: Arial; background: #f0f0f0; margin: 0; height: 100vh; display: flex; justify-content: center; align-items: center; }");
  client.println(".container { text-align: center; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 4px 20px rgba(0,0,0,0.1); max-width: 400px; }");
  client.println("h1 { color: #333; margin-bottom: 25px; }");
  client.println(".btn { display: inline-block; padding: 12px 20px; margin: 8px; font-size: 16px; color: white; text-decoration: none; border-radius: 6px; transition: background 0.3s; }");
  client.println(".on { background-color: #4CAF50; }");
  client.println(".on:hover { background-color: #45a049; }");
  client.println(".off { background-color: #f44336; }");
  client.println(".off:hover { background-color: #da190b; }");
  client.println(".section { margin-bottom: 20px; }");
  client.println("</style>");
  client.println("</head><body>");
  client.println("<div class='container'>");
  client.println("<h1>ESP8266 LED Control</h1>");

  client.println("<div class='section'>");
  client.println("<h2>Red LED</h2>");
  client.println("<a class='btn on' href='/led1/on'>Turn ON</a>");
  client.println("<a class='btn off' href='/led1/off'>Turn OFF</a>");
  client.println("</div>");

  client.println("<div class='section'>");
  client.println("<h2>Blue LED</h2>");
  client.println("<a class='btn on' href='/led2/on'>Turn ON</a>");
  client.println("<a class='btn off' href='/led2/off'>Turn OFF</a>");
  client.println("</div>");

  client.println("<div class='section'>");
  client.println("<h2>Green LED</h2>");
  client.println("<a class='btn on' href='/led3/on'>Turn ON</a>");
  client.println("<a class='btn off' href='/led3/off'>Turn OFF</a>");
  client.println("</div>");

  client.println("<div class='section'>");
  client.println("<h2>Yellow LED</h2>");
  client.println("<a class='btn on' href='/led4/on'>Turn ON</a>");
  client.println("<a class='btn off' href='/led4/off'>Turn OFF</a>");
  client.println("</div>");

  client.println("</div></body></html>");
  // Web Page -----------------------------------------------

  delay(1);
}

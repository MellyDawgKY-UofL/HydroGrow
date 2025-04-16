#include "DHT.h"
#include <Wire.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#define DHTPIN 13
#define LIGHT_PIN 35
#define motor1A 4   // GPIO to control Light relay via NPN transistor
#define motor2A 18  // GPIO to control Pump relay via NPN transistor

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "DeficationStation";
const char* password = "Kentucky1!";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);
AsyncWebServer server(80);

int lightOnTime, lightOffTime, waterOnTime, waterOffTime;
bool manualLight = false;
bool manualWater = false;

void loadConfig() {
  EEPROM.begin(512);
  lightOnTime = EEPROM.read(0);
  lightOffTime = EEPROM.read(1);
  waterOnTime = EEPROM.read(2);
  waterOffTime = EEPROM.read(3);
  EEPROM.end();

  if (lightOnTime > 23) lightOnTime = 8;
  if (lightOffTime > 23) lightOffTime = 20;
  if (waterOnTime > 23) waterOnTime = 9;
  if (waterOffTime > 23) waterOffTime = 10;
}

void saveConfig() {
  EEPROM.begin(512);
  EEPROM.write(0, lightOnTime);
  EEPROM.write(1, lightOffTime);
  EEPROM.write(2, waterOnTime);
  EEPROM.write(3, waterOffTime);
  EEPROM.commit();
  EEPROM.end();
}

String formatHour(int hour) {
  if (hour > 23) return "Unset";
  return String(hour);
}

String dashboardHtml() {
  float temperature = dht.readTemperature();
  float tempF = (temperature * 1.8) + 32;
  float humidity = dht.readHumidity();
  int light = analogRead(LIGHT_PIN);

  String lightState = digitalRead(motor1A) == HIGH ? "ON" : "OFF";
  String pumpState = digitalRead(motor2A) == HIGH ? "ON" : "OFF";

  String html = "<html><body><h1>HydroGrow Dashboard</h1>";
  html += "<p><b>Current Time:</b> " + timeClient.getFormattedTime() + "</p>";
  html += "<p><b>Temperature:</b> " + String(tempF) + " F</p>";
  html += "<p><b>Humidity:</b> " + String(humidity) + " %</p>";
  html += "<p><b>Light Level:</b> " + String(light) + "</p>";
  html += "<p><b>Light Automation:</b> " + formatHour(lightOnTime) + ":00 to " + formatHour(lightOffTime) + ":00 | State: <b>" + lightState + "</b></p>";
  html += "<p><b>Water Automation:</b> " + formatHour(waterOnTime) + ":00 to " + formatHour(waterOffTime) + ":00 | State: <b>" + pumpState + "</b></p>";
  html += "<form action='/toggleLight'><button>Toggle Light</button></form>";
  html += "<form action='/toggleWater'><button>Toggle Water Pump</button></form>";
  html += "<br><a href='/config'><button>Edit Automation Settings</button></a>";
  html += "</body></html>";
  return html;
}

String configPageHtml() {
  String html = "<html><body><h2>Edit Automation Times</h2>";
  html += "<form action='/save' method='GET'>";
  html += "Light On Hour (0-23): <input name='lo' type='number'><br>";
  html += "Light Off Hour (0-23): <input name='lf' type='number'><br>";
  html += "Water On Hour (0-23): <input name='wo' type='number'><br>";
  html += "Water Off Hour (0-23): <input name='wf' type='number'><br>";
  html += "<input type='submit' value='Save'>";
  html += "</form>";
  html += "<br><a href='/'><button>Back</button></a></body></html>";
  return html;
}

void updateLightOutput() {
  int h = timeClient.getHours();
  digitalWrite(motor1A, (manualLight || (h >= lightOnTime && h < lightOffTime)) ? HIGH : LOW);
}

void updateWaterOutput() {
  int h = timeClient.getHours();
  digitalWrite(motor2A, (manualWater || (h >= waterOnTime && h < waterOffTime)) ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(motor1A, OUTPUT);  // Light Control via NPN
  pinMode(motor2A, OUTPUT);  // Pump Control via NPN
  pinMode(LIGHT_PIN, INPUT);

  loadConfig();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); Serial.println("Connecting...");
  }
  Serial.println("Connected");

  timeClient.begin();
  MDNS.begin("HydroGrow");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *r){ r->send(200, "text/html", dashboardHtml()); });
  server.on("/config", HTTP_GET, [](AsyncWebServerRequest *r){ r->send(200, "text/html", configPageHtml()); });

  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *r) {
    if (r->hasParam("lo")) lightOnTime = r->getParam("lo")->value().toInt();
    if (r->hasParam("lf")) lightOffTime = r->getParam("lf")->value().toInt();
    if (r->hasParam("wo")) waterOnTime = r->getParam("wo")->value().toInt();
    if (r->hasParam("wf")) waterOffTime = r->getParam("wf")->value().toInt();
    saveConfig();
    r->redirect("/");
  });

  server.on("/toggleLight", HTTP_GET, [](AsyncWebServerRequest *r){ manualLight = !manualLight; updateLightOutput(); r->redirect("/"); });
  server.on("/toggleWater", HTTP_GET, [](AsyncWebServerRequest *r){ manualWater = !manualWater; updateWaterOutput(); r->redirect("/"); });

  server.begin();
}

void loop() {
  timeClient.update();
  updateLightOutput();
  updateWaterOutput();
  delay(2000);
}

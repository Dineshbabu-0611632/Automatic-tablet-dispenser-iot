#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
// WiFi credentials
const char* ssid = "hello";
const char* password = "00000000";
// API details
const char* apiKey = "uFJy0X5wRwij";
const char* templateID = "101";
const char* mobileNumber = "916374917133";
const char* var1 = "patient taking tablet";
const char* var2 = "collage";
// RFID and LCD setup
#define SS_PIN D2
#define RST_PIN D1
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Predefined user list
struct User {
String uid;
String name;
};
User users[] = {
{"B6913A57", "Mathan"},
{"83D46115", "vasan"},
{"13D47F0D", "hari"}
};
String getUserName(String uid) {
for (User user : users) {
if (user.uid == uid) {
return user.name;
}
}
return "Unknown";
}
void setup() {
Serial.begin(115200);
Serial.println("Starting system...");
WiFi.begin(ssid, password);
Serial.print("Connecting to WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("\nConnected to WiFi");
// Initialize SPI, RFID, and LCD
Serial.println("Initializing SPI, RFID, and LCD...");
SPI.begin();
rfid.PCD_Init();
Wire.begin(D3, D4);
lcd.begin(16, 2);
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("Scan Your Card");
Serial.println("Ready to scan RFID cards.");
}
void sendSMS(String userName) {
if (WiFi.status() == WL_CONNECTED) {
WiFiClientSecure client;
client.setInsecure();
HTTPClient http;
String apiUrl = "https://www.circuitdigest.cloud/send_sms?ID=" + String(templateID);
http.begin(client, apiUrl);
http.addHeader("Authorization", apiKey);
http.addHeader("Content-Type", "application/json");
String payload = "{\"mobiles\":\"" + String(mobileNumber) + "\",\"var1\":\"" + userName + " Access\",\"var2\":\"Granted\"}";
Serial.println("Sending SMS: " + payload);
int httpResponseCode = http.POST(payload);
if (httpResponseCode == 200) {
Serial.println("SMS sent successfully!");
} else {
Serial.print("Failed to send SMS. Error code: ");
Serial.println(httpResponseCode);
}
http.end();
} else {
Serial.println("WiFi not connected! Cannot send SMS.");
}
}
void loop() {
if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
Serial.print("Card detected! UID: ");
String uid = "";
for (byte i = 0; i < rfid.uid.size; i++) {
uid += String(rfid.uid.uidByte[i], HEX);
}
Serial.println(uid);
String userName = getUserName(uid);
Serial.println("User Identified: " + userName);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Hello, " + userName);
lcd.setCursor(0, 1);
lcd.print("ID: " + uid);
sendSMS(userName);
delay(3000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Scan Your Card");
Serial.println("Ready for next scan.");
}
}



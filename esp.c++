#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>

const char* ssid = "DEIN_SSID";         // Dein WLAN Name
const char* password = "DEIN_PASSWORT"; // Dein WLAN Passwort

WebServer server(80);
Servo myservo;

bool isOpen = false; // Status merken

void setup() {
  Serial.begin(115200);

  myservo.attach(18);  // Servo Signal an GPIO 18

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nVerbunden: ");
  Serial.println(WiFi.localIP());
  
  // Servo Toggle Handler
  server.on("/toggle", []() {
    if (isOpen) {
      myservo.write(0);    // Fenster/Tür schließen
    } else {
      myservo.write(90);   // Fenster/Tür öffnen
    }
    isOpen = !isOpen;
    server.send(200, "text/plain", "OK");
  });

  // Servo Reset Handler
  server.on("/reset", []() {
    myservo.write(0);    // Immer auf 0 Grad zurück
    isOpen = false;
    server.send(200, "text/plain", "Reset OK");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}

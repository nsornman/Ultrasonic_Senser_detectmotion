#include <WiFi.h>
#include <HTTPClient.h>

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  
  return pulseIn(echoPin, HIGH);
}

const char* ssid = "Namo10.5G";
const char* password = "";
const char* token = "sCKUqCcTm8HGZK5tpP5yrPw5XahnbZb8wwzf4ytRUpj";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println(F("Ultrasonic Distance Sensor Test"));

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.println("READY");
}

void loop() {
  float u = 0.016997492221 * readUltrasonicDistance(25, 26); 
  
  if (u > 0 && u <= 200) {  
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin("https://notify-api.line.me/api/notify");
      http.addHeader("Authorization", "Bearer " + String(token));
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      String message1 = "Motion detection has been triggered. \n";
      String message2 = "Human height: ";
      String message3 = String(u);
      String message4 = " CM \n";
      const char* link = "โปรดคลิ๊กลิงค์นี้เพื่อยืนยัน : http://127.0.0.1:5500/pbl.html";

      int httpCode = http.POST("message=" + message1 + message2 + message3 + message4 + link);
      
      if (httpCode > 0) {
        String response = http.getString();
        Serial.print("HTTP Response code: ");
        Serial.println(httpCode);
        Serial.print("Server response: ");
        Serial.println(response);
        Serial.print("Distance: ");
        Serial.println(u);
      } else {
        Serial.println("Error in HTTP request");
      }
      http.end();
    } else {
      Serial.println("WiFi not connected");
    }
  } else { 
    Serial.println("Distance exceeds 200 cm, no notification sent.");
  }
  
  delay(1000); 
}
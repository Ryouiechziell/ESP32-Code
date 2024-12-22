#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include <HTTPClient.h>

#define pin36 36;
#define pin39 39;
#define pin34 34;
#define pin35 35;
#define pin32 32;
#define pin33 33;
#define pin25 25;
#define pin26 26;
#define pin27 27;
#define pin14 14;
#define pin12 12;

#define pin13 13;
#define pin15 15;
#define pin2 2;
#define pin0 0;
#define pin4 4;
#define pin16 16;
#define pin17 17;
#define pin5 5;
#define pin18 18;
#define pin19 19;
#define pin21 21;
#define pin22 22;
#define pin23 23;

// Configuration Wifi
const char* ssid = "Giovanna";
const char* password = "muhammadnazril";

// Create objek server
AsyncWebServer server(80);
Servo myServo;
HTTPClient http;

// Handle operation
String handlePinOperation(int pin, String operation, String value = "") {
  if (operation == "write_analog") {
    int analogValue = value.toInt();
    lcdcWrite(0, analogValue);
    return "Wrote analog " + String(analogValue) + " to pin " + String(pin);
  } else if (operation == "read_analog") {
    int analogValue = analogRead(pin);
    return "Read analog " + String(analogValue) + " from pin " + String(pin);
  } else if (operation == "write_digital") {
    int digitalValue = (value == "HIGH") ? HIGH : LOW;
    digitalWrite(pin, digitalValue);
    return "Set pin " + String(pin) + " to " + (digitalValue == HIGH ? "HIGH" : "LOW");
  } else if (operation == "read_digital") {
    int digitalValue = digitalRead(pin);
    return "Read digital " + String(digitalValue == HIGH ? "HIGH" : "LOW") + " from pin " + String(pin);
  }
  return "Invalid operation for pin " + String(pin);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("Terhubung ke WiFi");
  Serial.println(WiFi.localIP());

  // Set pin mode
  pinMode(pin36, INPUT); // analog 0
  pinMode(pin39, INPUT); // analog 3
  pinMode(pin34, INPUT); // analog 6
  pinMode(pin35, INPUT); // analog 7
  pinMode(pin32, INPUT); // analog 4
  pinMode(pin33, INPUT); // analog 5
  pinMode(pin25, INPUT); // analog 18
  pinMode(pin26, INPUT); // analog 19
  pinMode(pin27, INPUT); // analog 17
  pinMode(pin14, INPUT); // analog 16
  pinMode(pin12, INPUT); // analog 15
  pinMode(pin13, INPUT); // analog 14

  pinMode(pin15, OUTPUT); // analog 13
  pinMode(pin2, OUTPUT); // analog 12
  pinMode(pin0, OUTPUT); // analog 11
  pinMode(pin4, OUTPUT); // analog 10
  pinMode(pin16, OUTPUT);
  pinMode(pin17, OUTPUT);
  pinMode(pin5, OUTPUT);
  pinMode(pin18, OUTPUT);
  pinMode(pin19, OUTPUT);
  pinMode(pin21, OUTPUT);
  pinMode(pin22, OUTPUT);
  pinMode(pin23, OUTPUT);

  myServo.attach(18);
  
  // Endpoint untuk mengendalikan beberapa pin
  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request){

    // Membuat objek JSON
    StaticJsonDocument<1024> doc;

    // Menangani pin 36
    if (request->hasParam("pin36")) {
      String pin_operation = request->getParam("pin36")->value();
      String pin_value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin36"] = handlePinOperation(pin36, pin_operation, value);
    }
    // Menangani pin 39
    if (request->hasParam("pin39")) {
      String pin_operation = request->getParam("pin39")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin39"] = handlePinOperation(pin39, pin_operation, value);
    }
    // Menangani pin 34
    if (request->hasParam("pin34")) {
      String pin_operation = request->getParam("pin34")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin34"] = handlePinOperation(pin34, pin_operation, value);
    }
    // Menangani pin 35
    if (request->hasParam("pin35")) {
      String pin_operation = request->getParam("pin35")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin35"] = handlePinOperation(pin35, pin_operation, value);
    }
    // Menangani pin 32
    if (request->hasParam("pin32")) {
      String pin_operation = request->getParam("pin32")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin32"] = handlePinOperation(pin32, pin_operation, value);
    }
    // Menangani pin 33
    if (request->hasParam("pin33")) {
      String pin_operation = request->getParam("pin33")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin33"] = handlePinOperation(pin33, pin_operation, value);
    }
                // Menangani pin 25
    if (request->hasParam("pin25")) {
      String pin_operation = request->getParam("pin25")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin25"] = handlePinOperation(pin25, pin_operation, value);
    }
                // Menangani pin 26
    if (request->hasParam("pin26")) {
      String pin_operation = request->getParam("pin26")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin26"] = handlePinOperation(pin26, pin_operation, value);
    }
                // Menangani pin 27
    if (request->hasParam("pin27")) {
      String pin_operation = request->getParam("pin27")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin27"] = handlePinOperation(pin27, pin_operation, value);
    }
                // Menangani pin 14
    if (request->hasParam("pin14")) {
      String pin_operation = request->getParam("pin14")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin14"] = handlePinOperation(pin14, pin_operation, value);
    }
                // Menangani pin 12
    if (request->hasParam("pin12")) {
      String pin_operation = request->getParam("pin12")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin12"] = handlePinOperation(pin12, pin_operation, value);
    }
                // Menangani pin 13
    if (request->hasParam("pin13")) {
      String pin_operation = request->getParam("pin13")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin13"] = handlePinOperation(pin13, pin_operation, value);
    }
                // Menangani pin 15
    if (request->hasParam("pin15")) {
      String pin_operation = request->getParam("pin15")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin15"] = handlePinOperation(pin15, pin_operation, value);
    }
                // Menangani pin 2
    if (request->hasParam("pin2")) {
      String pin_operation = request->getParam("pin2")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin2"] = handlePinOperation(pin2, pin_operation, value);
    }
                // Menangani pin 0
    if (request->hasParam("pin0")) {
      String pin_operation = request->getParam("pin0")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin0"] = handlePinOperation(pin0, pin_operation, value);
    }
                // Menangani pin 4
    if (request->hasParam("pin4")) {
      String pin_operation = request->getParam("pin4")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin4"] = handlePinOperation(pin4, pin_operation, value);
    }
                // Menangani pin 16
    if (request->hasParam("pin16")) {
      String pin_operation = request->getParam("pin16")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin16"] = handlePinOperation(pin16, pin_operation, value);
    }
                // Menangani pin 17
    if (request->hasParam("pin17")) {
      String pin_operation = request->getParam("pin17")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin17"] = handlePinOperation(pin17, pin_operation, value);
    }
                // Menangani pin 5
    if (request->hasParam("pin5")) {
      String pin_operation = request->getParam("pin5")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin5"] = handlePinOperation(pin5, pin_operation, value);
    }
                // Menangani pin 18
    if (request->hasParam("pin18")) {
      String pin_operation = request->getParam("pin18")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin18"] = handlePinOperation(pin18, pin_operation, value);
    }
                // Menangani pin 19
    if (request->hasParam("pin19")) {
      String pin_operation = request->getParam("pin19")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin19"] = handlePinOperation(pin19, pin_operation, value);
    }
                // Menangani pin 21
    if (request->hasParam("pin21")) {
      String pin_operation = request->getParam("pin21")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin21"] = handlePinOperation(pin21, pin_operation, value);
    }
                // Menangani pin 22
    if (request->hasParam("pin22")) {
      String pin_operation = request->getParam("pin22")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin22"] = handlePinOperation(pin22, pin_operation, value);
    }
                // Menangani pin 23
    if (request->hasParam("pin23")) {
      String pin_operation = request->getParam("pin23")->value();
      String value = request->hasParam("value") ? request->getParam("value")->value() : "";
      doc["pin23"] = handlePinOperation(pin23, pin_operation, value);
    }

    // Mengonversi objek JSON ke string
    String jsonResponse;
    serializeJson(doc, jsonResponse);

    // Kirim respons dalam format JSON
    request->send(200, "application/json", jsonResponse);
  });

  // Endpoint untuk mengontrol servo
server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request){
  String angle = request->hasParam("angle") ? request->getParam("angle")->value() : "";
  StaticJsonDocument<256> doc;

  if (angle != "") {
      int angleValue = angle.toInt();
      if (angleValue >= 0 && angleValue <= 180) {
          myServo.write(angleValue); // Set posisi servo
          doc["servo"] = "Position set to " + String(angleValue) + " degrees.";
      } else {
          doc["error"] = "Invalid angle. Please provide a value between 0 and 180.";
      }
  } else {
      doc["error"] = "Missing angle parameter.";
  }

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  request->send(200, "application/json", jsonResponse);
});

// Endpoint untuk membaca sensor ultrasonic dengan pin yang dinamis
server.on("/ultrasonic", HTTP_GET, [](AsyncWebServerRequest *request){
  // Mengambil parameter pin trig dan echo
  String trigPinParam = request->hasParam("trigPin") ? request->getParam("trigPin")->value() : "";
  String echoPinParam = request->hasParam("echoPin") ? request->getParam("echoPin")->value() : "";

  StaticJsonDocument<256> doc;

  if (trigPinParam != "" && echoPinParam != "") {
      int trigPin = trigPinParam.toInt();
      int echoPin = echoPinParam.toInt();

      // Mengirimkan pulsa trig
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      // Membaca durasi pulsa dari echo
      long duration = pulseIn(echoPin, HIGH);
      long distance = duration * 0.0344 / 2;  // Kecepatan suara: 343 m/s (0.0344 cm/µs)

      doc["distance"] = distance;
  } else {
      doc["error"] = "Missing trigPin or echoPin parameter.";
  }

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  request->send(200, "application/json", jsonResponse);
});

// Endpoint untuk mengirim HTTP berdasarkan parameter
server.on("/http", HTTP_GET, [](AsyncWebServerRequest *request){
  String url = request->hasParam("url") ? request->getParam("url")->value() : "";
  String method = request->hasParam("method") ? request->getParam("method")->value() : "GET"; // Default to GET
  String payload = request->hasParam("payload") ? request->getParam("payload")->value() : "";
    
  StaticJsonDocument<512> doc;
    
  if (url != "") {
    http.begin(url);  // Inisialisasi koneksi ke URL yang diberikan

    // Tentukan metode HTTP yang akan digunakan (GET/POST)
    if (method == "POST") {
      http.addHeader("Content-Type", "application/json"); // Contoh header untuk POST request
      int httpResponseCode = http.POST(payload); // Kirimkan payload sebagai POST
      if (httpResponseCode > 0) {
          doc["status"] = "Success";
          doc["responseCode"] = httpResponseCode;
          doc["response"] = http.getString(); // Mendapatkan respons dari server
      } else {
          doc["error"] = "HTTP POST request failed";
          doc["responseCode"] = httpResponseCode;
        }
    } else {
        int httpResponseCode = http.GET();  // Kirimkan GET request
        if (httpResponseCode > 0) {
          doc["status"] = "Success";
          doc["responseCode"] = httpResponseCode;
          doc["response"] = http.getString(); // Mendapatkan respons dari server
        } else {
            doc["error"] = "HTTP GET request failed";
            doc["responseCode"] = httpResponseCode;
        }
    }
    http.end(); // Tutup koneksi HTTP
  } else {
      doc["error"] = "Missing URL parameter.";
  }

  // Mengonversi objek JSON ke string
  String jsonResponse;
  serializeJson(doc, jsonResponse);

  // Kirim respons dalam format JSON
  request->send(200, "application/json", jsonResponse);
});
  
  // Memulai server
  server.begin();
}

void loop() {

}

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Servo.h>

// Definisi pin ESP32 yang sudah diatur sebelumnya
#define pin36 36
#define pin39 39
#define pin34 34
#define pin35 35
#define pin32 32
#define pin33 33
#define pin25 25
#define pin26 26
#define pin27 27
#define pin14 14
#define pin12 12
#define pin13 13
#define pin15 15
#define pin2 2
#define pin0 0
#define pin4 4
#define pin16 16
#define pin17 17
#define pin5 5
#define pin18 18
#define pin19 19
#define pin21 21
#define pin22 22
#define pin23 23

// Variabel baru untuk pin tambahan
int pinServo = pin23;         // Pin untuk servo
int pinBuzzer = pin22;        // Pin untuk buzzer
int pinSensorPir = pin34;     // Pin untuk sensor PIR
int pinTrig = pin12;          // Pin untuk trig sensor ultrasonic
int pinEcho = pin13;          // Pin untuk echo sensor ultrasonic

// Pin LED untuk indikator
int pinLedMerah = pin25;      // Pin LED Merah
int pinLedBiru = pin26;       // Pin LED Biru

// Pin untuk angle servo
int servoAngle = 0;           // Variabel untuk sudut servo
int validDistance = 20;       // Variabel untuk jarak validasi ultrasonic

// Konfigurasi WiFi
const char* ssid = "Giovanna";
const char* password = "muhammadnazril";

// Membuat objek server dan servo
WebServer server(80);
HTTPClient http;
Servo myServo;  // Objek servo

// Flag untuk kontrol
bool flagSensorPir = false;  // Flag untuk sensor PIR
bool flagUltrasonic = false; // Flag untuk kontrol ultrasonic

// Fungsi untuk menangani operasi pin
String handlePinOperation(int pin, String operation, String value = "") {
  if (operation == "write_analog") {
    int analogValue = value.toInt();
    analogWrite(pin, analogValue);
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

  // Set mode pin untuk ESP32
  for (int pin : {pin36, pin39, pin34, pin35, pin32, pin33, pin25, pin26, pin27, pin14, pin12, pin13}) {
    pinMode(pin, INPUT);
  }
  for (int pin : {pin15, pin2, pin0, pin4, pin16, pin17, pin5, pin18, pin19, pin21, pin22, pin23}) {
    pinMode(pin, OUTPUT);
  }

  // Inisialisasi servo
  myServo.attach(pinServo);

  // Endpoint untuk mengontrol pin
  server.on("/control", HTTP_GET, []() {
    StaticJsonDocument<1024> doc;

    for (int pin : {pin36, pin39, pin34, pin35, pin32, pin33, pin25, pin26, pin27, pin14, pin12, pin13,
                    pin15, pin2, pin0, pin4, pin16, pin17, pin5, pin18, pin19, pin21, pin22, pin23}) {
      String pinName = "pin" + String(pin);
      if (server.hasArg(pinName)) {
        String operation = server.arg(pinName);
        String value = server.hasArg("value") ? server.arg("value") : "";
        doc[pinName] = handlePinOperation(pin, operation, value);
      }
    }

    String jsonResponse;
    serializeJson(doc, jsonResponse);
    server.send(200, "application/json", jsonResponse);
  });

  // Endpoint untuk mengubah flag sensor PIR
  server.on("/setFlagSensorPir", HTTP_GET, []() {
    if (server.hasArg("flag")) {
      flagSensorPir = server.arg("flag") == "1";  // Mengatur flag PIR berdasarkan parameter
    }
  });

  // Endpoint untuk mengubah flag ultrasonic
  server.on("/setFlagUltrasonic", HTTP_GET, []() {
    if (server.hasArg("flag")) {
      flagUltrasonic = server.arg("flag") == "1";  // Mengatur flag ultrasonic berdasarkan parameter
    }
  });

  // Endpoint untuk mengatur jarak validasi ultrasonic dan sudut servo
  server.on("/setVariables", HTTP_GET, []() {
    if (server.hasArg("distance")) {
      validDistance = server.arg("distance").toInt();  // Mengubah jarak validasi
    }
    if (server.hasArg("angle")) {
      servoAngle = server.arg("angle").toInt();  // Mengubah sudut servo
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();

  // Jika flag sensor PIR aktif, baca sensor PIR dan hidupkan buzzer
  if (flagSensorPir) {
    int sensorPIR = digitalRead(pinSensorPir);  // Membaca sensor PIR
    if (sensorPIR == HIGH) {
      digitalWrite(pinBuzzer, HIGH);  // Hidupkan buzzer jika ada gerakan
      digitalWrite(pinLedMerah, HIGH); // LED merah menyala
      digitalWrite(pinLedBiru, LOW);  // LED biru mati
    } else {
      digitalWrite(pinBuzzer, LOW);   // Matikan buzzer jika tidak ada gerakan
      digitalWrite(pinLedMerah, LOW); // LED merah mati
      digitalWrite(pinLedBiru, HIGH); // LED biru menyala
    }
  }

  // Jika flag ultrasonic aktif, baca sensor ultrasonic dan gerakkan servo sesuai angle
  if (flagUltrasonic) {
    long duration, distance;

    // Menghitung jarak menggunakan sensor ultrasonic
    digitalWrite(pinTrig, LOW);
    delayMicroseconds(2);
    digitalWrite(pinTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrig, LOW);

    duration = pulseIn(pinEcho, HIGH);
    distance = duration * 0.0344 / 2;

    // Jika jarak kurang dari jarak validasi, gerakkan servo ke sudut yang ditentukan oleh servoAngle
    if (distance < validDistance) {
      myServo.write(servoAngle);  // Gunakan sudut servo dari variabel angle
    }
  }
}

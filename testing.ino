#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>

// **Konfigurasi WiFi**
const char* ssid = "ESP32";
const char* password = "0123456789";

#define RXD2 16
#define TXD2 17
#define pinLDR 34  
#define pinServo 23
#define pinSensorPir 35
#define pinTrig 12
#define pinEcho 13
#define pinSound 36  // Pin untuk sensor suara

// **Konfigurasi Motor L298N**
#define motor1A 32  
#define motor1B 33  
#define motor2A 25  
#define motor2B 26  
#define pwm1 27     
#define pwm2 14     

// **Konstanta PWM**
#define PWM_CHANNEL_1 0
#define PWM_CHANNEL_2 1
#define PWM_FREQUENCY 1000
#define PWM_RESOLUTION 8  

// **Objek WebServer, DFPlayer Mini, Servo**
WebServer server(80);
HardwareSerial mySerial(2);
DFRobotDFPlayerMini mp3;
ESP32Servo myServo;

void handleLDRSensor() {
  int ldrValue = analogRead(pinLDR);

  StaticJsonDocument<200> doc;
  doc["status"] = "success";
  doc["ldr_value"] = ldrValue;

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

// 📌 **Fungsi Mengontrol Servo**
void handleServoControl() {
  if (server.hasArg("angle")) {
    int servoAngle = server.arg("angle").toInt();
    myServo.write(servoAngle);

    StaticJsonDocument<200> doc;
    doc["status"] = "success";
    doc["angle"] = servoAngle;

    String jsonResponse;
    serializeJson(doc, jsonResponse);
    server.send(200, "application/json", jsonResponse);
  } else {
    server.send(400, "application/json", "{\"error\":\"Missing angle parameter\"}");
  }
}

// 📌 **Fungsi Membaca Sensor PIR**
void handlePirSensor() {
  int sensorPIR = digitalRead(pinSensorPir);

  StaticJsonDocument<200> doc;
  doc["status"] = "success";
  doc["pir_state"] = sensorPIR == HIGH ? "Detected" : "Not Detected";

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

// 📌 **Fungsi Membaca Sensor Ultrasonic**
void handleUltrasonicSensor() {
  long duration, distance;

  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  duration = pulseIn(pinEcho, HIGH);
  distance = duration * 0.0344 / 2;

  StaticJsonDocument<200> doc;
  doc["status"] = "success";
  doc["distance_cm"] = distance;

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

// 📌 **Fungsi Mengontrol DFPlayer Mini**
void handleMP3Control() {
  if (!server.hasArg("command")) {
    server.send(400, "application/json", "{\"error\":\"Missing command parameter\"}");
    return;
  }

  String command = server.arg("command");
  StaticJsonDocument<200> doc;
  doc["status"] = "success";

  if (command == "play") {
    if (server.hasArg("track")) {
      int trackNumber = server.arg("track").toInt();
      mp3.play(trackNumber);
      doc["message"] = "Playing track " + String(trackNumber);
    } else {
      server.send(400, "application/json", "{\"error\":\"Missing track parameter\"}");
      return;
    }
  } else if (command == "pause") {
    mp3.pause();
    doc["message"] = "Paused";
  } else if (command == "resume") {
    mp3.start();
    doc["message"] = "Resumed";
  } else if (command == "stop") {
    mp3.stop();
    doc["message"] = "Stopped";
  } else if (command == "next") {
    mp3.next();
    doc["message"] = "Next track";
  } else if (command == "previous") {
    mp3.previous();
    doc["message"] = "Previous track";
  } else {
    server.send(400, "application/json", "{\"error\":\"Invalid command\"}");
    return;
  }

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

// 📌 **Fungsi Handle Sound Detector**
void handleSoundDetected() {
  Serial.println("Suara terdeteksi!");

  StaticJsonDocument<200> doc;
  doc["status"] = "success";
  doc["message"] = "Sound detected";

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

// 📌 **Setup ESP32**
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke WiFi");
  Serial.println(WiFi.localIP());

  // **Set pin sebagai OUTPUT**
  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(pinLDR, INPUT);
  pinMode(pinSensorPir, INPUT);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  pinMode(pinSound, INPUT);  // Inisialisasi pin sound detector

  // **Setup PWM**
  ledcSetup(PWM_CHANNEL_1, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcSetup(PWM_CHANNEL_2, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(pwm1, PWM_CHANNEL_1);
  ledcAttachPin(pwm2, PWM_CHANNEL_2);

  // **Setup Servo**
  myServo.attach(pinServo);

  // **Setup DFPlayer Mini**
  mySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  if (!mp3.begin(mySerial)) {
    Serial.println("Gagal menghubungkan DFPlayer Mini!");
    while (true);
  }
  mp3.volume(20);

  // **Tambahkan endpoint**
  server.on("/ldr", HTTP_GET, handleLDRSensor);
  server.on("/servo", HTTP_GET, handleServoControl);
  server.on("/pir", HTTP_GET, handlePirSensor);
  server.on("/ultrasonic", HTTP_GET, handleUltrasonicSensor);
  server.on("/mp3", HTTP_GET, handleMP3Control);
  server.on("/car", HTTP_GET, handleCarControl);
  server.on("/sound", HTTP_GET, handleSoundDetected);  // Tambahkan endpoint sound detector

  server.begin();
}

// 📌 **Loop Utama**
void loop() {
  server.handleClient();
}

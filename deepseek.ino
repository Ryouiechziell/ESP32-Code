#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>

// **Konfigurasi WiFi**
const char* ssid = "ESP32";
const char* password = "0123456789";

// **Pin Definitions**
#define RXD2 16
#define TXD2 17
#define PIN_LDR 34
#define PIN_SERVO 22
#define PIN_PIR 35
#define PIN_TRIG 12
#define PIN_ECHO 13
#define PIN_SOUND 36

// **Motor Pins**
#define MOTOR1_A 32
#define MOTOR1_B 33
#define MOTOR2_A 25
#define MOTOR2_B 26
#define PWM1 27
#define PWM2 14

// **PWM Configuration**
#define PWM_CHANNEL_1 0
#define PWM_CHANNEL_2 1
#define PWM_FREQ 1000
#define PWM_RES 8

// **Global Objects**
WebServer server(80);
HardwareSerial mySerial(2);
DFRobotDFPlayerMini mp3;
ESP32Servo myServo;

int mp3Volume = 20;

// **Enum for Motor Commands**
enum MotorCommand {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  STOP
};

// **Enum for MP3 Commands**
enum MP3Command {
  PLAY,
  PAUSE,
  RESUME,
  STOP_MP3,
  NEXT,
  PREVIOUS
};

// **Setup Functions**
void setupWiFi() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.println(WiFi.localIP());
}

void setupMotor() {
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);

  ledcSetup(PWM_CHANNEL_1, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CHANNEL_2, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM1, PWM_CHANNEL_1);
  ledcAttachPin(PWM2, PWM_CHANNEL_2);
}

void setupSensors() {
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_SOUND, INPUT);
}

void setupServo() {
  myServo.attach(PIN_SERVO);
}

void setupDFPlayer() {
  mySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  if (!mp3.begin(mySerial)) {
    Serial.println("Failed to connect to DFPlayer Mini!");
    while (true);
  }
  mp3.volume(mp3Volume);
}

void setupServer() {
  server.on("/ldr", HTTP_GET, handleLDRSensor);
  server.on("/servo", HTTP_GET, handleServoControl);
  server.on("/pir", HTTP_GET, handlePirSensor);
  server.on("/ultrasonic", HTTP_GET, handleUltrasonicSensor);
  server.on("/mp3", HTTP_GET, handleMP3Control);
  server.on("/car", HTTP_GET, handleCarControl);
  server.on("/sound", HTTP_GET, handleSoundDetected);
  server.on("/set", HTTP_GET, handleSetVariable);

  server.begin();
}

// **Handler Functions**
void handleLDRSensor() {
  int ldrValue = analogRead(PIN_LDR);
  sendResponse(200, "ldr_value", ldrValue);
}

void handleServoControl() {
  if (!server.hasArg("angle")) {
    sendError(400, "Missing angle parameter");
    return;
  }

  int angle = server.arg("angle").toInt();
  myServo.write(angle);
  sendResponse(200, "angle", angle);
}

void handlePirSensor() {
  int pirState = digitalRead(PIN_PIR);
  sendResponse(200, "pir_state", pirState == HIGH ? "Detected" : "Not Detected");
}

void handleUltrasonicSensor() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duration = pulseIn(PIN_ECHO, HIGH);
  long distance = duration * 0.0344 / 2;
  sendResponse(200, "distance_cm", distance);
}

void handleMP3Control() {
  if (!server.hasArg("command")) {
    sendError(400, "Missing command parameter");
    return;
  }

  String command = server.arg("command");
  if (command == "play" && server.hasArg("track")) {
    int track = server.arg("track").toInt();
    mp3.play(track);
    sendResponse(200, "message", "Playing track " + String(track));
  } else if (command == "pause") {
    mp3.pause();
    sendResponse(200, "message", "Paused");
  } else if (command == "resume") {
    mp3.start();
    sendResponse(200, "message", "Resumed");
  } else if (command == "stop") {
    mp3.stop();
    sendResponse(200, "message", "Stopped");
  } else if (command == "next") {
    mp3.next();
    sendResponse(200, "message", "Next track");
  } else if (command == "previous") {
    mp3.previous();
    sendResponse(200, "message", "Previous track");
  } else {
    sendError(400, "Invalid command");
  }
}

void handleCarControl() {
  if (!server.hasArg("command")) {
    sendError(400, "Missing command parameter");
    return;
  }

  String command = server.arg("command");
  if (command == "forward") {
    setMotor(FORWARD);
    sendResponse(200, "message", "Moving forward");
  } else if (command == "backward") {
    setMotor(BACKWARD);
    sendResponse(200, "message", "Moving backward");
  } else if (command == "left") {
    setMotor(LEFT);
    sendResponse(200, "message", "Turning left");
  } else if (command == "right") {
    setMotor(RIGHT);
    sendResponse(200, "message", "Turning right");
  } else if (command == "stop") {
    setMotor(STOP);
    sendResponse(200, "message", "Stopped");
  } else {
    sendError(400, "Invalid command");
  }
}

void handleSoundDetected() {
  Serial.println("Sound detected!");
  sendResponse(200, "message", "Sound detected");
}

void handleSetVariable() {
  if (!server.hasArg("name") || !server.hasArg("value")) {
    sendError(400, "Missing parameters");
    return;
  }

  String varName = server.arg("name");
  int varValue = server.arg("value").toInt();

  if (varName == "mp3_volume") {
    if (varValue < 0 || varValue > 30) {
      sendError(400, "Volume must be between 0 and 30");
      return;
    }
    mp3Volume = varValue;
    mp3.volume(mp3Volume);
    sendResponse(200, "message", "MP3 volume set to " + String(mp3Volume));
  } else {
    sendError(400, "Unknown variable");
  }
}

// **Helper Functions**
void sendResponse(int statusCode, const String& key, const String& value) {
  StaticJsonDocument<200> doc;
  doc["status"] = "success";
  doc[key] = value;

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(statusCode, "application/json", jsonResponse);
}

void sendError(int statusCode, const String& message) {
  StaticJsonDocument<200> doc;
  doc["status"] = "error";
  doc["message"] = message;

  String jsonResponse;
  serializeJson(doc, jsonResponse);
  server.send(statusCode, "application/json", jsonResponse);
}

void setMotor(MotorCommand command) {
  switch (command) {
    case FORWARD:
      digitalWrite(MOTOR1_A, HIGH);
      digitalWrite(MOTOR1_B, LOW);
      digitalWrite(MOTOR2_A, HIGH);
      digitalWrite(MOTOR2_B, LOW);
      break;
    case BACKWARD:
      digitalWrite(MOTOR1_A, LOW);
      digitalWrite(MOTOR1_B, HIGH);
      digitalWrite(MOTOR2_A, LOW);
      digitalWrite(MOTOR2_B, HIGH);
      break;
    case LEFT:
      digitalWrite(MOTOR1_A, LOW);
      digitalWrite(MOTOR1_B, HIGH);
      digitalWrite(MOTOR2_A, HIGH);
      digitalWrite(MOTOR2_B, LOW);
      break;
    case RIGHT:
      digitalWrite(MOTOR1_A, HIGH);
      digitalWrite(MOTOR1_B, LOW);
      digitalWrite(MOTOR2_A, LOW);
      digitalWrite(MOTOR2_B, HIGH);
      break;
    case STOP:
      digitalWrite(MOTOR1_A, LOW);
      digitalWrite(MOTOR1_B, LOW);
      digitalWrite(MOTOR2_A, LOW);
      digitalWrite(MOTOR2_B, LOW);
      break;
  }
}

// **Setup and Loop**
void setup() {
  setupWiFi();
  setupMotor();
  setupSensors();
  setupServo();
  setupDFPlayer();
  setupServer();
}

void loop() {
  server.handleClient();
}

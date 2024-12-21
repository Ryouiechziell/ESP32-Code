#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>

// Ganti dengan kredensial WiFi Anda
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Tentukan pin yang digunakan untuk servo
const int servoPin = 34;  // Pin servo
Servo myServo;  // Membuat objek servo

// Membuat objek server
AsyncWebServer server(80);

void setup() {
  // Memulai serial monitor untuk debug
  Serial.begin(115200);

  // Menghubungkan ESP32 ke WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Mencoba koneksi WiFi...");
  }
  Serial.println("Terhubung ke WiFi");

  // Menampilkan alamat IP server
  Serial.println(WiFi.localIP());

  // Menyambungkan servo ke pin
  myServo.attach(servoPin);

  // Menangani endpoint untuk menggerakkan servo
  server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request){
    String result = "Error: No angle parameter provided";

    if (request->hasParam("angle")) {
      String angle_str = request->getParam("angle")->value();
      int angle = angle_str.toInt();  // Mengubah nilai angle menjadi integer

      if (angle >= 0 && angle <= 180) {
        myServo.write(angle);  // Menggerakkan servo ke sudut yang diinginkan
        result = "Servo moved to angle: " + String(angle);
      } else {
        result = "Error: Angle must be between 0 and 180";
      }
    }

    // Kirim respons kembali sebagai teks
    request->send(200, "text/plain", result);
  });

  // Memulai server
  server.begin();
}

void loop() {
  // Tidak ada logika di loop() karena server berbasis event-driven
}

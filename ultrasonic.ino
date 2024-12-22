// Menentukan pin untuk sensor Ultrasonik
const int trigPin = 18;
const int echoPin = 12;

// Variabel untuk menyimpan waktu dan jarak
long duration;
int distance;

void setup() {
  // Memulai komunikasi serial
  Serial.begin(9600);

  // Mengatur pin sebagai input dan output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Membersihkan trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Mengirimkan pulsa trig
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Membaca durasi pulsa echo
  duration = pulseIn(echoPin, HIGH);

  // Menghitung jarak (dalam cm)
  distance = duration * 0.0344 / 2;

  // Menampilkan jarak di serial monitor
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Menunggu sebelum membaca jarak lagi
  delay(500);
}

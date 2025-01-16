// Endpoint untuk menyetel pin tambahan
server.on("/setPin", HTTP_GET, []() {
  if (server.hasArg("pinType") && server.hasArg("pinNumber")) {
    String pinType = server.arg("pinType");
    int pinNumber = server.arg("pinNumber").toInt();

    if (pinType == "buzzer") {
      pinBuzzer = pinNumber;
    } else if (pinType == "echo") {
      pinEcho = pinNumber;
    } else if (pinType == "trigger") {
      pinTrig = pinNumber;
    } else if (pinType == "pir") {
      pinSensorPir = pinNumber;
    } else if (pinType == "ledMerah") {
      pinLedMerah = pinNumber;
    } else if (pinType == "ledBiru") {
      pinLedBiru = pinNumber;
    }
    // Tidak ada respon seperti yang diminta
  }
});

const long intervalRed = 1000;
unsigned long previousTimeRed = 0;
int ledState = LOW;
int redPin = 14;

void setup() {
  pinMode(redPin, OUTPUT);
}

void loop() {
  const long currentTime = millis();
  if (currentTime - previousTimeRed >= intervalRed) {
    previousTimeRed = currentTime;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
  }
  digitalWrite(redPin, ledState);
}
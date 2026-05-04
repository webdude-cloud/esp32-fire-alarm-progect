
const int callPin = 4;
const int silencePin = 18;
const int resetPin = 5;

const int ledPin = 2;
const int buzzerPin = 23;

bool alarmActive = false;
bool silenced = false;

// button tracking
bool lastCall = HIGH;
bool lastSilence = HIGH;
bool lastReset = HIGH;

// alert tone timing
unsigned long lastToneTime = 0;
int toneStep = 0;

void setup() {
  pinMode(callPin, INPUT_PULLUP);
  pinMode(silencePin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
}

void loop() {

  bool call = digitalRead(callPin);
  bool silence = digitalRead(silencePin);
  bool reset = digitalRead(resetPin);

  // 🔴 CALL
  if (lastCall == HIGH && call == LOW) {
    alarmActive = true;
    silenced = false;
  }

  // 🔘 SILENCE toggle
  if (lastSilence == HIGH && silence == LOW) {
    if (alarmActive) {
      silenced = !silenced;
    }
    delay(200);
  }

  // 🔄 RESET
  if (lastReset == HIGH && reset == LOW) {

    alarmActive = false;
    silenced = false;

    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);

    delay(50);

    // chirp
    digitalWrite(buzzerPin, HIGH);
    delay(80);
    digitalWrite(buzzerPin, LOW);
  }

  lastCall = call;
  lastSilence = silence;
  lastReset = reset;

  // 💡 LED
  digitalWrite(ledPin, alarmActive);

  // 🔊 ALERT TONE
  if (alarmActive && !silenced) {

    unsigned long now = millis();

    if (now - lastToneTime > 500) {
      lastToneTime = now;
      toneStep++;

      if (toneStep > 3) toneStep = 0;

      // beep then pause pattern
      if (toneStep == 0) {
        digitalWrite(buzzerPin, HIGH);  // beep
      } else {
        digitalWrite(buzzerPin, LOW);   // silence
      }
    }

  } else {
    digitalWrite(buzzerPin, LOW);
  }
}
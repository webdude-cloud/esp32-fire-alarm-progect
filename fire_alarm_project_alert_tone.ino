
const int callPin = 4;
const int silencePin = 18;
const int resetPin = 5;

const int ledPin = 2;
const int buzzerPin = 23;

bool alarmActive = false;
bool silenced = false;

// button edge tracking
bool lastCall = HIGH;
bool lastSilence = HIGH;
bool lastReset = HIGH;

// hi-lo timing
unsigned long lastToneChange = 0;
bool toneState = false;

void setup() {
  pinMode(callPin, INPUT_PULLUP);
  pinMode(silencePin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
}

void loop() {

  bool call = digitalRead(callPin);
  bool silence = digitalRead(silencePin);
  bool reset = digitalRead(resetPin);

  // 🔴 CALL → latch alarm
  if (lastCall == HIGH && call == LOW) {
    alarmActive = true;
    silenced = false;
  }

  // 🔘 SILENCE → toggle
  if (lastSilence == HIGH && silence == LOW) {
    if (alarmActive) {
      silenced = !silenced;
    }
    delay(200);
  }

  // 🔄 RESET → clear system
  if (lastReset == HIGH && reset == LOW) {

    alarmActive = false;
    silenced = false;

    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);

    delay(50);

    // reset chirp
    tone(buzzerPin, 2500);
    delay(80);
    noTone(buzzerPin);
  }

  lastCall = call;
  lastSilence = silence;
  lastReset = reset;

  // 💡 LED
  digitalWrite(ledPin, alarmActive);

  // 🔊 HI-LO EVAC TONE
  if (alarmActive && !silenced) {

    unsigned long now = millis();

    if (now - lastToneChange > 630) {
      lastToneChange = now;
      toneState = !toneState;

      if (toneState) {
        tone(buzzerPin, 2000);   // HIGH tone
      } else {
        tone(buzzerPin, 1200);   // LOW tone
      }
    }

  } else {
    noTone(buzzerPin);
  }
}
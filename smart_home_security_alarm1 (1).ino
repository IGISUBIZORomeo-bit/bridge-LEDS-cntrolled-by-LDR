// ============================================================
// Smart Nova Solutions — Security Alarm System
// Arduino UNO | LDR Controlled (No Buttons)
// ============================================================

// --- Pin Definitions ---
const int RED_LED   = 2;   // Red LED: alarm (too dark = intruder risk)
const int GREEN_LED = 3;   // Green LED: all clear (enough light)
const int BUZZER    = 6;   // Buzzer: alarm sound
const int LDR_PIN   = A0;  // LDR: light sensor

// --- Thresholds ---
const int NIGHT_THRESHOLD = 600;  // Above this = dark (alarm triggers)
const int DAY_THRESHOLD   = 400;  // Below this = bright (alarm resets)
// Gap between the two prevents flickering

// --- State ---
bool alarmActive = false;

// Blink timing for red LED during alarm
unsigned long lastBlinkTime = 0;
const int BLINK_INTERVAL    = 200;
bool redLedState            = false;

// ============================================================
void setup() {
  pinMode(RED_LED,   OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER,    OUTPUT);

  Serial.begin(9600);

  // Start in safe (day) mode
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED,   LOW);
  noTone(BUZZER);

  Serial.println("MONITORING ACTIVE");
}

// ============================================================
void loop() {
  int light = analogRead(LDR_PIN);

  Serial.print("LDR: ");
  Serial.println(light);

  // --- DARK: trigger alarm ---
  if (light > NIGHT_THRESHOLD && !alarmActive) {
    triggerAlarm();
  }

  // --- BRIGHT: reset alarm automatically ---
  if (light < DAY_THRESHOLD && alarmActive) {
    resetAlarm();
  }

  // --- While alarm is active: blink red LED ---
  if (alarmActive) {
    unsigned long now = millis();
    if (now - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = now;
      redLedState = !redLedState;
      digitalWrite(RED_LED, redLedState ? HIGH : LOW);
    }
  }

  delay(100);
}

// ============================================================
// FUNCTION: triggerAlarm — called when it gets dark
// ============================================================
void triggerAlarm() {
  alarmActive = true;
  Serial.println("DARK DETECTED — ALARM ON");

  digitalWrite(GREEN_LED, LOW);
  tone(BUZZER, 1000);  // continuous beep
}

// ============================================================
// FUNCTION: resetAlarm — called when light returns
// ============================================================
void resetAlarm() {
  alarmActive = false;
  Serial.println("LIGHT RESTORED — SYSTEM RESET");

  digitalWrite(RED_LED,   LOW);
  noTone(BUZZER);

  digitalWrite(GREEN_LED, HIGH);
  Serial.println("MONITORING ACTIVE");
}
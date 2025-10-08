#include <LiquidCrystal.h>

const int dotBtn = 8;
const int dashBtn = 10;
const int ledPin = 11;

const int RS = 13, EN = 12, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

String morseInput = "";
unsigned long lastPressTime = 0;
unsigned long startTime = 0;

String morseCode[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
  "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.",
  "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-",
  "-.--", "--.."
};
char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const unsigned long debounceDelay = 50;
unsigned long lastDotPress = 0;
unsigned long lastDashPress = 0;

void setup() {
  pinMode(dotBtn, INPUT);
  pinMode(dashBtn, INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Morse Decoder");
  Serial.println("Morse Decoder");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  startTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();

  // Dot button (debounced)
  if (digitalRead(dotBtn) == HIGH && currentMillis - lastDotPress > debounceDelay) {
    morseInput += ".";
    blinkDot();
    lastPressTime = currentMillis;
    lastDotPress = currentMillis;
  }

  // Dash button (debounced)
  if (digitalRead(dashBtn) == HIGH && currentMillis - lastDashPress > debounceDelay) {
    morseInput += "-";
    blinkDash();
    lastPressTime = currentMillis;
    lastDashPress = currentMillis;
  }

  if (morseInput.length() > 0 && currentMillis - lastPressTime > 1000 && currentMillis - startTime > 3000) {
    String decoded = decodeMorse(morseInput);
    Serial.print(decoded);
    lcd.print(decoded);
    morseInput = "";
  }
}

void blinkDot() {
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(50);
}

void blinkDash() {
  digitalWrite(ledPin, HIGH);
  delay(600);
  digitalWrite(ledPin, LOW);
  delay(50);
}

String decodeMorse(String morse) {
  for (int i = 0; i < 26; i++) {
    if (morse == morseCode[i]) {
      return String(letters[i]);
    }
  }
  return "?";
}

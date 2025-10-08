#include <LiquidCrystal.h>

const int dotBtn = 8;
const int dashBtn = 10;
const int ledPin = 11;

const int RS = 13, EN = 12, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

String morseInput = "";
unsigned long lastPressTime = 0;
unsigned long startTime;

String morseCode[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....",
  "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.",
  "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-",
  "-.--", "--.."
};
char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
  if (digitalRead(dotBtn) == HIGH) {
    morseInput += ".";
    blinkDot();
    lastPressTime = millis();
    while (digitalRead(dotBtn) == HIGH);
  }

  if (digitalRead(dashBtn) == HIGH) {
    morseInput += "-";
    blinkDash();
    lastPressTime = millis();
    while (digitalRead(dashBtn) == HIGH);
  }

  if (morseInput.length() > 0 && millis() - lastPressTime > 1000 && millis() - startTime > 3000) {
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

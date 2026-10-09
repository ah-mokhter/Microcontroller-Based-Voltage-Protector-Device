#include <LiquidCrystal.h>
#include <EEPROM.h>

// Initialize LCD pins (RS=12, EN=11, D4=5, D5=4, D6=3, D7=2)
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Hardware Pin Assignments from Schematic
const int AC_SENSE_PIN = A0;
const int INC_BTN_PIN  = 9;  // Increment (+5V) Button
const int DEC_BTN_PIN  = 8;  // Decrement (-5V) Button
const int RELAY_PIN    = 10; // Relay Output

// Dynamic Protection Threshold Variables
float overvoltageLimit = 230.0; // Default limit
const float CALIBRATION_FACTOR = 73.35; // 217V AC / 2.30V DC at Pin A0

// Button state tracking for edge detection
bool lastIncState = LOW;
bool lastDecState = LOW;

void setup() {
  lcd.begin(16, 2);
  
  pinMode(AC_SENSE_PIN, INPUT);
  pinMode(INC_BTN_PIN, INPUT);
  pinMode(DEC_BTN_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Read saved threshold from EEPROM if valid
  float savedLimit;
  EEPROM.get(0, savedLimit);
  if (savedLimit >= 150.0 && savedLimit <= 300.0) {
    overvoltageLimit = savedLimit;
  }

  // Initial Startup Screen
  lcd.setCursor(0, 0);
  lcd.print("AC Monitor Init ");
  delay(1000);
  lcd.clear();
}

void loop() {
  // 1. Read Increment / Decrement Buttons with Edge Detection (+5V / -5V)
  bool currentIncState = digitalRead(INC_BTN_PIN);
  bool currentDecState = digitalRead(DEC_BTN_PIN);

  // Increment button pressed
  if (currentIncState == HIGH && lastIncState == LOW) {
    overvoltageLimit += 5.0;
    if (overvoltageLimit > 300.0) overvoltageLimit = 300.0; // Max safety cap
    EEPROM.put(0, overvoltageLimit); // Save setting
    delay(50); // Debounce
  }

  // Decrement button pressed
  if (currentDecState == HIGH && lastDecState == LOW) {
    overvoltageLimit -= 5.0;
    if (overvoltageLimit < 150.0) overvoltageLimit = 150.0; // Min safety cap
    EEPROM.put(0, overvoltageLimit); // Save setting
    delay(50); // Debounce
  }

  lastIncState = currentIncState;
  lastDecState = currentDecState;

  // 2. Read 30 ADC samples to smooth DC ripple from C1/C2
  long adcSum = 0;
  for (int i = 0; i < 30; i++) {
    adcSum += analogRead(AC_SENSE_PIN);
    delay(1);
  }
  float avgAdc = adcSum / 30.0;

  // 3. Convert ADC to DC Pin Voltage & Calculate AC Voltage
  float pinVoltage = (avgAdc * 5.0) / 1023.0;
  float acVoltage = pinVoltage * CALIBRATION_FACTOR;

  // 4. Update Row 0: Real-time AC Voltage & Set Limit Threshold
  lcd.setCursor(0, 0);
  lcd.print("AC:");
  lcd.print((int)acVoltage);
  lcd.print("V Set:");
  lcd.print((int)overvoltageLimit);
  lcd.print("V ");

  // 5. Automatic Relay Protection Logic
  lcd.setCursor(0, 1);
  if (acVoltage >= overvoltageLimit) {
    digitalWrite(RELAY_PIN, LOW); // Relay OFF on overvoltage
    lcd.print("Relay:OFF (TRIP)");
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Relay ON during normal voltage
    lcd.print("Relay:ON (NORM) ");
  }

  delay(50); // Loop stability delay
}

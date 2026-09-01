// Industrial Multi-Gas Monitor (pins as provided)
// Sensors: MQ2, MQ3, MQ4, MQ5, MQ6, MQ8
// AO pins: A0..A5
// Two LEDs per sensor: Green = safe, Red = alarm
// Active buzzer on D7

// ----- Sensor analog pins (MQ2, MQ3, MQ4, MQ5, MQ6, MQ8) -----
const int mqPins[6] = { A0, A1, A2, A3, A4, A5 };

// ----- Green LEDs (safe) corresponding to sensors above -----
const int greenLEDs[6] = { 2, 4, 6, 9, 11, 13 };

// ----- Red LEDs (alarm) corresponding to sensors above -----
// Note: pin 0 is Serial RX. If you use Serial Monitor, change this pin to a free digital pin.
const int redLEDs[6]   = { 3, 5, 8, 10, 12, 0 };

// ----- Buzzer (active) -----
const int buzzerPin = 7;

// ----- Human friendly names for serial output -----
const char* gasNames[6] = {
  "MQ2 (LPG/Smoke)",
  "MQ3 (Alcohol)",
  "MQ4 (Methane/LPG)",
  "MQ5 (Natural gas)",
  "MQ6 (LPG/Butane)",
  "MQ8 (Hydrogen)"
};

// ----- Thresholds (raw ADC 0..1023) - tune after testing -----
// Example starting values (you must calibrate these)
int threshold[6] = { 1000,900,800,700,750,1000 };

void setup() {
  Serial.begin(9600);
  // configure sensor pins (analog inputs don't need pinMode)
  for (int i = 0; i < 6; i++) {
    pinMode(greenLEDs[i], OUTPUT);
    pinMode(redLEDs[i], OUTPUT);
    // Start with safe (green ON, red OFF)
    digitalWrite(greenLEDs[i], HIGH);
    digitalWrite(redLEDs[i], LOW);
  }
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Serial.println();
  Serial.println("Multi-gas Monitor (MQ2, MQ3, MQ4, MQ5, MQ6, MQ8)");
  Serial.println("Note: If you use Serial Monitor, avoid using D0 for LED (it's RX).");
  Serial.println("Warming up sensors... wait ~20-30s for stable readings.");
  Serial.println();
}

void loop() {
  bool anyAlarm = false;

  for (int i = 0; i < 6; i++) {
    int raw = analogRead(mqPins[i]);          // 0..1023
    Serial.print(gasNames[i]);
    Serial.print(" : ");
    Serial.println(raw);

    if (raw > threshold[i]) {
      // alarm for this sensor
      digitalWrite(redLEDs[i], HIGH);
      digitalWrite(greenLEDs[i], LOW);
      anyAlarm = true;
    } else {
      // safe
      digitalWrite(redLEDs[i], LOW);
      digitalWrite(greenLEDs[i], HIGH);
    }
  }

  // common buzzer logic
  if (anyAlarm) {
    digitalWrite(buzzerPin, HIGH);  // active buzzer ON
  } else {
    digitalWrite(buzzerPin, LOW);   // OFF
  }

  Serial.println("------------------------------");
  delay(1000); // sample every 1s
}

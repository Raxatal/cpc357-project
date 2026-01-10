#include <Arduino.h>

// ---------------- Pins ----------------
#define FLOW_PIN 6 // your YF-S401 sensor pin
volatile int pulseCount = 0;

// ---------------- Interrupt ----------------
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

// ---------------- Variables ----------------
float volumeLiters = 0.5; // 500 mL bottle = 0.5 L
unsigned long startTime, endTime;

void setup() {
  Serial.begin(115200);
  pinMode(FLOW_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), pulseCounter, RISING);

  Serial.println("=== YF-S401 Calibration ===");
  Serial.println("Step 1: Prepare 500 mL container.");
  Serial.println("Step 2: Place tube in container.");
  Serial.println("Step 3: Press RESET to start measurement.");
  delay(5000);
}

void loop() {
  pulseCount = 0;
  Serial.println("\nStarting pulse measurement...");
  Serial.println("Start water flow NOW!");
  
  startTime = millis();

  // Wait until container is full
  // Manually stop the ESP32 when 500 mL is collected
  // Press the Serial Monitor RESET button (or any method)
  Serial.println("Measure until 500 mL is filled, then press 's' in Serial Monitor...");
  
  // Wait for manual stop
  while (Serial.available() == 0) {
    // just count pulses
  }

  endTime = millis();
  float timeSeconds = (endTime - startTime) / 1000.0;

  Serial.println("\nMeasurement stopped!");
  Serial.print("Total pulses counted: ");
  Serial.println(pulseCount);
  Serial.print("Time taken (s): ");
  Serial.println(timeSeconds);

  // Calculate K-factor: pulses per L/min
  // Flow rate (L/min) = volumeLiters * 60 / timeSeconds
  // K = pulses counted / flow rate
  float flowRate = (volumeLiters * 60.0) / timeSeconds; // L/min
  float Kfactor = pulseCount / flowRate;

  Serial.print("Your calibrated K-factor (pulses per L/min) = ");
  Serial.println(Kfactor, 2);

  // End of calibration
  while (true) {
    // stop here
  }
}

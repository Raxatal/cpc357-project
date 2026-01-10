#include <WiFi.h>
#include <HTTPClient.h>

// ---------------- Wi-Fi ----------------
const char* ssid = "";          // Name of Wi-Fi, connect devices to the same Wi-Fi / hotspot
const char* password = "";      // Replace with actual Wi-Fi name and password

// ---------------- Firebase ----------------
const char* firebaseBaseURL = "";   // Replace with Firebase RTDB URL

// ---------------- Pins ----------------
#define TRIG_PIN 41
#define ECHO_PIN 42
#define FLOW_PIN 6

#define RED_LED A5
#define YELLOW_LED A4
#define BUZZER 47

volatile int pulseCount = 0;

// ---------------- Calibration ----------------
const float KFACTOR = 4440; // calibrated factor based on our setup

// ---------------- Distance smoothing ----------------
const int DIST_SAMPLES = 5;         // moving average window
float distanceBuffer[DIST_SAMPLES];
int distanceIndex = 0;

// ---------------- Thresholds ----------------
const float DIST_NORMAL = 6.0;      // cm
const float DIST_CRITICAL = 4.0;    // cm

// ---------------- Flow timing ----------------
unsigned long lastFlowMillis = 0;
float flowRate = 0.0;

// ---------------- Interrupt for flow sensor ----------------
void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);

  // Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi Connected");

  // Ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Flow sensor
  pinMode(FLOW_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), pulseCounter, RISING);

  // Alerts
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH); // buzzer off

  // Initialise distance buffer
  for (int i = 0; i < DIST_SAMPLES; i++) distanceBuffer[i] = 0.0;
}

// ---------------- Helper function to read distance ----------------
float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
  float distance = duration * 0.034 / 2;          // cm
  return distance;
}

// ---------------- Helper function for smooth distance ----------------
float smoothDistance(float newDistance) {
  distanceBuffer[distanceIndex] = newDistance;
  distanceIndex = (distanceIndex + 1) % DIST_SAMPLES;

  float sum = 0;
  for (int i = 0; i < DIST_SAMPLES; i++) sum += distanceBuffer[i];
  return sum / DIST_SAMPLES;
}

// ---------------- Helper function to update flow rate ----------------
void updateFlowRate() {
  unsigned long now = millis();
  if (now - lastFlowMillis >= 1000) { // 1-second window
    detachInterrupt(digitalPinToInterrupt(FLOW_PIN));

    // Correct calibrated flow
    flowRate = pulseCount / KFACTOR;

    pulseCount = 0;
    lastFlowMillis = now;

    attachInterrupt(digitalPinToInterrupt(FLOW_PIN), pulseCounter, RISING);
  }
}

// ---------------- Helper function to send data to Firebase ----------------
void sendToFirebase(String path, String payload) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(firebaseBaseURL + path);
    http.addHeader("Content-Type", "application/json");
    http.PUT(payload);
    http.end();
  }
}

// ---------------- Main loop ----------------
void loop() {
  // Read sensors
  float rawDistance = readDistance();
  float distance = smoothDistance(rawDistance);
  updateFlowRate();

  // Determine status (distance only)
  String status;
  if (distance <= DIST_CRITICAL) {
    status = "Critical Blockage";
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BUZZER, LOW); // continuous alarm
  }
  else if (distance <= DIST_NORMAL) {
    status = "Partial Blockage";
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);

    // Single short beep
    digitalWrite(BUZZER, LOW);
    delay(200);
    digitalWrite(BUZZER, HIGH);
  }
  else {
    status = "Normal";
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BUZZER, HIGH); // silent
  }

  // Print to Serial Monitor
  Serial.print("Distance: "); Serial.print(distance,2);
  Serial.print(" cm | Flow: "); Serial.print(flowRate,3);
  Serial.print(" L/min | Status: "); Serial.println(status);

  // Send to Firebase
  sendToFirebase("/ultrasonic/distance.json", String(distance,2));
  sendToFirebase("/flow/rate.json", String(flowRate,3));

  String historyPayload = "{"
    "\"distance\":" + String(distance,2) + ","
    "\"flow\":" + String(flowRate,3) + ","
    "\"status\":\"" + status + "\""
    "}";
  sendToFirebase("/history/" + String(millis()) + ".json", historyPayload);

  delay(500); // loop
}

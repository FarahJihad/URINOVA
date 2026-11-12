/* URINOVA hackathon prototype — NOT for clinical use.
   Assumed hardware: classic ESP32 DevKit + HC-SR04.
   Verify your board and sensor before wiring. Mount the sensor at the bag's
   lowest point facing the floor. Otherwise this measures sensor-to-floor,
   not necessarily bag-to-floor. Set SENSOR_OFFSET_CM after calibration.

   HC-SR04 VCC -> regulated 5V; GND -> common GND; TRIG -> GPIO 5.
   ECHO -> 10k resistor -> GPIO 18; GPIO 18 -> 10k resistor -> GND.
   This divider lowers the 5V echo to ~2.5V. Never connect 5V echo directly
   to an ESP32 GPIO. Pins apply to classic DevKit, not every ESP32 variant.
   Serial JSON at 115200 baud. No Wi-Fi or cloud connection in this sketch.
*/
#include <Arduino.h>

constexpr uint8_t TRIG_PIN = 5;
constexpr uint8_t ECHO_PIN = 18;
constexpr float SENSOR_OFFSET_CM = 0.0f;
constexpr float DEMO_THRESHOLD_CM = 20.0f;
uint32_t sequenceNumber = 0;

float sampleDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return -1; // No echo is a fault, never a zero reading.
  float cm = duration * 0.0343f / 2.0f;
  if (cm < 2 || cm > 400) return -1;
  return cm;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  float samples[5];
  int count = 0;
  for (int i = 0; i < 5; i++) {
    float cm = sampleDistance();
    if (cm >= 0) samples[count++] = cm;
    delay(65);
  }
  sequenceNumber++;
  if (count < 3) {
    Serial.printf("{\"deviceId\":\"SIM-01\",\"status\":\"sensor_error\",\"sequence\":%lu}\n", (unsigned long)sequenceNumber);
  } else {
    for (int i=0; i<count-1; i++) for (int j=i+1; j<count; j++) {
      if (samples[j]<samples[i]) {float tmp=samples[i];samples[i]=samples[j];samples[j]=tmp;}
    }
    float height = samples[count/2] - SENSOR_OFFSET_CM;
    if (height < 0) {
      Serial.printf("{\"deviceId\":\"SIM-01\",\"status\":\"calibration_error\",\"sequence\":%lu}\n", (unsigned long)sequenceNumber);
    } else {
      Serial.printf("{\"deviceId\":\"SIM-01\",\"status\":\"ok\",\"heightCm\":%.1f,\"belowThreshold\":%s,\"sequence\":%lu}\n",
        height, height<=DEMO_THRESHOLD_CM?"true":"false", (unsigned long)sequenceNumber);
    }
  }
  delay(650);
}

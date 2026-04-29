// ============================================
// Water Level Monitor — MQTT (ThingSpeak)
// Board  : ESP32
// Sensor : HC-SR04 Ultrasonic
// ============================================

#include <WiFi.h>
#include <PubSubClient.h>

// ── WiFi ────────────────────────────────────
const char* ssid     = "He’s sad";
const char* password = "123456789";

// ── MQTT (ThingSpeak) ───────────────────────
const char* mqttServer = "mqtt3.thingspeak.com";
const int   mqttPort   = 1883;

const char* mqttUser = "FRk1MQY0JRwGNQU5OjsbORc";
const char* mqttPass = "MhOWTrn9CTr7PtoqcN0JlUDU";
const char* clientID = "FRk1MQY0JRwGNQU5OjsbORc";

unsigned long channelID = 3359615;

// ── Pins ────────────────────────────────────
#define TRIG_PIN  12
#define ECHO_PIN  13
#define LED_PIN   2
#define BUZZER    14

// ── Tank config ─────────────────────────────
const float TANK_HEIGHT_CM = 18.0;

// 🔥 CALIBRATION (VERY IMPORTANT)
const float EMPTY_DISTANCE = 18;  // measure when empty
const float FULL_DISTANCE  = 2.5;   // measure when full

// Alerts (better realistic values)
const float ALERT_LOW_PCT  = 20.0;
const float ALERT_HIGH_PCT = 90.0;

// ── Objects ─────────────────────────────────
WiFiClient espClient;
PubSubClient client(espClient);

// ── Variables ───────────────────────────────
float distanceCm    = 0;
float waterLevelCm  = 0;
float waterLevelPct = 0;

// ── Measure distance ────────────────────────
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long dur = pulseIn(ECHO_PIN, HIGH, 30000);
  if (dur == 0) return -1.0;

  return (dur * 0.0343f) / 2.0f;
}

// ── WiFi connect (with timeout) ─────────────
void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW);
    Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nWiFi FAILED!");
  }
}

// ── MQTT connect ────────────────────────────
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    if (client.connect(clientID, mqttUser, mqttPass)) {
      Serial.println("Connected!");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying...");
      delay(2000);
    }
  }
}

// ── Send data via MQTT ──────────────────────
void sendMQTT() {

  float raw = measureDistance();

  if (raw < 0) {
    Serial.println("[ERR] Sensor error!");
    return;
  }

  distanceCm = raw;

  // 🔥 CALIBRATED FORMULA
  waterLevelPct =
    (EMPTY_DISTANCE - distanceCm) /
    (EMPTY_DISTANCE - FULL_DISTANCE) * 100.0;

  waterLevelPct = constrain(waterLevelPct, 0, 100);
  waterLevelCm  = (waterLevelPct / 100.0) * TANK_HEIGHT_CM;

  // ── Status ───────────────────────────────
  String status;

  if (waterLevelPct <= ALERT_LOW_PCT) {
    status = "LOW";
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER, 1000, 400);
  }
  else if (waterLevelPct >= ALERT_HIGH_PCT) {
    status = "HIGH";
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER, 2000, 400);
  }
  else {
    status = "NORMAL";
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER);
  }

  // ── MQTT send ────────────────────────────
  if (!client.connected()) connectMQTT();
  client.loop();

  String topic = "channels/" + String(channelID) + "/publish";

  String payload =
    "field1=" + String(waterLevelPct) +
    "&field2=" + String(waterLevelCm) +
    "&field3=" + String(distanceCm) +
    "&field4=" + status;

  if (client.publish(topic.c_str(), payload.c_str())) {
    Serial.println("MQTT sent successfully!");
  } else {
    Serial.println("MQTT send FAILED!");
  }

  // ── Debug print ──────────────────────────
  Serial.printf(
    "Dist: %.1f cm | Level: %.1f cm | %.1f%% | %s\n",
    distanceCm, waterLevelCm, waterLevelPct, status.c_str()
  );
}

// ── Setup ───────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_PIN, LOW);

  Serial.println("\n=== Water Monitor — MQTT ===");

  connectWiFi();
  client.setServer(mqttServer, mqttPort);
}

// ── Loop ────────────────────────────────────
void loop() {
  sendMQTT();
  delay(15000); // ThingSpeak limit
}

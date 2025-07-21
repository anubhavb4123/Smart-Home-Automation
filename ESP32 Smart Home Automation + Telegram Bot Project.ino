// Required Libraries
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Servo.h>

// WiFi and Telegram Bot
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
unsigned long lastTimeBotRan;

// Sensor Pins
#define DHTPIN 15
#define DHTTYPE DHT11
#define MQ135_PIN 34
#define RAIN_SENSOR_PIN 35
#define TRIG_PIN 5
#define ECHO_PIN 18
#define DOOR_SENSOR_PIN 32
#define PIR_PIN 33

// Relay Control
#define LIGHT_RELAY_PIN 25
#define FAN_RELAY_PIN 26

// Servo Lock
Servo lockServo;
#define SERVO_PIN 27

// DHT and BMP Setup
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

// Motion Detection
bool motionActive = false;
bool wasRaining = false; // Rain state tracker
bool gasAlertSent = false; // Gas alert flag

void setup() {
  Serial.begin(115200);
  dht.begin();
  bmp.begin();
  lockServo.attach(SERVO_PIN);

  pinMode(MQ135_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT);

  pinMode(LIGHT_RELAY_PIN, OUTPUT);
  pinMode(FAN_RELAY_PIN, OUTPUT);

  digitalWrite(LIGHT_RELAY_PIN, HIGH);
  digitalWrite(FAN_RELAY_PIN, HIGH);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  client.setInsecure();
  bot.sendMessage(CHAT_ID, "🤖 Smart Home Bot is Online!", "");
}

long readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH);
}

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String text = bot.messages[i].text;
    String chat_id = bot.messages[i].chat_id;

    if (text == "/status") {
      float temp = dht.readTemperature();
      float hum = dht.readHumidity();
      int air = analogRead(MQ135_PIN);
      int rain = digitalRead(RAIN_SENSOR_PIN);
      float pressure = bmp.readPressure() / 100.0F;
      bool door = digitalRead(DOOR_SENSOR_PIN);

      String message = "🌡 Temp: " + String(temp) + " °C\n";
      message += "💧 Humidity: " + String(hum) + " %\n";
      message += "📈 Pressure: " + String(pressure) + " hPa\n";
      message += "🌫 Air Quality: " + String(air) + "\n";
      message += "🌧 Rain: " + String(rain == LOW ? "Yes" : "No") + "\n";
      message += "🚪 Door: " + String(door == LOW ? "Closed" : "Open") + "\n";
      message += "🕵️ PIR: " + String(motionActive ? "ACTIVE" : "OFF") + "";
      bot.sendMessage(chat_id, message, "");
    }
    else if (text == "/on light") {
      digitalWrite(LIGHT_RELAY_PIN, LOW);
      bot.sendMessage(chat_id, "💡 Light turned ON", "");
    }
    else if (text == "/off light") {
      digitalWrite(LIGHT_RELAY_PIN, HIGH);
      bot.sendMessage(chat_id, "💡 Light turned OFF", "");
    }
    else if (text == "/on fan") {
      digitalWrite(FAN_RELAY_PIN, LOW);
      bot.sendMessage(chat_id, "🌬 Fan turned ON", "");
    }
    else if (text == "/off fan") {
      digitalWrite(FAN_RELAY_PIN, HIGH);
      bot.sendMessage(chat_id, "🌬 Fan turned OFF", "");
    }
    else if (text == "/lock") {
      lockServo.write(0);
      bot.sendMessage(chat_id, "🔐 Gate Locked", "");
    }
    else if (text == "/unlock") {
      lockServo.write(90);
      bot.sendMessage(chat_id, "🔓 Gate Unlocked", "");
    }
    else if (text == "/waterlevel") {
      long duration = readUltrasonicDistance();
      float distance = duration * 0.034 / 2;
      float level = map(distance, 0, 30, 100, 0); // 30cm = max depth
      bot.sendMessage(chat_id, "🚰 Water Tank Level: " + String(level) + "%", "");
    }
    else if (text == "/motion on") {
      motionActive = true;
      bot.sendMessage(chat_id, "🟢 Motion detection ENABLED", "");
    }
    else if (text == "/motion off") {
      motionActive = false;
      bot.sendMessage(chat_id, "🔴 Motion detection DISABLED", "");
    }
  }
}

void loop() {
  if (millis() - lastTimeBotRan > 1000) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  // 🌧 Rain Detection
  bool isRaining = digitalRead(RAIN_SENSOR_PIN) == LOW;
  if (isRaining && !wasRaining) {
    bot.sendMessage(CHAT_ID, "🌧️ Rain Detected! Please take precautions.", "");
    wasRaining = true;
  } else if (!isRaining && wasRaining) {
    bot.sendMessage(CHAT_ID, "☀️ Rain Stopped.", "");
    wasRaining = false;
  }

  // 🌫 Air Quality Check
  int airQuality = analogRead(MQ135_PIN);
  if (airQuality > 1000 && !gasAlertSent) {
    bot.sendMessage(CHAT_ID, "⚠️ ALERT: Poor Air Quality or Possible Gas Leak Detected!", "");
    gasAlertSent = true;
  } else if (airQuality <= 1000 && gasAlertSent) {
    gasAlertSent = false;
  }

  // 🕵️ PIR Motion Alert
  if (motionActive && digitalRead(PIR_PIN) == HIGH) {
    bot.sendMessage(CHAT_ID, "🚨 Motion Detected at Door!", "");
    delay(5000); // Cooldown to prevent spam
  }
} // END

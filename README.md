# 🤖 ESP32 Smart Home Automation with Telegram Bot

This project is a **DIY smart home automation system** built using an **ESP32** microcontroller. It integrates multiple sensors and actuators, with control and alerts handled via a **Telegram bot**.

---

## 🌐 Features

| Feature                      | Description |
|-----------------------------|-------------|
| 📱 Telegram Bot Control      | Turn ON/OFF light and fan, lock/unlock door, get sensor data, and more. |
| 🌡 DHT11 Sensor              | Monitor room temperature and humidity. |
| 🌫 MQ135 Sensor              | Detect poor air quality or gas leakage. Sends alert only when the air quality is bad. |
| 🌧 Rain Sensor               | Sends alert when rain starts and when it stops. |
| 🏞 BMP180 Sensor             | Measures barometric pressure. |
| 🚪 Magnetic Reed Switch      | Detects if the door is open or closed. |
| 💧 Ultrasonic Water Level    | Monitors water tank level. |
| 🔐 Servo-based Lock          | Remote lock/unlock door control. |
| 🕵️ PIR Motion Sensor         | Motion detection at door (manually activatable). |
| 🧠 Command-based Activation  | Control sensors with `/motion on`, `/status`, `/waterlevel`, etc. |
| 📡 Expandable via MQTT (optional) | Integrate more sensors wirelessly across your home. |

---

## 🛠 Hardware Used

- ESP32 Dev Board
- DHT11 Temperature and Humidity Sensor
- BMP180 Pressure Sensor
- MQ135 Gas Sensor
- Rain Sensor Module
- HC-SR04 Ultrasonic Sensor
- Magnetic Door Sensor (Reed Switch)
- PIR Motion Sensor
- 2-Channel Relay Module
- Servo Motor (for lock)
- Jumper wires, Resistors
- Breadboard / PCB

---

## 📦 Libraries Required

Make sure to install these libraries via Arduino IDE Library Manager:

- `WiFi.h`
- `WiFiClientSecure.h`
- `UniversalTelegramBot`
- `DHT.h`
- `Adafruit_BMP085.h`
- `Servo.h`

---

## 📲 Telegram Bot Setup

1. Open Telegram and search for [BotFather](https://t.me/BotFather).
2. Create a new bot using `/newbot`.
3. Copy the **Bot Token**.
4. Get your **Chat ID** using this bot: [IDBot](https://t.me/myidbot)
5. Replace `BOT_TOKEN` and `CHAT_ID` in the code.

---

## ⚙️ How to Use

| Command           | Action |
|-------------------|--------|
| `/status`         | Get full sensor data overview |
| `/on light`       | Turn on light |
| `/off light`      | Turn off light |
| `/on fan`         | Turn on fan |
| `/off fan`        | Turn off fan |
| `/lock`           | Lock the gate (servo) |
| `/unlock`         | Unlock the gate |
| `/motion on`      | Enable motion sensor |
| `/motion off`     | Disable motion sensor |
| `/waterlevel`     | Get current water tank level |

Alerts are sent automatically for:
- Rain start/stop
- Motion detection (if active)
- Poor air quality or gas leak

---

## 🔐 Security

This system uses Telegram Bot API for secure communication. Ensure your bot token and chat ID are **not public** in any published repo.

---

## 🔌 Circuit Diagram

Coming Soon...

---

## 📸 Screenshots

Coming Soon...

---

## 📡 Future Improvements

- MQTT-based multi-node system for larger homes
- Blynk/ESP RainMaker app support
- Real-time web dashboard
- Offline control via local server

---

## 🙌 Author

**Anubhav Bajpai**  
📧 anubhavb4123@gmail.com  
🔗 [GitHub](https://github.com/anubhavb4123) | [LinkedIn](https://www.linkedin.com/in/anubhav-bajpai-12318032b)


> Smart homes aren't about luxury — they're about **control, awareness**, and **peace of mind**. 🧠🏡

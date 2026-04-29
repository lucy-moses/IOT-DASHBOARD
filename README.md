# 💧 IoT Water Level Monitoring System

An IoT-based system that monitors water levels in real-time using an ESP32 and ultrasonic sensor, with cloud integration and a lightweight web dashboard.

---

## 🚀 Live Demo

🌐 https://lucy-moses.github.io/IOT-DASHBOARD/

---

## 📌 Overview

This project measures the water level in a tank using an ultrasonic sensor and sends the data to the cloud using MQTT. The data is then visualized on a simple web dashboard built using HTML.

---

## ⚙️ Features

* 📡 Real-time water level monitoring
* ☁️ Cloud integration using ThingSpeak
* 🌐 Lightweight web dashboard (HTML-based)
* 📱 Accessible on mobile and desktop
* 📏 Calibration-based measurement

---

## 🛠️ Tech Stack

* **Hardware:** ESP32, HC-SR04 Ultrasonic Sensor
* **Communication:** MQTT Protocol
* **Cloud:** ThingSpeak
* **Frontend:** HTML
* **Hosting:** GitHub Pages

---

## 🔄 System Flow

Sensor → ESP32 → WiFi → MQTT → ThingSpeak → Web Dashboard

---

## 📐 Working Principle

* The ultrasonic sensor measures the distance to the water surface

* Based on calibration values:

  * Empty Distance = 18.5 cm
  * Full Distance = 2.5 cm

* Water level is calculated as:

```
Level (%) = (Empty - Distance) / (Empty - Full) × 100
```

---

## 📂 Project Structure

```
IOT-DASHBOARD/
 ├── index.html
 ├── water_monitor.ino
 └── README.md
```

---

## 🚧 Challenges

* Sensor noise and fluctuating readings
* Calibration tuning
* MQTT setup and connectivity

---

## 💡 Future Improvements

* Add CSS and JavaScript for enhanced UI
* Pump automation using relay
* Mobile notifications (Telegram)
* Data analytics and prediction

---

## 🎓 Key Learnings

* IoT system integration
* MQTT communication
* Cloud-based monitoring
* Basic web dashboard development

---

## 👩‍💻 Author

Lucy Moses

---

## ⭐ If you like this project

Give it a star ⭐

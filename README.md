
# 🌱 HydroGrow: Autonomous IoT-Based Hydroponic Control System

Welcome to **HydroGrow**, a smart hydroponic automation system powered by the ESP32 microcontroller. Designed for indoor growers, hobbyists, and automation enthusiasts, HydroGrow delivers real-time monitoring and timed control of your grow lights and water pump—all through a clean local web interface.

---

## 🔧 Features

- 🌐 **Wi-Fi Enabled**: Secure local intranet access with NTP-based time sync.
- 📟 **Live Dashboard**: View current time, temperature (°F), humidity (%), and ambient light level.
- 💡 **Light Automation**: Schedule ON/OFF hours with persistent storage.
- 💧 **Water Pump Automation**: Set pump intervals and override manually if needed.
- 📦 **EEPROM Persistence**: Automation settings survive reboots and outages.
- 🌍 **Local Web Control**: GUI served via ESP32 at `http://HydroGrow.local`.
- ⚡ **Relay Switching via Transistors**: Efficient and stable hardware design.

---

## 🛠️ Hardware Overview

- **ESP32 Dev Board**
- **DHT11** - Temperature & Humidity Sensor
- **Photoresistor + Resistor Divider** - Light Level Detection
- **NPN Transistors (S8050)** - Drives high-power relay coils
- **Relays (with internal flyback diodes)** - Controls 110V/12V loads
- **1kΩ Base Resistors** - Protect GPIOs from overcurrent
- **Power Supply** - 5V/12V regulated

---

## 🧠 Why Transistors, Not L293D?

Originally, an L293D motor driver was envisioned to control the relays, but it couldn’t identify the current, impedence, or demand\load. Swapping to **NPN transistors** provided a seamless, stable solution—ensuring proper switching and full compatibility with the ESP32’s low-power output.

---

## 💻 Web Dashboard Preview

- Realtime sensor values
- Automation schedule overview
- Manual toggle for lights and pump
- Configuration page for updating on/off times

> Access: `http://HydroGrow.local` (intranet only)

---

## 🚀 Future Upgrades

- 🌬️ Add fan control & air circulation logic
- 🌊 Add water level, pH, and TDS monitoring
- 📲 Mobile dashboard (ESPAsync + Bootstrap)
- ☁️ Optional cloud sync & historical data logging
- 🔒 Secure login & HTTPS encryption

---

## 📸 Demo & Images

📷 <img width="692" alt="image" src="https://github.com/user-attachments/assets/c420e211-8efb-4c61-8ce9-c410cb35e858" />
<img width="379" alt="image" src="https://github.com/user-attachments/assets/d1e1354d-96f9-4c35-a1c8-74a2759bf347" />
<img width="1323" alt="CodeSchematic" src="https://github.com/user-attachments/assets/48583b7c-d79b-46ef-9233-c33c7e619caf" />


---

## 📁 File Structure

```
/HydroGrow
├── src/
│   └── main.ino               # Final ESP32 Arduino code
├── assets/                    # Images, diagrams, screenshots
└── README.md                  # You're here
```

---

## ✨ Authors

- Melissa Gandy @melgandy  
- CSE-590 | IoT Spring 2025 | University of Louisville

---

## 📝 License

MIT License — use, modify, and grow responsibly 🌿

---

> “Automation doesn’t replace care, it delivers it—precisely, reliably, and on time.”

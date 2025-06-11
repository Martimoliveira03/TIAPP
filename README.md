# TIAPP – Sustainable Water Monitoring System

This repository contains the source files, documentation, and diagrams related to the **TIAPP project**, developed during the Interface Technologies course. The project aims to encourage more conscious water use habits by providing real-time feedback and long-term consumption insights through a low-cost smart shower monitoring system.

---

## 📌 Project Description

TIAPP combines hardware and software components to track water consumption during showers and provide immediate and reflective feedback to users. The system uses an **ESP32 microcontroller** to monitor flow data in real time and sends it to a **Node.js server**, where data is stored and later displayed in a simple application interface.  

The system supports:
- Real-time feedback (LCD, LED, buzzer)
- Dual interaction mode (progressive/regressive counter)
- Data transmission via Wi-Fi
- Server-side data storage in JSON
- Visualization of water usage history via line graphs

The goal is to **raise awareness** and support **behavioral change** through interaction design, embedded feedback, and data visualization.

---

## 📁 Repository Structure

```bash
TIAPP/
├── docs/                 # Final report and intermediate versions   
│
├── res/                  # Visual resources used in the report
│
├── src/                  # Source code (ESP32 frontend)
│   ├── esp32/            # Arduino code for ESP32
│   └── assets/           # icons and images for frontend
|
├── watr-server/                 # JSON data examples and node modules
├── .gitignore
├── LICENSE
├── README.md
└── CODE_OF_CONDUCT.md

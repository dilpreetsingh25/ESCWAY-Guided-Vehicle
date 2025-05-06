# ESP32 RC Car over Wi-Fi
This project is a simple yet effective implementation of a Wi-Fi-controlled RC car built using an ESP32 microcontroller. The car can be controlled in real time using a smartphone over a local Wi-Fi network, without any external router.
## Features:
ESP32 as the main controller
Wi-Fi Access Point (AP) mode – no external router needed

Control the car through a web-based interface hosted by ESP32

Responsive controls: forward, reverse, left, right, and stop
## Technologies Used:
Arduino IDE for ESP32 programming


ESP32 Wi-Fi libraries (WiFi.h, WebServer.h)

Optional: OTA updates, battery monitoring, speed control

## How it Works:
ESP32 hosts a Wi-Fi network (e.g., ESP32-CAR)

Phone connects to the ESP32 network

Web page served from ESP32 gives on-screen controls

Commands are sent via HTTP and interpreted by the ESP32 to control motors

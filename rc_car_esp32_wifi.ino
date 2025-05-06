#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials (STATION)
const char* ssid = "*******";
const char* password = "********";

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Web server on port 80
WebServer server(80);


// MOTOR 1 (Left Motor) PINS
#define L_PWM1 37
#define R_PWM1 38
#define R_EN1 39
#define L_EN1 40
#define ENCODER_A1 35
#define ENCODER_B1 36

// MOTOR 2 (Right Motor) PINS
#define L_PWM2 7
#define R_PWM2 4
#define R_EN2 5
#define L_EN2 6
#define ENCODER_A2 10
#define ENCODER_B2 9

volatile long encoderCount1 = 0; // Motor 1 encoder counter
volatile long encoderCount2 = 0; // Motor 2 encoder counter

void setup() {
  Serial.begin(115200);
    initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  // Setup Motor pins
  pinMode(L_PWM1, OUTPUT);
  pinMode(R_PWM1, OUTPUT);
  pinMode(R_EN1, OUTPUT);
  pinMode(L_EN1, OUTPUT);

  pinMode(L_PWM2, OUTPUT);
  pinMode(R_PWM2, OUTPUT);
  pinMode(R_EN2, OUTPUT);
  pinMode(L_EN2, OUTPUT);

  // Enable motors initially
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);

  // Setup Encoder pins
  pinMode(ENCODER_A1, INPUT_PULLUP);
  pinMode(ENCODER_B1, INPUT_PULLUP);
  pinMode(ENCODER_A2, INPUT_PULLUP);
  pinMode(ENCODER_B2, INPUT_PULLUP);

  // Attach interrupts for encoders
  attachInterrupt(digitalPinToInterrupt(ENCODER_A1), readEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A2), readEncoder2, CHANGE);

  stopMotors();

    // Start WiFi
  WiFi.softAP(ssid, password);
  Serial.println("WiFi Started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {

  server.handleClient();
  
//  if (Serial.available()) {
//    char command = Serial.read();
//
//    switch (command) {
//      case 'w':
//        moveForward();
//        break;
//      case 'x':
//        moveBackward();
//        break;
//      case 'a':
//        turnLeft();
//        break;
//      case 'd':
//        turnRight();
//        break;
//      case 's':
//        stopMotors();
//        break;
//    }
//  }
//
//  // Print encoder counts for monitoring
//  Serial.print("Motor1 Encoder: ");
//  Serial.print(encoderCount1);
//  Serial.print(" | Motor2 Encoder: ");
//  Serial.println(encoderCount2);
//  delay(100);
}

// ================= Motor Control =================

void moveForward() {
  analogWrite(R_PWM1, 0);
  analogWrite(L_PWM1, 100);

  analogWrite(R_PWM2, 100);
  analogWrite(L_PWM2, 0);
}

void moveBackward() {
  analogWrite(R_PWM1, 100);
  analogWrite(L_PWM1, 0);

  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, 100);
}

void turnLeft() {
  analogWrite(R_PWM1, 100);
  analogWrite(L_PWM1, 0);

  analogWrite(R_PWM2, 100);
  analogWrite(L_PWM2, 0);
}

void turnRight() {
  analogWrite(R_PWM1, 0);
  analogWrite(L_PWM1, 100);

  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, 100);
}

void stopMotors() {
  analogWrite(R_PWM1, 0);
  analogWrite(L_PWM1, 0);
  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, 0);
}

// ================= Encoder Reading =================

void readEncoder1() {
  int A = digitalRead(ENCODER_A1);
  int B = digitalRead(ENCODER_B1);
  if (A == B) {
    encoderCount1++;
  } else {
    encoderCount1--;
  }
}

void readEncoder2() {
  int A = digitalRead(ENCODER_A2);
  int B = digitalRead(ENCODER_B2);
  if (A == B) {
    encoderCount2++;
  } else {
    encoderCount2--;
  }
}

// ================= Webserver Handlers =================
void handleRoot() {
  server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 RC Car</title>
<style>
  body {
    background-color: black;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    height: 100vh;
    margin: 0;
  }
  button {
    font-size: 40px;
    padding: 30px 50px;
    margin: 20px;
    color: white;
    background-color: #333;
    border: none;
    border-radius: 15px;
  }
  button:hover {
    background-color: #555;
  }
</style>
</head>
<body>
  <button onclick="location.href='/forward'">W</button><br>
  <div>
    <button onclick="location.href='/left'">A</button>
    <button onclick="location.href='/stop'">X</button>
    <button onclick="location.href='/right'">D</button>
  </div><br>
  <button onclick="location.href='/backward'">S</button>
</body>
</html>
  )rawliteral");
}


void handleForward() {
  moveForward();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleBackward() {
  moveBackward();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLeft() {
  turnLeft();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleRight() {
  turnRight();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleStop() {
  stopMotors();
  server.sendHeader("Location", "/");
  server.send(303);
}

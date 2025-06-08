#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <ESP32Servo.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Inicialitza el display SH1106
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Pins
const int Trigger = 12;
const int Echo = 13;
const int ServoPin = 5;

Servo radarServo;

float getDistance();
void actualitzaRadar(int angle);

// SSID i pasword
const char* ssid = "GOUFONE-81669B";
const char* password = "eeeeeeee";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setupWifi() {
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);
  Serial.print("Connectant a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connectat! IP: ");
  Serial.println(WiFi.localIP());
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("[WS] Client connectat: #%u\n", client->id());
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("[WS] Client desconnectat: #%u\n", client->id());
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Radar ESP32</title>
  <style>
    body {
      font-family: sans-serif;
      background: #111;
      color: #0f0;
      text-align: center;
    }
    canvas {
      background: #000;
      border: 1px solid #0f0;
      margin-top: 20px;
    }
    #info {
      margin-top: 10px;
      font-size: 18px;
    }
    button {
      margin: 10px;
      padding: 10px 20px;
      font-size: 16px;
      background-color: #0f0;
      color: black;
      border: none;
      border-radius: 5px;
      cursor: pointer;
    }
    button:hover {
      background-color: #3f3;
    }
  </style>
</head>
<body>
  <h1>Radar ESP32</h1>
  <canvas id="radarCanvas" width="400" height="400"></canvas>
  <div id="info">
    <p><strong>Angle:</strong> <span id="angle">--</span>°</p>
    <p><strong>Distància:</strong> <span id="distance">--</span> cm</p>
  </div>
  <div>
    <button onclick="togglePause()"><span id="pauseText">⏸ Pausa</span></button>
    <button onclick="resetRadar()">🔁 Reinicia</button>
  </div>
  <script>
    const canvas = document.getElementById("radarCanvas");
    const ctx = canvas.getContext("2d");
    const centerX = canvas.width / 2;
    const centerY = canvas.height;
    const maxRadius = 300;
    let traces = new Array(76).fill(null);
    let paused = false;

    function degToRad(deg) {
      return deg * Math.PI / 180;
    }

    function drawRadar() {
      ctx.clearRect(0, 0, canvas.width, canvas.height);

      // Fons i cercles
      ctx.strokeStyle = "#0f0";
      ctx.lineWidth = 1;
      for (let r = 60; r <= maxRadius; r += 60) {
        ctx.beginPath();
        ctx.arc(centerX, centerY, r, Math.PI, 2 * Math.PI);
        ctx.stroke();
      }

      // Línies angulars
      [15, 45, 90, 135, 165].forEach(angle => {
        const rad = degToRad(angle);
        const x = centerX + maxRadius * Math.cos(rad);
        const y = centerY - maxRadius * Math.sin(rad);
        ctx.beginPath();
        ctx.moveTo(centerX, centerY);
        ctx.lineTo(x, y);
        ctx.stroke();
      });

      // Dibuixar traça
      ctx.strokeStyle = "#0ff";
      traces.forEach((d, i) => {
        if (d !== null) {
          const angle = 15 + i * 2;
          const rad = degToRad(angle);
          const radius = Math.min((d / 100) * maxRadius, maxRadius);
          const x = centerX + radius * Math.cos(rad);
          const y = centerY - radius * Math.sin(rad);
          ctx.beginPath();
          ctx.moveTo(centerX, centerY);
          ctx.lineTo(x, y);
          ctx.stroke();
        }
      });
    }

    function updateRadar(angle, distance) {
      if (paused) return;
      const index = Math.floor((angle - 15) / 2);
      if (index >= 0 && index < traces.length) {
        traces[index] = distance;
      }
      document.getElementById("angle").textContent = angle;
      document.getElementById("distance").textContent = distance.toFixed(1);
      drawRadar();
    }

    function togglePause() {
      paused = !paused;
      document.getElementById("pauseText").textContent = paused ? "▶ Continua" : "⏸ Pausa";
    }

    function resetRadar() {
      traces.fill(null);
      drawRadar();
    }

    const protocol = location.protocol === "https:" ? "wss" : "ws";
    const socket = new WebSocket(protocol + "://" + location.host + "/ws");

    socket.onmessage = (event) => {
      const data = JSON.parse(event.data);
      updateRadar(data.angle, data.distance);
    };
  </script>
</body>
</html>
)rawliteral";


void setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  server.begin();
}

// Mapeig d'angle (15°–165°) a pantalla (20–108)
int mapAngleToX(int angle) {
  return map(angle, 15, 165, 20, 108);
}

// Mapeig de distància a radi (centre cap avall)
int mapDistanceToY(float distance) {
  if (distance < 0) return 63;
  distance = constrain(distance, 0, 100);
  return 63 - map(distance, 0, 100, 0, 60);
}

// Variables per la traça persistent
#define NUM_ANGLES 76  // (165 - 15) / 2 + 1
float distancias[NUM_ANGLES];  // distàncies per cada angle

#define RADAR_CX 64
#define RADAR_CY 63
#define MAX_RADIUS 60

float degToRad(int deg) {
  return deg * PI / 180.0;
}


int angleToIndex(int angle) {
  return (angle - 15) / 2;
}

int indexToAngle(int index) {
  return index * 2 + 15;
}

void setup() {
  Serial.begin(115200);

  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  digitalWrite(Trigger, LOW);

  radarServo.setPeriodHertz(50);
  radarServo.attach(ServoPin, 500, 2400);

  Wire.begin(1, 2);  // Pins SDA i SCL del Display

  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);
  display.drawStr(10, 30, "Iniciant Radar...");
  display.sendBuffer();
  delay(1000);

  // Inicialitzar array de distàncies
  for (int i = 0; i < NUM_ANGLES; i++) {
    distancias[i] = -1;
  }

  // Inicalitzem la conexió wifi
  setupWifi();
  setupServer();
}

void loop() {
  for (int angle = 15; angle <= 165; angle += 2) {
    actualitzaRadar(angle);
  }
  for (int angle = 165; angle >= 15; angle -= 2) {
    actualitzaRadar(angle);
  }
}

void actualitzaRadar(int angle) {
  radarServo.write(angle);
  delay(30);  // estabilització servo

  float distance = getDistance();

  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print("°, Distància: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Desa la distància
  int idx = angleToIndex(angle);
  if (idx >= 0 && idx < NUM_ANGLES) {
    distancias[idx] = distance;
  }

  // Dibuix
  display.clearBuffer();

  // Línies de referència
  display.drawLine(64, 63, 64, 0); // 90°
  display.drawLine(0, 63, 127, 63); // horitzontal
  display.drawLine(64, 63, 64 + 42, 63 - 42); // 45°
  display.drawLine(64, 63, 64 - 42, 63 - 42); // 135°
  for (int r = 10; r <= 60; r += 10) {
    display.drawCircle(64, 63, r);
  }

 // Dibuixar línies de traça amb coordenades polars reals
for (int i = 0; i < NUM_ANGLES; i++) {
  float d = distancias[i];
  if (d > 0) {
    int a = indexToAngle(i);
    float angleRad = degToRad(a);
    float radius = constrain(map(d, 0, 100, 0, MAX_RADIUS), 0, MAX_RADIUS);
    int x = RADAR_CX + radius * cos(angleRad);
    int y = RADAR_CY - radius * sin(angleRad);
    display.drawLine(RADAR_CX, RADAR_CY, x, y);
  }
}

  // Text
  display.setFont(u8g2_font_5x7_tr);
  display.setCursor(0, 10);
  display.print("Angle: ");
  display.print(angle);
  display.print(" deg");

  display.setCursor(0, 20);
  display.print("Dist: ");
  display.print(distance, 0);
  display.print(" cm");

  display.sendBuffer();
  delay(40);

  // Envia dades al WebSocket
  String json = "{\"angle\":" + String(angle) + ",\"distance\":" + String(distance, 1) + "}";
  ws.textAll(json);
}

float getDistance() {
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  long duration = pulseIn(Echo, HIGH, 30000);
  if (duration == 0) return -1;
  return duration / 58.0;
}
#include <WiFi.h>
#include <WebServer.h>


// Practica A

// HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html>"
"<html lang=\"ca\">"
"<head>"
"    <meta charset=\"UTF-8\">"
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
"    <title>ESP32-S3 - Informació</title>"
"    <style>"
"        body {"
"            font-family: Arial, sans-serif;"
"            margin: 20px;"
"            padding: 20px;"
"            background-color: #f4f4f4;"
"        }"
"        .container {"
"            max-width: 800px;"
"            margin: auto;"
"            background: white;"
"            padding: 20px;"
"            border-radius: 10px;"
"            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);"
"            text-align: center;"
"        }"
"        h1, h2 {"
"            color: #333;"
"        }"
"        ul {"
"            list-style: none;"
"            padding: 0;"
"        }"
"        ul li {"
"            background: #ddd;"
"            margin: 5px 0;"
"            padding: 10px;"
"            border-radius: 5px;"
"        }"
"        img {"
"            max-width: 100%;"
"            height: auto;"
"            margin: 20px 0;"
"            border-radius: 10px;"
"        }"
"    </style>"
"</head>"
"<body>"
"    <div class=\"container\">"
"        <h1>Microcontrolador ESP32-S3</h1>"
"        <img src=\"https://www.mouser.es/images/espressifsystems/lrg/ESP32-S3-DEVKITC-1-N8_SPL.jpg\" alt=\"ESP32-S3 DevKit\">"
"        <p>L'ESP32-S3 és un microcontrolador de baix consum desenvolupat per Espressif. Està pensat per aplicacions IoT, intel·ligència artificial i sistemes embeguts.</p>"
"        "
"        <h2>Característiques principals:</h2>"
"        <ul>"
"            <li>Processador dual-core Xtensa LX7</li>"
"            <li>Connectivitat Wi-Fi i Bluetooth 5.0</li>"
"            <li>Suport per acceleració d'intel·ligència artificial</li>"
"            <li>Gran nombre de GPIOs i perifèrics</li>"
"            <li>Baix consum energètic</li>"
"        </ul>"
"        "
"        <h2>Aplicacions típiques:</h2>"
"        <ul>"
"            <li>Dispositius IoT</li>"
"            <li>Automatització domèstica</li>"
"            <li>Intel·ligència artificial en dispositius de baixa potència</li>"
"            <li>Robòtica i sistemes embeguts</li>"
"        </ul>"
"    </div>"
"</body>"
"</html>";


// SSID & Password
const char* ssid = "iPhone de Aina";  // Enter your SSID here
const char* password = "carreras";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

// Handle root url (/)
void handle_root() {
  server.send(200, "text/html", HTML);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_root);

  server.begin();
  Serial.println("HTTP server started");
  delay(100); 
}

void loop() {
  server.handleClient();
}

// Exercici de millora 1:
/*
// HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html>\
<html>\
<body>\
<h1>My Primera Pagina con ESP32 - Conexio AP &#128522;</h1>\
</body>\
</html>";


// SSID & Password
const char* ssid = "esp32";  // Enter your SSID here
const char* password = "123456789";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

// Handle root url (/)
void handle_root() {
  server.send(200, "text/html", HTML);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);        //Start Acces point mode
  while(!WiFi.softAP(ssid, password))
  {
  Serial.println(".");
    delay(100);
  }
  
  Serial.print("Iniciado AP ");
  Serial.println(ssid);
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());

  server.on("/", handle_root);
  server.begin();
  Serial.println("HTTP server started");
  delay(100); 
}

void loop() {
  server.handleClient();
}*/ 
#include <Arduino.h>

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);
  while (!Serial);  // Espera que el terminal USB estigui llest

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  Serial.println("Bucle UART0 <-> UART2 iniciat (esperant entrada amb Enter)");
}

void loop() {
  static String inputString = "";

  // Llegeix del Monitor Sèrie (UART0)
  if (Serial.available()) {
    char incomingByte = Serial.read();

    if (incomingByte == '\n' || incomingByte == '\r') {
      if (inputString.length() > 0) {
        // Envia tota la cadena a UART2
        Serial2.write(inputString.c_str());
        Serial2.write("\r\n");  // Opcional: envia salt de línia
        Serial.println("Enviat a UART2: " + inputString);
        inputString = "";
      }
    } else {
      inputString += incomingByte;
    }
  }

  // Llegeix del UART2 i mostra al monitor sèrie
  while (Serial2.available()) {
    char received = Serial2.read();
    Serial.write(received);
  }
}

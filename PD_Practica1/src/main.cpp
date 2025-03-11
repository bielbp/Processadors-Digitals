#include <Arduino.h>



// Practica 2 A


struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

Button button1 = {18, 0, false};

void IRAM_ATTR isr() {
  button1.numberKeyPresses += 1;
  button1.pressed = true;
}

void setup() {
  Serial.begin(115200);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
}

void loop() {
  if (button1.pressed) {
      Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
      button1.pressed = false;
  }

  //Detach Interrupt after 1 Minute
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();
    detachInterrupt(button1.PIN);
     Serial.println("Interrupt Detached!");
  }
}



// Pràctica 2 B

/*
volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(115200);
 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
 
}
 
void loop() {
 
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
 
  }
}
  */

#define DELAY 500
#define LED_PIN 2 //Utilitzarem el pin 2 gpio2


// Nota: Tots els exercicis estan comentats al mateix document. Treu la "/* */" per exectuar l'exercici desitjat.

// Exercici 1/2:

/*
void setup() {
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);

  Serial.println("Configuració completa!");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // Encén el LED
  Serial.println("ON");     //Envia missatge ON
  delay(DELAY);                   // Espera 500 ms
  digitalWrite(LED_PIN, LOW);   // Apaga el LED
  Serial.println("OFF");    //Envia missatge OFF
  delay(DELAY); 

}
*/

//Exercici 3:

uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG; // Establecer un puntero al registro de I/O
/*
void setup() {
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  *gpio_out |= (1 << 2); // Activar el bit correspondiente al pin 2
  Serial.println("ON");
  delay(DELAY);             
  *gpio_out ^= (1 << 2); // Alternar el estado del bit correspondiente al pin 2
  Serial.println("OFF");
  delay(DELAY); 

}
*/

// Exercici 4:

// 4.1 f=921,1Hz
/*void setup() {
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // Encén el LED
  Serial.println("ON");     //
  digitalWrite(LED_PIN, LOW);   // Apaga el LED
  Serial.println("OFF");
}*/

// 4.2 f=145,8Hz

/*void setup() {
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  *gpio_out |= (1 << 2); // Activar el bit correspondiente al pin 2
  Serial.println("ON");     //
  *gpio_out ^= (1 << 2); // Alternar el estado del bit correspondiente al pin 2
  Serial.println("OFF");
}*/

// 4.3 f=1,4M
/*
void setup() {
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);  // Encén el LED
  digitalWrite(LED_PIN, LOW);   // Apaga el LED
}
*/

// 4.4 f= No dona res per tant no tenim frequencia
/*
void setup() {
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  *gpio_out |= (1 << 2); // Activar el bit correspondiente al pin 2
  *gpio_out ^= (1 << 2); // Alternar el estado del bit correspondiente al pin 2
}
*/

// Activitat extra

/*#include <Arduino.h>
#include "esp_adc_cal.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
}

void loop() {
    
    float tempC = temperatureRead();  // Llegir la temperatura interna de l'ESP32-S3
    Serial.print("Temperatura: ");
    Serial.print(tempC);
    Serial.println(" °C");
    delay(5000);
}*/
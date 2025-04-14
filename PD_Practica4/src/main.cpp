#include <Arduino.h>


// Ejercicio pràctico 1:

/*
// this function will be invoked when additionalTask was created
void anotherTask( void * parameter )
{
  // loop forever
  for(;;)
  {
    Serial.println("this is another Task");
    delay(1000);
  }
  // delete a task when finish,
  // this will never happen because this is infinity loop
  vTaskDelete( NULL );
}

void setup()
{
  Serial.begin(112500);
  // we create a new task here
  xTaskCreate(
    anotherTask, // Task function.
    "another Task", // name of task.
    10000, // Stack size of task
    NULL, // parameter of the task
    1, // priority of the task
    NULL); // Task handle to keep track of created task
}
 
// the forever loop() function is invoked by Arduino ESP32 loopTask 
void loop()
{
  Serial.println("this is ESP32 Task");
  delay(1000);
}

*/

// Ejercicio Propuesto: Hacer dos funciones que enciendan y apaguen dos LEDs diferentes con una frecuencia distinta
/*
const int led1 = 20;
const int led2 = 21;

void encendreLED1(void * parameter)
{
  // loop forever
  for(;;){ 
    // Turn the LED on
    digitalWrite(led1, HIGH);
    // Pause the task for 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);
    // Turn the LED off
    digitalWrite(led1, LOW);
    // Pause the task again for 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void encendreLED2(void * parameter)
{
  // loop forever
  for(;;)
  {
    // Turn the LED on
    digitalWrite(led2, HIGH);  // <-- CORREGIT
    // Pause the task for 2300ms
    vTaskDelay(2300 / portTICK_PERIOD_MS);
    // Turn the LED off
    digitalWrite(led2, LOW);  // <-- CORREGIT
    // Pause the task again for 2300ms
    vTaskDelay(2300 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);

  // Configurar els LEDs com a sortida
  pinMode(led1, OUTPUT);  // <-- AFEGIT
  pinMode(led2, OUTPUT);  // <-- AFEGIT

  xTaskCreate(
    encendreLED1, // Task function.
    "Encendre led f1", // name of task.
    10000, // Stack size of task
    NULL, // parameter of the task
    1, // priority of the task
    NULL); // Task handle to keep track of created task
  

  xTaskCreate(
    encendreLED2, // Task function.
    "Encendre led f2", // name of task.
    10000, // Stack size of task
    NULL, // parameter of the task
    1, // priority of the task
    NULL); // Task handle to keep track of created task
}

void loop()
{
  // No es fa res a loop() perquè les tasques s'executen en paral·lel
}
*/

 

// Ejercicio Practico 2:

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

const int led1 = 1;
SemaphoreHandle_t semafor;

void EncendreLED(void * parameter)
{
  while (true)
  {
    if (xSemaphoreTake(semafor, portMAX_DELAY)) // Agafa el semàfor
    {
      Serial.println("Encenent LED...");
      digitalWrite(led1, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      xSemaphoreGive(semafor); // Allibera el semàfor
    }
    vTaskDelay(100 / portTICK_PERIOD_MS); // Petita espera per evitar bloquejos
  }
}

void ApagarLED(void * parameter)
{
  while (true)
  {
    if (xSemaphoreTake(semafor, portMAX_DELAY)) // Agafa el semàfor
    {
      Serial.println("Apagant LED...");
      digitalWrite(led1, LOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      xSemaphoreGive(semafor); // Allibera el semàfor
    }
    vTaskDelay(100 / portTICK_PERIOD_MS); // Petita espera per evitar bloquejos
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(led1, OUTPUT);

  semafor = xSemaphoreCreateBinary(); // Crear semàfor binari

  if (semafor != NULL) {
    xSemaphoreGive(semafor); // Inicialitzar-lo en estat disponible
  }

  xTaskCreate(
    EncendreLED,
    "Encendre LED",
    1000,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    ApagarLED,
    "Apagar LED",
    1000,
    NULL,
    1,
    NULL
  );
}

void loop()
{
}



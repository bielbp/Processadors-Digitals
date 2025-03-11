# PRACTICA 2 : INTERRUPCIONES 

El objetivo de la practica es comprender el funcionamiento de las interrupciones.

Para lo cual realizaremos una practica  donde controlaremos  2 leds  de una forma periódica  y una entrada ; de forma que el uso de la entrada provoque un cambio de frecuencia de las oscilaciones  pero solo en un led.

## Practica A interrupción por GPIO 

El montaje a realizar es el siguiente 

![](https://descubrearduino.com/wp-content/uploads/2020/06/conexion-botones-ESP32-para-interrupci%C3%B3n-GPIO.png)

Ejemplo de código :


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

### informe

Para empezar se crea un struct Button con los siguientes campos: numero de pin, numero pulsaciones y un booleano que nos indica si el pulsador esta pulsado o no. También se define una variable de tipo Button previamente definido y la funcion que se llama durante la interrupcion.  

En el setup se inicializa el puerto serie, el pin de entrada y la interrupcion.  

En el loop se define una primera condición, si se pulsa el boton, sale por pantalla la cuenta de veces que se ha pulsado el boton y se pone la variable pressed en false. Despues de un minuto se desconetca la interrupcion.
En el pierto serie, se nos indica cada vez que pulsamos el pulsador con un mensaje que lleva la cuenta de todas las veces que hemos provocado una interrupcion. A los 60 segundos, nos envia el mensaje de "Interrupt Detached" que nos indica que se ha desconectado la interrupcion.  

Como la conexion no es perfecta y la velocidad del microprocesador es muy alta, cada vez que pulsamos el pulsador se añade a la cuenta varias interrupciones. Este fenomeno se llama rebote (debounce).
Hay varias formas de arreglar esto, por ejemplo conectando un condensador en paralelo con la resistencia de pull-up con el tiempo lo suficientemente pequeño para que el usuario no lo note. Tambien se puede arreglar con software añadiendo un delay en cada interrupcion. 

## Practica B interrupción por timer 

El codigo de la practica es el siguiente 
```
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
```

### informe

Para empezar se declaran las variables globales de los contadores de interrupciones y se declara la funcion de insterrupcion.  

En el setup se inicializa el puerto serie, el temporizador con su división de frequencia a 80, asocia la interrupcion del temporizador con la funcion de interrupcion, configura el temporizador para activar la interrupcion cada segundo y activa la alarma del temporizador.  

En la funcion loop primero comprueva si ha ocurrido una interrupcion, incrementa el contador total de interrupciones y muestra por el monitor serie el mensaje que nos indica cada vez que se produce una interrupcion y un contador del total de interrupciones que se han producido.
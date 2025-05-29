# PRACTICA 8 :  Buses de comunicación IV (uart)  

El objetivo de la practica es comprender el funcionamiento de la comunicacion serie asincrona 
usart.
Esta comunicacion  es muy utilizada de hecho en todas las practicas las estamos usando 
cuando nos referimos a la impresora serie  Serial.println("").

Este tipo de protocolo dispone de muchos perifericos  y por tanto la comprension de su funcionamiento es basica para cualquier  ingeniero electronico.

## Niveles de comunicacion

En funcion de como se identifique el **nivel logico** tendremos diferentes metodos de comunicacion . Los principales  son los indicados a continuación.

### RS232:
Utiliza niveles de tensión para representar los estados lógicos (0 y 1). Es un sistema punto a punto (solo dos dispositivos pueden comunicarse entre sí). Los niveles lógicos suelen estar entre ±3V y ±15V. Por ejemplo, el nivel lógico 1 (MARK) se representa con una tensión negativa (como −12V), y el 0 (SPACE) con una tensión positiva (como +12V).

Ejemplo de uso:
Conexión entre un ordenador y un módem externo, o entre un ordenador y una placa Arduino para depuración a través del puerto serie (usando un conversor USB a serie).

### RS485:
Emplea corrientes diferenciales para transmitir datos, lo que le da una alta inmunidad al ruido eléctrico. Es ideal para largas distancias y ambientes industriales. Permite comunicación multidrop, es decir, varios dispositivos pueden compartir el mismo bus (comunicación maestro-esclavo o entre pares).

Ejemplo de uso:
Red de sensores industriales que se comunican con un PLC (Controlador Lógico Programable) usando el protocolo Modbus RTU sobre RS485.

### RS422:
También utiliza señalización diferencial, pero está pensado para comunicación punto a punto (aunque permite velocidades más altas y mayor distancia que RS232). Ofrece muy buena inmunidad al ruido.

Ejemplo de uso:
Transmisión de datos entre un sistema de control de iluminación teatral y una consola de control DMX512 (que puede usar RS422 como capa física).

## Software de arduino 

El control de los perifericos viene soportado en arduino por una API  ( objeto ) Serial ,derivada de la clase stream con la que se puede controlar con facilidad  cualquier protocolo de comunicación . 

Las funciones principales 

* `if(Serial):` Comprueba si el puerto serie está disponible
```
if (Serial) {
  Serial.println("Puerto serie disponible");
}
```
* `available():`Devuelve el número de bytes disponibles para leer del buffer de entrada.
```
if (Serial.available() > 0) {
  char c = Serial.read();
  Serial.print("Recibido: ");
  Serial.println(c);
}
```
* `availableForWrite():`Devuelve el número de bytes disponibles en el buffer de salida.
```
if (Serial.availableForWrite() > 0) {
  Serial.println("Listo para escribir");
}
```
* `begin():`Inicializa la comunicación serie a la velocidad indicada
```
Serial.begin(115200);
```
* `end():`Finaliza la comunicación serie.
```
Serial.end();
```
* `find():`Busca una cadena en el flujo de datos de entrada.
```
if (Serial.find("OK")) {
  Serial.println("Cadena OK recibida");
}
```
* `findUntil():`Busca una cadena hasta encontrar otra cadena límite.
```
if (Serial.findUntil("clave", "fin")) {
  Serial.println("Clave detectada antes de 'fin'");
}
```
* `flush():`Espera hasta que se hayan enviado todos los datos del buffer de salida.
```
Serial.print("Enviando...");
Serial.flush();
```
* `parseFloat():`Lee un número decimal (float) desde la entrada serie.
```
float valor = Serial.parseFloat();
Serial.println(valor * 2);
```
* `parseInt():`Lee un número entero desde la entrada serie.
```
int valor = Serial.parseInt();
Serial.println(valor + 10);
```
* `peek():`Lee el siguiente byte sin eliminarlo del buffer.
```
if (Serial.available()) {
  char c = Serial.peek();
  Serial.print("Siguiente byte: ");
  Serial.println(c);
}
```
* `print():`Envía datos al puerto serie sin salto de línea.
```
Serial.print("Valor: ");
Serial.print(42);
```
* `println():`Igual que print(), pero añade un salto de línea al final.
```
Serial.println("Hola mundo");
```
* `read():`Lee un byte del buffer de entrada.
```
char c = Serial.read();
Serial.print("Leído: ");
Serial.println(c);
```
* `readBytes():`Lee varios bytes en un array hasta alcanzar la longitud o agotar el tiempo.
```
char datos[10];
Serial.readBytes(datos, 10);
```
* `readBytesUntil():`Lee bytes hasta encontrar un carácter específico o alcanzar la longitud máxima.
```
char mensaje[20];
Serial.readBytesUntil('\n', mensaje, 20);
```
* `readString():`Lee toda la entrada disponible como una cadena.
```
String texto = Serial.readString();
Serial.println("Recibido: " + texto);
```
* `readStringUntil():`Lee una cadena hasta encontrar un carácter específico.
```
String linea = Serial.readStringUntil('\n');
Serial.println("Línea: " + linea);
```
* `setTimeout():`Establece el tiempo de espera máximo para funciones como readBytes o parseInt.
```
Serial.setTimeout(2000); // 2 segundos
```
* `write():`Envía bytes directamente al puerto serie.
```
Serial.write('A'); // Envia el byte 0x41
```
* `serialEvent():`Función especial que se ejecuta automáticamente cuando llegan datos por serie (en ciertas placas).
```
void serialEvent() {
  while (Serial.available()) {
    char c = Serial.read();
    Serial.print("Evento: ");
    Serial.println(c);
  }
}
```

## Ejercicio practico 1  bucle de comunicacion uart2

### Montaje
Para este ejercicio no se necesita ningun montaje previo a parte de un puente entre los pines de entrada y salida que se definira en el codigo.

### Cabezera
En la cabezera se configura los pines físicos que usará el puerto serie Serial2 del ESP32 en este caso el pin 16 y el 17.
```
#define RXD2 16
#define TXD2 17
```

### Setup
Para empezar, en el setup se inicia la conexion serie. Cuando la conexion serie esta lista, se inicia la conexion UART2 con la misma velocidad y usando los pines definidos en la cabezera. Para terminar con el setup escribe por el puerto serie un mensaje de confirmacion.
```
void setup() {
  Serial.begin(115200);
  while (!Serial);  // Espera que el terminal USB estigui llest

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  Serial.println("Bucle UART0 <-> UART2 iniciat (esperant entrada amb Enter)");
}
```

### Loop
En el loop, se acumula texto introducido per monitor serie (UART0) hasta que se presiona enter. Al presionar enter se envia l acadena por el Serial 2(UART2) y se muestra una confirmacion por pantalla. Luego se leen los datos recibidos por UART2 y los reenvia al terminal por el puerto serie porque aparezca de nuevo por pantalla. 
```
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
```
### Salida puerto serie

Para empezar se imprime el mensaje de esperando entrada una vez al iniciar el programa. Al pulsar enter se envia un mensaje de confirmacion i para finalizar se vuelve a mostrar la cadena enviada. 
```
Bucle UART0 <-> UART2 iniciat (esperant entrada amb Enter)
Enviat a UART2: Hola mundo
Hola mon
```
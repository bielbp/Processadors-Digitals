# PRACTICA 5 :  Buses de comunicación I (introducción y I2c)

El objetivo de la practica es comprender el funcionamiento de los buses sistemas de comunicación entre periféricos;  estos elementos pueden ser internos o externos  al procesador.

## Ejercicio Practico 2   Display oled** **I2C SSD1306 OLED

Este ejercicio permite visualizar diferentes formas geométricas, texto y animaciones en una pantalla OLED SSD1306 mediante comunicación I2C. Se utiliza la librería Adafruit_SSD1306 junto con Adafruit_GFX para facilitar el dibujo en el display. El objetivo principal es demostrar las capacidades gráficas de este tipo de pantallas mediante una serie de pruebas visuales.

### Cabezera
En la cabecera del código se incluyen todas las librerías necesarias, constantes de configuración y variables para inicializar y controlar la pantalla OLED. También se define un pequeño gráfico en formato bitmap que se utilizará en una de las pruebas.
```
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Declaración de funciones de prueba
void testdrawline(void);
void testdrawrect(void);
void testfillrect(void);
void testdrawcircle(void);
void testfillcircle(void);
void testdrawroundrect(void);
void testfillroundrect(void);
void testdrawtriangle(void);
void testfilltriangle(void);
void testdrawchar(void);
void testdrawstyles(void);
void testscrolltext(void);
void testdrawbitmap(void);
void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h);

// Configuración de pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Gráfico de ejemplo en bitmap
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] = {
  B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

```
### Setup
En la función setup() se inicializa el puerto serie y la pantalla OLED. Si la inicialización es exitosa, se muestra brevemente el logotipo de Adafruit y se realiza una serie de pruebas gráficas. Estas pruebas incluyen dibujo de líneas, rectángulos, círculos, texto, imágenes y texto desplazable. Cada test se muestra durante unos segundos para permitir la observación en la pantalla.
```
void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  display.drawPixel(10, 10, SSD1306_WHITE);
  display.display();
  delay(2000);

  // Ejecución de pruebas gráficas
  testdrawline();
  testdrawrect();
  testfillrect();
  testdrawcircle();
  testfillcircle();
  testdrawroundrect();
  testfillroundrect();
  testdrawtriangle();
  testfilltriangle();
  testdrawchar();
  testdrawstyles();
  testscrolltext();
  testdrawbitmap();

  display.clearDisplay();
  testdrawchar(); // Muestra final
}
```
### Loop
La función loop() está vacía ya que todo el comportamiento visual se ejecuta una única vez en setup() y no es necesario repetirlo continuamente.
```
void loop() {
}
```
### Salida del puerto serie
En este caso no tenemos mensajes desde el puerto serie ya que se muestra todo por el display.
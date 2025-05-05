# PRACTICA 7 :  Buses de comunicación III (I2S)  

El objetivo de la practica actual es describir el funcionamiento del bus I2S y realizar una practica para comprender su  funcionamiento 

## Ejercicio Practico 2  reproducir un archivo WAVE en ESP32 desde una tarjeta SD externa

queremos reproducir el archivo WAVE que mencioné al principio de este tutorial a través del ESP32 NodeMCU y el altavoz. Debido a que el ESP32 tiene que leer el archivo WAVE y reenviar la señal de audio digital al MAX98357A, tenemos que usar una tarjeta SD con el archivo WAVE. También puede utilizar un archivo MP3 en lugar del archivo WAVE.

La siguiente imagen muestra el cableado del ESP32 NodeMCU con el módulo de tarjeta (Micro) SD, el MAX98357A y el altavoz. En la imagen que ve, tiene que cambiar el pin DIN del MAX98357A, en comparación con el segundo proyecto.

![](https://892962.smushcdn.com/2087382/wp-content/uploads/2020/08/I2S-ESP32-Play-from-SD-Card_Steckplatine.png?lossy=1&strip=1&webp=1)


### Cabezera
en la cabezera se incluyen las librerias necesarias de Arduino, de audio, de lectura de targetas sd y de ficheros. Se definen los pines de entrada y salida utilizados para la comunicacion SPI con la targera SD y I2S. Para terminar con la cabezera se crea un objeto de classe `Audio` que serà el encargado de controlar toda la reproduccion del audio.
```c
#include <Arduino.h>
#include "Audio.h"
#include "SD.h"
#include "FS.h"

// Digital I/O used
#define SPI_MOSI      11
#define SPI_MISO      13
#define SPI_SCK       12
#define SD_CS         10
#define I2S_DOUT      1
#define I2S_BCLK      42
#define I2S_LRC       41

Audio audio;
```
### Setup
En la funcion setup se configuran el pin de seleccion SD como salida con la funcion `pinMode(SD_CS, OUTPUT)` y lo deja en estado alto con `digitalWrite(SD_CS, HIGH)`. Inicia la comunicacion SPI con la funcion `SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI)`. Se inicilaliza el puerto serie y la targeta sd con `SD.begin(SD_CS)`. Mas tarde confugura la salida del audio con los pines assignados `audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT)` y ajusta el volumen de salida con `audio.setVolume(10)`. Para acabar con el setup se inidca cual es el fichero de audio que se quiere reproducir con la funcion `audio.connecttoFS(SD, "prova.wav")`.
```c
void setup(){
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    Serial.begin(115200);
    SD.begin(SD_CS);
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(10); // 0...21
    audio.connecttoFS(SD, "prova.wav");
}
```
### Loop
El loop es sencillo. Se llama a la fincion `audio.loop()` que mantiene el proceso de reproduccion en marcha.
```c
void loop(){
    audio.loop();
}
```
### Funciones de depuracion
Al final del progama se definen estas funciones opcionales que sirven para dar informacion opcional sobre la reproduccion y permiten añadir funcionalidades personalizadas.
```c
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreaminfo(const char *info){
    Serial.print("streaminfo  ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}
```
### Salida puerto serie
Si se utilizaran mas funciones de depuracion las salidas del puerto serie dependerian de las funciones que utilizemos. En este caso, sin funciones de depuracion, no hay salida por el puerto serie.

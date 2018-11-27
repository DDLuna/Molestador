#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <Esplora.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <SoftwareSerial.h>
#include "notas.h"
#include "melodias.h"
#include "bitmaps.h"

// pins para conectar la pantalla 
#define SCLK 8 
#define MOSI 7
#define DC   16
#define RST  15
#define CS   -1 // No lo usamos
#define MISO -1 // No lo usamos

// pins para bluetooth
#define TX 3
#define RX 11


// Estado desafio
const int BOTONES = 1;
const int MOVERSE = 2;
const int ANDROID = 3;
const int FIN = 4;

// Cantidad de repeteciones de cada desafío
const int REPETICIONES = 3;
int repeticion = 0;

Adafruit_ILI9341 tft = Adafruit_ILI9341(CS, DC, MOSI, SCLK, RST, MISO);
SoftwareSerial bluetooth(RX, TX);

int minutoActual;
int sonando = false;
bool activo = false;
bool androidStart = true;
int desafio = BOTONES;
int alarmID = -1;

void setup() {
  Serial.begin(38400);
  bluetooth.begin(38400);
  tft.begin();
  tft.setRotation(3);
  setTime(0);
  minutoActual = minute();
  tft.fillScreen(ILI9341_BLACK);
  dibujarHora();
  dibujarTemp();
}

void loop() {
  if (bluetooth.available()) {
    recibirMensaje();
  }
  if (minutoActual != minute()) {
    dibujarTemp();
    minutoActual = minute();
    dibujarHora();
  }
  if (activo) {
    if (!luzAlta()) {
      return;
    }
    switch (desafio) {
      case BOTONES:
        sonar(marioMelodia, marioTempo, sizeof(marioMelodia) / sizeof(int));
        desafioBotones();
        break;
      case MOVERSE:
        sonar(underMelodia, underTempo, sizeof(underMelodia) / sizeof(int));
        desafioMoverse();
        break;
      case ANDROID:
        sonar(marioMelodia, marioTempo, sizeof(marioMelodia) / sizeof(int));
        //sonar(starWarsMelodia, starWarsTempo, sizeof(starWarsMelodia) / sizeof(int));
        desafioAndroid();
        break;
      case FIN:
        inactivar();
    }
  }
  Alarm.delay(0); // Necesario para que funcione la biblioteca de alarmas.
}

//-------------------LUZ----------------------------

bool luzEstado = true;
bool inicial = true;
unsigned long bufferLuz = 0;

// El sensor de luz devuelve valores entre 0 y 1023
// Para cambiar de estado el sensor debe pasar el umbral de 800 o 200
// según corresponda

bool luzAlta() {
  if (inicial) {
    tft.drawBitmap(270, 0, lampara, 50, 50, ILI9341_GREEN);
    inicial = false;
  }
  bool toggle = false;
  int luz = Esplora.readLightSensor();
  if (luz > 999) {
    luz = 999;
  }
  if (luz < 200 && luzEstado) {
    tft.drawBitmap(270, 0, lampara, 50, 50, ILI9341_RED);
    luzEstado = false;
    toggle = true;
  }
  if (luz > 800 && !luzEstado) { 
    tft.drawBitmap(270, 0, lampara, 50, 50, ILI9341_GREEN); 
    luzEstado = true;
    toggle = true;
  }
  if(millis() - bufferLuz > 1000 || toggle) {
    if (luzEstado) {
      tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    } else {
      tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    }
    bufferLuz = millis();
    tft.setCursor(270, 50);
    tft.setTextSize(2);
    tft.println(luz);
  }
  return luzEstado;
}


//----------------------------TEMPERATURA-----------------------

void dibujarTemp() {
  tft.setCursor(0,0);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  tft.println(String(Esplora.readTemperature(DEGREES_C)) + "C");
}


//----------------------------MUSICA---------------------------

unsigned long tiempoDesdeNota = 0;
int nota = 0;

// @param melodia: Array de enteros que indica la nota a tocar
// @param tempo: Array de enteros que indica el tempo de la nota a tocar
// @param tam tamaño de los vectores de melodia o nota

void sonar(int melodia[], int tempo[], int tam) {
  if(millis() - tiempoDesdeNota >= 1500 / tempo[nota]) {
    Esplora.tone(melodia[nota], 1000 / tempo[nota]);
    tiempoDesdeNota = millis();
    nota++;
    nota %= tam;
  }
}

unsigned long tiempoDesdeNotaMillis = 0;
int notaMilli = 0;

// Esta función hace lo mismo que sonar, pero funciona para vectores de tempo
// en milisegundos en vez de tempo.

void sonarEnMillis(int melodia[], int tempo[], int tam) {
  if (millis() - tiempoDesdeNotaMillis >= 1.5 * tempo[notaMilli]) {
    Esplora.tone(melodia[nota], tempo[nota]);
    tiempoDesdeNotaMillis = millis();
    notaMilli++;
    nota %= tam;
  }
}

// Esta funcion reinicia la cancion al principio de la misma, se invoca
// al cambiar de desafío

void resetCancion() {
  nota = 0;
}

//--------------------COMUNICACION BT-----------------------------
// in
const char SET_HORA = '0';
const char SET_ALARMA = '1';
const char SIG_DESAFIO = '2';
const char INFO_DESAFIO = '3';
const char APAGAR_ALARMA = '4';

void recibirMensaje() {
  Serial.print("<-");
  char mensaje = bluetooth.read();
  Serial.println(mensaje);
  switch(mensaje) {
    case INFO_DESAFIO:
      actualizarInfoAndroid();
      break;
    case SET_HORA:
      setHora(bluetooth.readString());
      break;
    case SET_ALARMA:
      setAlarma(bluetooth.readString());
      break;
    case SIG_DESAFIO:
      desafio++;
      break;
    case APAGAR_ALARMA:
      inactivar();
      break;
  }
}

//int largoMensaje = String("Pasadas: 10 - Giros: 10").length();

const int largoCadena = 24;
char mensaje[largoCadena];

void actualizarInfoAndroid() {
  mensaje[largoCadena - 1] = '\0';
  //mensaje = mensaje.substring(mensaje.length() - largoMensaje);
  for (int i = 0; i < largoCadena - 1; i++) {
    mensaje[i] = bluetooth.read();
    Serial.println(mensaje[i] + " i");
  }
  Serial.println(mensaje);
  tft.setCursor(20, 185);
  tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  tft.setTextSize(2);
  tft.print(mensaje);
}

// out

const int ACTIVAR_SENSORES = 0;
const int INFO_DESAFIO_BOTONES = 1;
const int INFO_DESAFIO_MOVERSE = 2;


void activarSensores() {
  bluetooth.println(String(ACTIVAR_SENSORES));
}

int botonCounter = 0;

void enviarInfoBotones() {
  bluetooth.println(String(INFO_DESAFIO_BOTONES) + "Boton: " + String(((botonCounter) + (repeticion * 4))) +
      " de " + String(REPETICIONES * 4));
}

void enviarInfoMoverse() {
  bluetooth.println(String(INFO_DESAFIO_MOVERSE)
    + "Repeticion: " + String(repeticion + 1) + " de " + String(REPETICIONES));
}
//----------------------------TIEMPO-------------------------

void dibujarHora() {
  int hora = hour();
  int minuto = minute();
  char tiempo[6];
  tiempo[0] = (char) hora / 10 + '0';
  tiempo[1] = (char) hora % 10 + '0';
  tiempo[2] = ':';
  tiempo[3] = (char) minuto / 10 + '0';
  tiempo[4] = (char) minuto % 10 + '0';
  tiempo[5] = '\0';
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setCursor(65, 0);
  tft.setTextSize(6);
  tft.println(tiempo);
}

void setHora(String hora) {
  setTime(hora.toInt());
  dibujarHora();
  
}

void setAlarma(String tiempo) {
  Serial.println(alarmID);
  int hora = tiempo.substring(0, 2).toInt();
  int minutos = tiempo.substring(3, 5).toInt();
  if (alarmID >= 0) {
    Alarm.disable(alarmID);
  }
  alarmID = Alarm.alarmRepeat(hora, minutos, 0, activar);
  String horaPantalla = String(hora) + ':' + minutos;
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setCursor(110, 50);
  tft.setTextSize(3);
  tft.println(tiempo);
}

void shuffle(int items[], int tam) {
  for (int i = 0; i < tam; i++) {
    int rando = random(0, tam);
    cambiar(&items[i], &items[rando]);
  }
}

//--------------------------MISC-------------------------------------------

void activar() {
  activo = true;
  tft.fillRect(110, 50, 120, 25, ILI9341_BLACK);
}

void inactivar() {
  desafio = BOTONES;
  tft.fillRect(0, 0, 320, 240, ILI9341_BLACK);
  androidStart = true;
  dibujarHora();
  dibujarTemp();
  activo = false;
  nota = 0;
  Esplora.writeRGB(0, 0, 0);
}

void cambiar(int *n1, int *n2) {
  int aux = *n1;
  *n1 = *n2;
  *n2 = aux;
}

//--------------------DESAFIOS--------------------------------------------------
//                    BOTONES

int botones[4] = {
  SWITCH_1,
  SWITCH_2,
  SWITCH_3,
  SWITCH_4,
};

bool desafioBotonesStart = true;
const int RADIO_BOTON = 15;


void desafioBotones() {
  if (desafioBotonesStart) {
    randomSeed(millis());
    shuffle(botones, 4);
    desafioBotonesStart = false;
    Esplora.writeRGB(255,255,0);
    dibujarBotones(botones[botonCounter]);
  }
  if (Esplora.readButton(botones[botonCounter]) == LOW) {
    dibujarBoton(botones[botonCounter], ILI9341_GREEN);
    botonCounter++;
    dibujarBoton(botones[botonCounter], ILI9341_YELLOW);
    enviarInfoBotones();
  }
  if (botonCounter == 4) {
    limpiarBotones();
    botonCounter = 0;
    desafioBotonesStart  = true;
    repeticion++;
    if (repeticion == REPETICIONES) {
      repeticion = 0;
      desafio++;
      resetCancion();
    }
  }
}

void dibujarBoton(int boton, int color) {
  switch(boton) {
    case(SWITCH_UP):
      tft.fillCircle(160, 120, RADIO_BOTON, color);
      break;
    case(SWITCH_LEFT):
      tft.fillCircle(120, 160, RADIO_BOTON, color);
      break;
    case(SWITCH_RIGHT):
      tft.fillCircle(200, 160, RADIO_BOTON, color);
      break;
    case(SWITCH_DOWN):
      tft.fillCircle(160, 200, RADIO_BOTON, color);
      break;
  }
}

void dibujarBotones(int botonAPresionar) {
  if (botonAPresionar == SWITCH_UP) {
    tft.fillCircle(160, 120, RADIO_BOTON, ILI9341_YELLOW);
    tft.fillCircle(120, 160, RADIO_BOTON, ILI9341_GREEN);
    tft.fillCircle(200, 160, RADIO_BOTON, ILI9341_GREEN);
    tft.fillCircle(160, 200, RADIO_BOTON, ILI9341_GREEN);
    return;
  }
  if (botonAPresionar == SWITCH_LEFT) {
    tft.fillCircle(160, 120, RADIO_BOTON, ILI9341_GREEN);
    tft.fillCircle(120, 160, RADIO_BOTON, ILI9341_YELLOW);
    tft.fillCircle(200, 160, RADIO_BOTON, ILI9341_GREEN);
    tft.fillCircle(160, 200, RADIO_BOTON, ILI9341_GREEN);
    return;
  }
  if (botonAPresionar == SWITCH_RIGHT) {
    tft.fillCircle(160, 120, RADIO_BOTON, ILI9341_GREEN);
    tft.fillCircle(120, 160, RADIO_BOTON, ILI9341_GREEN);
    tft.fillCircle(200, 160, RADIO_BOTON, ILI9341_YELLOW);
    tft.fillCircle(160, 200, RADIO_BOTON, ILI9341_GREEN);
    return;
  }
  tft.fillCircle(160, 120, RADIO_BOTON, ILI9341_GREEN);
  tft.fillCircle(120, 160, RADIO_BOTON, ILI9341_GREEN);
  tft.fillCircle(200, 160, RADIO_BOTON, ILI9341_GREEN);
  tft.fillCircle(160, 200, RADIO_BOTON, ILI9341_YELLOW);
}

void limpiarBotones() {
  tft.fillCircle(160, 120, RADIO_BOTON, ILI9341_BLACK);
  tft.fillCircle(120, 160, RADIO_BOTON, ILI9341_BLACK);
  tft.fillCircle(200, 160, RADIO_BOTON, ILI9341_BLACK);
  tft.fillCircle(160, 200, RADIO_BOTON, ILI9341_BLACK);
}

int otroBotonPresionado(int botonPresionado) {
  if (botonPresionado != SWITCH_UP && Esplora.readButton(SWITCH_UP) == LOW) {
    tft.fillCircle(160, 120, 20, ILI9341_RED);
    return SWITCH_UP;
  }
  if (botonPresionado != SWITCH_LEFT && Esplora.readButton(SWITCH_LEFT) == LOW) {
    tft.fillCircle(120, 160, 20, ILI9341_RED);
    return SWITCH_LEFT;
  }
  if (botonPresionado != SWITCH_RIGHT && Esplora.readButton(SWITCH_RIGHT) == LOW) {
    tft.fillCircle(200, 160, 20, ILI9341_RED);
    return SWITCH_RIGHT;
  }
  if (botonPresionado != SWITCH_DOWN && Esplora.readButton(SWITCH_DOWN) == LOW) {
    tft.fillCircle(160, 200, 20, ILI9341_RED);
    return SWITCH_DOWN;
  }
  
  return -1;
}

// CAJITA
int moverseStart = true;
int posJug[2] = {160, 170};
int posObj[2];
long unsigned moverseBuffer = 0;
const int X = 0;
const int Y = 1;
const int DEADZONE = 50;
const int DIV_DIGITAL = 128;

void desafioMoverse() {
  randomSeed(millis());
  if (moverseStart) {
    tft.drawRect(0, 100, 320, 140, ILI9341_BLUE);
    posObj[X] = random(1, 310);
    posObj[Y] = random(101, 230);
    tft.drawPixel(posJug[X], posJug[Y], ILI9341_WHITE);
    tft.drawRect(posObj[X], posObj[Y], 10, 10, ILI9341_MAGENTA);
    moverseStart = false;
    Esplora.writeRGB(255, 0, 255);
  }
  if (millis() - moverseBuffer > 10) {
    moverseBuffer = millis();
    int stickX = Esplora.readJoystickX();
    int stickY = Esplora.readJoystickY();
    if (abs(stickX) > DEADZONE) {
      for (int i = 0; i < stickX / DIV_DIGITAL; i++) {  // izquierda
        if (posJug[X] <= 1) {
          break;
        }
        tft.drawPixel(posJug[X], posJug[Y], ILI9341_BLACK);
        posJug[X]--;
      }
        tft.drawPixel(posJug[X], posJug[Y], ILI9341_WHITE);
      for (int i = 0; i > stickX / DIV_DIGITAL; i--) {    // derecha
        if (posJug[X] >= 318) {
          break;
        }
        tft.drawPixel(posJug[X], posJug[Y], ILI9341_BLACK);
        posJug[X]++;
        tft.drawPixel(posJug[X], posJug[Y], ILI9341_WHITE);
      } 
    }
    if (abs(stickY) > DEADZONE) {
      for (int i = 0; i < stickY / DIV_DIGITAL; i++) {    // abajo
        if (posJug[Y] >= 238) {
          break;
        }
        tft.drawPixel(posJug[X], posJug[Y], ILI9341_BLACK);
        posJug[Y]++;
        tft.drawPixel(posJug[X], posJug[Y], ILI9341_WHITE);
      }
      for (int i = 0; i > stickY / DIV_DIGITAL; i--) {    // arriba
        if (posJug[Y] <= 101) {
          break;
        }
        tft.drawPixel(posJug[X], posJug[Y], ILI9341_BLACK);
        posJug[Y]--;
        tft.drawPixel(posJug[X], posJug[Y], ILI9341_WHITE);
      }  
    }
  }
  if (posJug[X] >= posObj[X] && posJug[X] <= posObj[X] + 10 &&
      posJug[Y] >= posObj[Y] && posJug[Y] <= posObj[Y] + 10) {
    enviarInfoMoverse();
    moverseStart = true;
    tft.drawRect(posObj[X], posObj[Y], 10, 10, ILI9341_BLACK);
    repeticion++;
    if (repeticion >= REPETICIONES) {
      tft.fillRect(0, 100, 320, 140, ILI9341_BLACK);
      repeticion = 0;
      desafio++;
      resetCancion();
    }
  }
}


//ANDROID

void desafioAndroid() {
  if (androidStart) {
    activarSensores();
    androidStart = false;
    tft.drawBitmap(110, 75, android, 100, 100, ILI9341_GREEN);
    Esplora.writeRGB(0, 255, 0);
  }
}

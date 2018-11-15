#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <Esplora.h>
#include <SPI.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include "notas.h"
#include "melodias.h"

// pins para conectar la pantalla 
#define SCLK 15
#define MOSI 16
#define CS   0
#define DC   7
#define RST  8
#define MISO -1 // No lo usamos

// Estados de la pantalla


Adafruit_ILI9341 tft = Adafruit_ILI9341(CS, DC, MOSI, SCLK, RST, MISO);

void sonar(int[], int[], int);
void dibujar(int);
void dibujarEstado(int);
void dibujarBotones();
void shuffle(int[], int);
void cambiar(int*, int*);
int otroBotonPresionado(int);

int minutoActual;

void setup() {
  Serial.begin(9800);
  tft.begin();
  tft.setRotation(1);
  setTime(12,59,30,5,11,18);
  //Alarm.timerRepeat(10, sonar);
  minutoActual = minute();
  tft.fillScreen(ILI9341_BLACK);
  dibujarHora();
  
  //sonar(starWarsMelodia, starWarsTempo, sizeof(starWarsMelodia) / sizeof(int));
  //sonar(marioMelodia, marioTempo, sizeof(marioMelodia) / sizeof(int));
  //sonar(melodiaUnder, tempoUnder, sizeof(underMario) / sizeof(int));
}


void loop() {
  if (minutoActual != minute()) {
    minutoActual = minute();
    dibujarHora();
  }
  desafioBotones();
  Alarm.delay(3000);
}

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
 // tft.fillRect(60, 0, 100, 50, ILI9341_BLACK);
  tft.println(tiempo);
}

void dibujarBotones(int botonAPresionar) {
  if (botonAPresionar == SWITCH_UP) {
    tft.fillCircle(160, 120, 20, ILI9341_YELLOW);
    tft.fillCircle(120, 160, 20, ILI9341_GREEN);
    tft.fillCircle(200, 160, 20, ILI9341_GREEN);
    tft.fillCircle(160, 200, 20, ILI9341_GREEN);
    return;
  }
  if (botonAPresionar == SWITCH_LEFT) {
    tft.fillCircle(160, 120, 20, ILI9341_GREEN);
    tft.fillCircle(120, 160, 20, ILI9341_YELLOW);
    tft.fillCircle(200, 160, 20, ILI9341_GREEN);
    tft.fillCircle(160, 200, 20, ILI9341_GREEN);
    return;
  }
  if (botonAPresionar == SWITCH_RIGHT) {
    tft.fillCircle(160, 120, 20, ILI9341_GREEN);
    tft.fillCircle(120, 160, 20, ILI9341_GREEN);
    tft.fillCircle(200, 160, 20, ILI9341_YELLOW);
    tft.fillCircle(160, 200, 20, ILI9341_GREEN);
    return;
  }
  if (botonAPresionar == SWITCH_DOWN) {
    tft.fillCircle(160, 120, 20, ILI9341_GREEN);
    tft.fillCircle(120, 160, 20, ILI9341_GREEN);
    tft.fillCircle(200, 160, 20, ILI9341_GREEN);
    tft.fillCircle(160, 200, 20, ILI9341_YELLOW);
  }
}

void limpiarBotones() {
  tft.fillCircle(160, 120, 20, ILI9341_BLACK);
  tft.fillCircle(120, 160, 20, ILI9341_BLACK);
  tft.fillCircle(200, 160, 20, ILI9341_BLACK);
  tft.fillCircle(160, 200, 20, ILI9341_BLACK);
}

void sonar(int melodia[], int tempo[], int tam) {
  for (int i = 0; i < tam; i++) {
    Esplora.tone(melodia[i], tempo[i]);
    Alarm.delay(tempo[i] * 1.2);
  }
}

void desafioBotones() {
  int botones[] = {
    SWITCH_1,
    SWITCH_2,
    SWITCH_3,
    SWITCH_4,
  };
  shuffle(botones, sizeof(botones) / sizeof(int));
  for(int i = 0; i < sizeof(botones) / sizeof(int); i++) {
    dibujarBotones(botones[i]);
    Esplora.writeRed(0);
    while(Esplora.readButton(botones[i]) == HIGH) {   // devuelve HIGH cuando no esta presionado
      if(otroBotonPresionado(botones[i])) {
        Esplora.writeRed(255);
      }
    }
  }
  limpiarBotones();
}

int otroBotonPresionado(int botonPresionado) {
  if (botonPresionado != SWITCH_UP && Esplora.readButton(SWITCH_UP) == LOW) {
    tft.fillCircle(160, 120, 20, ILI9341_RED);
    return 1;
  }
  if (botonPresionado != SWITCH_LEFT && Esplora.readButton(SWITCH_LEFT) == LOW) {
    tft.fillCircle(120, 160, 20, ILI9341_RED);
    return 1;
  }
  if (botonPresionado != SWITCH_RIGHT && Esplora.readButton(SWITCH_RIGHT) == LOW) {
    tft.fillCircle(200, 160, 20, ILI9341_RED);
    return 1;
  }
  if (botonPresionado != SWITCH_DOWN && Esplora.readButton(SWITCH_DOWN) == LOW) {
    tft.fillCircle(160, 200, 20, ILI9341_RED);
    return 1;
  }
  
  return 0;
}

void shuffle(int items[], int tam) {
  for (int i = 0; i < tam; i++) {
    int rando = random(0, tam);
    cambiar(&items[i], &items[rando]);
  }
}

void cambiar(int *n1, int *n2) {
  int aux = *n1;
  *n1 = *n2;
  *n2 = aux;
}

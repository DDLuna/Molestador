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

// Estado desafio
const int BOTONES = 1;
const int MOVERSE = 2;
const int FIN = 3;

// Cantidad de repeteciones de cada desafío
const int REPETICIONES = 3;
int repeticion = 0;

Adafruit_ILI9341 tft = Adafruit_ILI9341(CS, DC, MOSI, SCLK, RST, MISO);

void toggle(void);
void sonar(int[], int[], int);
void dibujarHora();
void dibujarBotones();
void desafioBotones();
void desafioMoverse();
void limpiarBotones();
void shuffle(int[], int);
void cambiar(int*, int*);
bool otroBotonPresionado(int);

int minutoActual;
int sonando = false;
bool activo = false;
int desafio = BOTONES;

void setup() {
  Serial.begin(9800);
  tft.begin();
  tft.setRotation(1);
  setTime(12,59,30,5,11,18);
 // Alarm.timerRepeat(60, dibujarHora);
  //Alarm.timerRepeat(10, sonar);
  minutoActual = minute();
  tft.fillScreen(ILI9341_BLACK);
  dibujarHora();
  Alarm.timerOnce(10, toggle);
}


void loop() {
  if (minutoActual != minute()) {
    minutoActual = minute();
    dibujarHora();
  }
  if (activo) {
    sonar(marioMelodia, marioTempo, sizeof(marioMelodia) / sizeof(int));
    switch(desafio) {
      case BOTONES:
        desafioBotones();
        break;
      case MOVERSE:
        desafioMoverse();
        break;
      case FIN:
        activo = false;
    }
  }
  Alarm.delay(1); // Necesario para que funcione la biblioteca de alarmas.
}

void toggle() {
  activo = true;
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

unsigned long tiempoDesdeNota = 0;
int nota = 0;

void sonar(int melodia[], int tempo[], int tam) {
  if(millis() - tiempoDesdeNota >= 1.3 * tempo[nota]) {
    Esplora.tone(melodia[nota], tempo[nota]);
    tiempoDesdeNota = millis();
    nota++;
    if (nota == tam - 1) {
      nota = 0;
    }
  }
}

int desafioBotonesStart = true;
int botones [] = {
  SWITCH_1,
  SWITCH_2,
  SWITCH_3,
  SWITCH_4,
};
int botonesCounter = 0;

void desafioBotones() {
  if (desafioBotonesStart) {
    shuffle(botones, sizeof(botones) / sizeof(int));
    desafioBotonesStart = false;
    dibujarBotones(botones[botonesCounter]);
  }
  otroBotonPresionado(botones[botonesCounter]);
  if (Esplora.readButton(botones[botonesCounter]) == LOW) {//Devuelve LOW cuando se aprieta
    botonesCounter++;
    Esplora.writeRed(0);
    dibujarBotones(botones[botonesCounter]);

  }
  if (botonesCounter == 4) {
    limpiarBotones();
    botonesCounter = 0;
    desafioBotonesStart = true;
    repeticion++;
    if (repeticion == REPETICIONES) {
      repeticion = 0;
      desafio++;
    }
  }
}

bool otroBotonPresionado(int botonPresionado) {
  if (botonPresionado != SWITCH_UP && Esplora.readButton(SWITCH_UP) == LOW) {
    tft.fillCircle(160, 120, 20, ILI9341_RED);
    return true;
  }
  if (botonPresionado != SWITCH_LEFT && Esplora.readButton(SWITCH_LEFT) == LOW) {
    tft.fillCircle(120, 160, 20, ILI9341_RED);
    return true;
  }
  if (botonPresionado != SWITCH_RIGHT && Esplora.readButton(SWITCH_RIGHT) == LOW) {
    tft.fillCircle(200, 160, 20, ILI9341_RED);
    return true;
  }
  if (botonPresionado != SWITCH_DOWN && Esplora.readButton(SWITCH_DOWN) == LOW) {
    tft.fillCircle(160, 200, 20, ILI9341_RED);
    return true;
  }
  
  return false;
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

int moverseStart = true;
int posJug[2];
int posObj[2];

void desafioMoverse() {
  if (moverseStart) {
    tft.drawRect(0, 100, 320, 140, ILI9341_BLUE);
    posJug[0] = random(1, 320); // X
    posJug[1] = random(101, 240); // Y
    posObj[0] = random(1, 299);
    posObj[1] = random(101, 219);
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
    tft.drawRect(posObj[0], posObj[1], 20, 20, ILI9341_MAGENTA);
    moverseStart = false;
  }
  if (Esplora.readJoystickX() > 200 && posJug[0] > 1) { //izquierda
    tft.drawPixel(posJug[0], posJug[1], ILI9341_BLACK);
    posJug[0]--;
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
  } else if ( Esplora.readJoystickX() < -200 && posJug[0] < 318) { //derecha
    tft.drawPixel(posJug[0], posJug[1], ILI9341_BLACK);
    posJug[0]++;
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
  }
  if(Esplora.readJoystickY() > 200 && posJug[1] < 238) { //abajo
    tft.drawPixel(posJug[0], posJug[1], ILI9341_BLACK);
    posJug[1]++;
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
    
  } else if (Esplora.readJoystickY() < -200 && posJug[1] > 101) { //arriba
    tft.drawPixel(posJug[0], posJug[1], ILI9341_BLACK);
    posJug[1]--;
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
  }
  if (posJug[0] >= posObj[0] && posJug[0] <= posObj[0] + 20 &&
      posJug[1] >= posObj[1] && posJug[1] <= posObj[1] + 20) {
    tft.fillRect(posObj[0], posObj[1], 20, 20, ILI9341_BLACK);
    moverseStart = true;
    repeticion++;
    if (repeticion >= REPETICIONES) {
      tft.fillRect(0, 100, 320, 140, ILI9341_BLACK);
      repeticion = 0;
      desafio++;
    }
  }
}

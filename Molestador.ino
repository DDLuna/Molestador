#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <Esplora.h>
#include <SPI.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <SoftwareSerial.h>
#include "notas.h"
#include "melodias.h"

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

//bitmap lampara
const unsigned char PROGMEM lampara[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x40, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xcc, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 
  0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x06, 
  0x3f, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x10, 0xff, 0xc2, 
  0x00, 0x00, 0x00, 0x00, 0x39, 0xff, 0xe7, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 
  0x00, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x38, 0xff, 0xe7, 0x00, 0x00, 0x00, 0x00, 0x10, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7f, 0x98, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x3f, 0x9c, 0x00, 
  0x00, 0x00, 0x00, 0x04, 0x3f, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Estado desafio
const int BOTONES = 1;
const int MOVERSE = 2;
const int ANDROID = 3;
const int FIN = 4;

// Cantidad de repeteciones de cada desafío
const int REPETICIONES = 1;
int repeticion = 0;

Adafruit_ILI9341 tft = Adafruit_ILI9341(CS, DC, MOSI, SCLK, RST, MISO);
SoftwareSerial bluetooth(RX, TX);

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
void resetCancion();

int minutoActual;
int sonando = false;
bool activo = false;
int desafio = BOTONES;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  tft.begin();
  tft.setRotation(3);
  minutoActual = minute();
  tft.fillScreen(ILI9341_BLACK);
  setHora();
  dibujarHora();
  setAlarma();
}

void loop() {
  if (minutoActual != minute()) {
    minutoActual = minute();
    dibujarHora();
  }
  if (activo) {
    if (luzBaja()) {
      llamarAPeron();
      return;
    }
    switch(desafio) {
      case BOTONES:
        sonar(marioMelodia, marioTempo, sizeof(marioMelodia) / sizeof(int));
        desafioBotones();
        break;
      case MOVERSE:
        sonar(underMelodia, underTempo, sizeof(underMelodia) / sizeof(int));
        desafioMoverse();
        break;
      case ANDROID:
        sonarEnMillis(despacito, despacitoTempo, sizeof(despacito) / sizeof(int));
        //sonar(starWarsMelodia, starWarsTempo, sizeof(starWarsMelodia) / sizeof(int));
        desafioAndroid();
        break;
      case FIN:
        inactivar();
    }
  }
  Alarm.delay(0); // Necesario para que funcione la biblioteca de alarmas.
}

void setAlarma() {
  String horaCompleta = recibirAlarma();
  int hora = horaCompleta.substring(0, 2).toInt();
  int minuto = horaCompleta.substring(3, 5).toInt();
  Alarm.alarmOnce(hora, minuto, 0, toggle);
  String horaPantalla = String(hora) + ':' + minuto;
  tft.setCursor(110, 50);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_CYAN);
  tft.println(horaPantalla);
}

String recibirAlarma() {
  tft.setCursor(0, 40);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Esperando alarma...");
  bool esperandoAlarma = true;
  while (esperandoAlarma) {
    if (bluetooth.available()) {
      tft.setCursor(0, 40);
      tft.setTextColor(ILI9341_BLACK);
      tft.println("Esperando alarma...");
      return bluetooth.readString();
    }
  }
}

void inactivar() {
  activo = false;
  tft.drawBitmap(270, 0, lampara, 50, 50, ILI9341_BLACK);
}

void setHora() {
  long hora = recibirHora();
  if (hora > 0) {
    setTime(hora);
  } else {
    setTime(12,59,30,5,11,2018);
  }
}

long recibirHora() {
  unsigned int counter = 10;
  tft.setCursor(0,0);
  tft.setTextSize(3);
  tft.println("Esperando hora...");
  bool esperandoBT = true;
  while(esperandoBT) {
    if (bluetooth.available()) {
      tft.setCursor(0, 0);
      tft.setTextColor(ILI9341_BLACK);
      tft.println("Esperando hora...");
      return bluetooth.readString().toInt(); // .toInt devuelve long
    }
  }
}

long unsigned notaPeronista = 0;
long unsigned tiempoDesdePeron = 0;

void llamarAPeron() {
  if (millis() - tiempoDesdePeron > 1500 / peronTempo[notaPeronista]) {
    Esplora.tone(peron[notaPeronista], 1000 / peronTempo[notaPeronista]);
    tiempoDesdePeron = millis();
    notaPeronista++;
    notaPeronista %= sizeof(peron) / sizeof(int);
  }
}

bool dibujarLuzVerde = true;

bool luzBaja() {
  int luz = Esplora.readLightSensor();
  if (luz < 340) {
    tft.drawBitmap(270, 0, lampara, 50, 50, ILI9341_RED);
    dibujarLuzVerde = true;
    return true;
  }
  if (luz < 680) {
    tft.drawBitmap(270, 0, lampara, 50, 50, ILI9341_ORANGE);
    dibujarLuzVerde = true;
    return false;
  }
  if (dibujarLuzVerde) {
    tft.drawBitmap(270, 0, lampara, 50, 50, ILI9341_GREEN);
    dibujarLuzVerde = false;
  }
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
  if(millis() - tiempoDesdeNota >= 1500 / tempo[nota]) {
    Esplora.tone(melodia[nota], 1000 / tempo[nota]);
    tiempoDesdeNota = millis();
    nota++;
    nota %= tam;
  }
}

unsigned long tiempoDesdeNotaMillis = 0;
int notaMilli = 0;

void sonarEnMillis(int melodia[], int tempo[], int tam) {
  if (millis() - tiempoDesdeNotaMillis >= 1.5 * tempo[notaMilli]) {
    Esplora.tone(melodia[nota], tempo[nota]);
    tiempoDesdeNotaMillis = millis();
    notaMilli++;
    nota %= tam;
  }
}

void resetCancion() {
  nota = 0;
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
  //otroBotonPresionado(botones[botonesCounter]);
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
      resetCancion();
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
    posObj[0] = random(1, 310);
    posObj[1] = random(101, 230);
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
    tft.drawRect(posObj[0], posObj[1], 10, 10, ILI9341_MAGENTA);
    moverseStart = false;
  }
  if (Esplora.readJoystickX() > 200 && posJug[0] > 1) { //izquierda
    tft.drawPixel(posJug[0], posJug[1], ILI9341_BLACK);
    posJug[0]--;
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
  } else if (Esplora.readJoystickX() < -200 && posJug[0] < 318) { //derecha
    tft.drawPixel(posJug[0], posJug[1], ILI9341_BLACK);
    posJug[0]++;
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
  }
  if (Esplora.readJoystickY() > 200 && posJug[1] < 238) { //abajo
    tft.drawPixel(posJug[0], posJug[1], ILI9341_BLACK);
    posJug[1]++;
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
    
  } else if (Esplora.readJoystickY() < -200 && posJug[1] > 101) { //arriba
    tft.drawPixel(posJug[0], posJug[1], ILI9341_BLACK);
    posJug[1]--;
    tft.drawPixel(posJug[0], posJug[1], ILI9341_WHITE);
  }
  if (posJug[0] >= posObj[0] && posJug[0] <= posObj[0] + 10 &&
      posJug[1] >= posObj[1] && posJug[1] <= posObj[1] + 10) {
    tft.fillRect(posObj[0], posObj[1], 10, 10, ILI9341_BLACK);
    moverseStart = true;
    repeticion++;
    if (repeticion >= REPETICIONES) {
      tft.fillRect(0, 100, 320, 140, ILI9341_BLACK);
      repeticion = 0;
      desafio++;
      resetCancion();
    }
  }
}

bool androidStart = true;

void desafioAndroid() {
  if (androidStart) {
    bluetooth.println("APAGAR");
    androidStart = false;
  }
  if (bluetooth.available()) {
    bluetooth.readString();
    desafio++;
    resetCancion();
    androidStart = true;
  }
}

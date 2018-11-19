
const int marioMelodia[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7,   NOTE_G7,
  NOTE_A7, 0,         NOTE_F7, NOTE_G7,
  0,        NOTE_E7,  0,       NOTE_C7,
  NOTE_D7, NOTE_B6,   0,       0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

const int marioTempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

const int underMelodia[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};

const int underTempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

const int starWarsMelodia[] = {
  NOTE_A4, NOTE_A4, NOTE_A4,
  NOTE_F4, NOTE_C5, NOTE_A4,
  NOTE_F4, NOTE_C5, NOTE_A4,
  
  NOTE_NO,
  
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_F5, NOTE_C5, NOTE_GS4,
  NOTE_F4, NOTE_C5, NOTE_A4,
};

const int starWarsTempo[] = {
  2, 2, 2,
  3, 7, 2,
  3, 7, 2,
  
  2,
  
  2, 2, 2,
  3, 7, 2,
  3, 7, 2,
};

const int peron[] = {
  NOTE_E4, NOTE_C4, NOTE_A4, NOTE_E4,
  NOTE_C4, NOTE_A4, NOTE_E4, NOTE_C4,
  NOTE_E4, NOTE_C4, NOTE_A4, NOTE_E4, NOTE_E4,
  NOTE_D4, NOTE_C4, NOTE_D4, NOTE_B4,
};

const int peronTempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 6, 6,
  12, 12, 12, 12,
};

const int despacito[] = {
  NOTE_D5, NOTE_CS5, NOTE_B4, NOTE_FS4,
  NOTE_NO, NOTE_FS4, NOTE_FS4, NOTE_FS4,
  NOTE_FS4, NOTE_FS4, NOTE_B4, NOTE_B4,
  NOTE_B4, NOTE_B4, NOTE_A4, NOTE_B4,  
  NOTE_NO, NOTE_G4, NOTE_NO, NOTE_G4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4,
  NOTE_CS5, NOTE_D5, NOTE_NO, NOTE_A4,
  NOTE_NO, NOTE_A4, NOTE_A4, NOTE_A4,
  NOTE_A4, NOTE_D5, NOTE_CS5, NOTE_D5,
  NOTE_CS5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_E5, NOTE_E5,
};

const int despacitoTempo[] = {
  640, 640, 320, 160,
  160, 160, 160, 160,
  160, 160, 160, 160,
  320, 160, 160, 320,
  160, 160, 160, 160,
  160, 160, 160, 160,
  160, 160, 320, 160,
  160, 320, 160, 160,
  160, 160, 160, 160,
  160, 160, 160, 160,
  320, 160
};

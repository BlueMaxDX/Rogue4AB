#define WEMAX   8     //9
#define ARMAX   7     //8
#define POMAX   14    //14
#define SCMAX   12    //14
#define RGMAX   11    //14
#define WDMAX   10    //14
#define RMAX    3
#define IMAX    26
#define TMAX    40
#define MMAX    16

#include <Arduboy2.h>
#include "src/fonts/Font5x7.h"

Arduboy2Base arduboy;
Font5x7 font5x7 = Font5x7();

struct Score {        //my first 'structure'
  long int gold;
  byte depth;
};

struct hero {
  byte hx;
  byte hy;
  byte im;
  byte dlv;
  byte st;
  byte stm;
  byte lv;
  byte hconf;
  byte hblnd;
  byte hhall;
  byte hfast;
  byte hslep;
  byte hmdet;
  byte hisee;
  byte hlevi;
  byte hheld;
  char rdex;
  char rstr;
  long int au;
  long int hp;
  long int hpm;
  long int ex;
  long int ht;
  long int hh;
  long int nl;
};

struct item {
  uint8_t ii; //byte ii;
  uint8_t i1; //byte i1;
  int8_t  i2; //char i2;
  int8_t  i3; //char i3;
  uint8_t i4; //byte i4;
};

Score glory[5];
Score tglory;

const char tsym[9] PROGMEM = {'*', ':', ')', '[', '!', '?', '=', '/', ','};
const char tprob[9] PROGMEM = {0, 10, 15, 24, 33, 63, 93, 96, 100};

const char psprob[2][POMAX + 1] PROGMEM = {
  {0, 5, 10, 20, 25, 35, 40, 50, 60, 65, 75, 85, 90, 95, 100},
  {0, 5, 10, 15, 20, 35, 45, 55, 65, 75, 85, 90,100,100, 100}
//  {0, 0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100,100,100, 100}
};

const char mname[26] PROGMEM = {
  'K', 'E', 'B', 'S', 'H', 'I', 'R', 'O',
  'Z', 'L', 'C', 'Q', 'A', 'N', 'Y', 'F',
  'T', 'W', 'P', 'X', 'U', 'M', 'V', 'G',
  'J', 'D'
};

const char mstat[26][7] PROGMEM = {
  //  hp,  r+XdY, dex, exp,item, state
  { 10,  1,  4,  60,   2,   0, 0b10000011},  // 0 K  76543210
  { 11,  1,  3,  65,   2,   0, 0b00000010},  // 1 E         wake
  { 10,  1,  3,  60,   2,   0, 0b00100101},  // 2 B        sleep (mimic is sleep and deep sleep)
  {  8,  1,  3,  50,   2,   0, 0b00000011},  // 3 S       deep sleep
  { 15,  2,  5,  67,   3,   0, 0b00000011},  // 4 H      NC
  { 15,  0,  0,  68,   5,   0, 0b00000100},  // 5 I     special attack(for medusa's grace)
  { 19,  2, 10,  70,  10,   0, 0b00000011},  // 6 R    wanderer
  { 25,  1,  5,  70,   5,  10, 0b00000011},  // 7 O   transparent
  { 21,  1,  7,  69,   8,   0, 0b00000011},  // 8 Z  fast
  { 25,  0,  0,  75,  21, 100, 0b00000100},  // 9 L
  { 32,  5, 19,  85,  15,  10, 0b00000101},  //10 C
  { 30,  3, 15,  78,  20,  20, 0b00000011},  //11 Q
  { 25,  0,  0, 100,  20,   0, 0b00000011},  //12 A
  { 25,  0,  0,  75,  39, 100, 0b00000100},  //13 N
  { 35,  3, 18,  80,  50,  20, 0b00000101},  //14 Y
  { 73,  0,  0,  80,  91,   0, 0b00000001},  //15 F
  { 75,  5, 28,  75, 125,  33, 0b00000011},  //16 T
  { 45,  2, 16,  75,  55,   0, 0b00000101},  //17 W
  { 76,  5, 20,  80, 120,  50, 0b01100101},  //18 P
  { 42,  4, 24,  75, 110,   0, 0b00000110},  //19 X
  { 90,  4, 40,  85, 200,  33, 0b00000011},  //20 U
  { 97,  7, 37,  85, 250,  25, 0b00000011},  //21 M
  { 55,  2, 18,  85,   4,  18, 0b00000011},  //22 V 350
  {115, 10, 50,  85,  20,  10, 0b10000011},  //23 G 2000
  {132,  7, 50, 100,  30,   0, 0b00000101},  //24 J 3000
  {145,  8, 60, 100,  50,  90, 0b00000010}   //25 D 5000
};
const char wstat[WEMAX][2] PROGMEM = {
  { 2, 3 }, { 3, 4 }, { 4, 5 }, { 1, 1 }, { 1, 1 }, { 1, 2 }, { 1, 3 }, { 1, 4 }
};
const char astat[ARMAX] PROGMEM = {
  2, 3, 4, 5, 6, 6, 7
};

byte dungeon[21][8];
byte known[3][8];
byte monst[21][8];
byte thing[21][8];

byte death=0;

byte rank=0;
hero hero={0,0,5,1,16,16,1,0,0,0,0,0,0,0,0,0,0,0,0,16,16,0,0,400,10};

byte ss = 0;      //show status flag
char roomSX[RMAX * 2] = {}, roomSY[RMAX * 2] = {}, roomEX[RMAX * 2] = {}, roomEY[RMAX * 2] = {};
byte hasRoom[RMAX * 2] = {};
byte isDark[RMAX * 2] = {};
byte isBigRoom=0;
byte mx[MMAX] = {}, my[MMAX] = {}, ms[MMAX] = {}, mh[MMAX] = {}, m1[MMAX] = {}, m2[MMAX] = {};

byte ttab[4][POMAX] = {
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}
};

item inv[IMAX]={};
item tng[TMAX]={};
item ftng={};

unsigned int tknow[4] = {0, 0, 0, 0}; //0:potion, 1:scroll, 2:ring, 3:wand

byte adepth=26, wiz=0;//, welc=0;

#include "FlashStringHelper.h"
FlashStringHelper activeMessage = nullptr;

char gmon[10];
char gmes[15];

byte gstate = 0;

ARDUBOY_NO_USB

void setup() {
  byte r;
  arduboy.boot();
  arduboy.setFrameRate(15);
  arduboy.initRandomSeed();
  loadHiScore();
  sortItem();
}

void loop() {
  byte tmp;
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  switch (gstate) {
    case 0:
      title();
      break;
    case 1:
      gameloop();
      break;
    case 2:
      gameover();
      break;
    case 3:
      winner();
      break;
    case 4:
      score();
      break;
    case 5:
      landing();
      break;
  }
  arduboy.display();
}

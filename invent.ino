#include "strings.h"
#include "FlashStringHelper.h"

byte inputWait() {
  byte result = 0;
  do {
    arduboy.pollButtons();
    if (arduboy.justPressed(LEFT_BUTTON)) result = 1;
    if (arduboy.justPressed(UP_BUTTON)) result = 2;
    if (arduboy.justPressed(RIGHT_BUTTON)) result = 3;
    if (arduboy.justPressed(DOWN_BUTTON)) result = 4;
    if (arduboy.justPressed(A_BUTTON)) result = 5;
    if (arduboy.justPressed(B_BUTTON)) result = 6;
    delay(20);
  } while (result == 0);
  setActiveMessage(29);
  return result;
}

void drawInventry(byte st, byte mode) {
  byte ed;
  arduboy.clear();
  locate(0, 0);
  if (mode == 1) {
    font5x7.print(F(" which?"));
  }
  locate(0, 1 - !mode );
  font5x7.print('>');

  if (st + 8 - mode > hero.im) {
    ed = hero.im;
  } else {
    ed = st + 8 - mode;
  }
  for (int i = st; i < ed; i++) {
    locate(1, i - st + 1 - !mode );
/*
    font5x7.print(inv[i].ii);
    font5x7.print(' ');
*/
    char buff;
    if (bitRead(inv[i].i4, 3) == 1) {
      buff = '{';
    } else {
      buff = pgm_read_byte(tsym + inv[i].ii / 16 - 1);
    }
    font5x7.print(buff);
    font5x7.print(' ');

    if (bitRead(inv[i].i4, 7) == 1) {
      font5x7.print(inv[i].i1);
      font5x7.print(' ');
    }
    if (inv[i].ii / 16 < 5 || inv[i].ii / 16 == 9) {
      itmToGitm(inv[i].ii / 16, inv[i].ii % 16, 0);
    } else {
      itmToGitm(inv[i].ii / 16, inv[i].ii % 16, bitRead(tknow[inv[i].ii / 16 - 5], inv[i].ii % 16));
    }
    font5x7.print(activeMessage);
    if (bitRead(inv[i].i4, 5) == 1) {
      if (inv[i].ii / 16 == 3) {       //weapon
        font5x7.print('[');
        font5x7.print((int)inv[i].i2);
        font5x7.print(',');
        font5x7.print((int)inv[i].i3);
        font5x7.print(']');
      } else if (inv[i].ii / 16 == 4 || inv[i].ii / 16 == 8) { //armor or ring
        font5x7.print('[');
        font5x7.print((int)inv[i].i2);
        font5x7.print(']');
      } else if (inv[i].ii / 16 == 7) {
        if (inv[i].i2 != 0) {
          font5x7.print('[');
          font5x7.print((int)inv[i].i2);
          font5x7.print(']');
        }
      }
    }
    if (bitRead(inv[i].i4, 4) == 1) {
      font5x7.print(F(" E"));
    }
    if (bitRead(inv[i].i4, 6) == 1 && bitRead(inv[i].i4, 1) == 1) {
      font5x7.print(F(" C"));
    }
  }
  arduboy.display();
}

byte inventry(byte mode) {
  byte st = 0;
  byte ex = 0;
  do {
    drawInventry(st, mode);
    byte a = inputWait();
    switch(a){
      case 1:
        if (st > 7 - mode) {
          st -= (8 - mode);
        } else {
          st = 0;
        }
        break;
      case 2:
        if (st > 0) st--;
        break;
      case 3:
        if (st < hero.im - 1 - 7 + mode) {
          st += (8 - mode);
        } else {
          st = hero.im - 1;
        }
        break;
      case 4:
        if (st < hero.im - 1 ) st++;
        break;
      case 5:
        if (mode == 0) {
          ex = action(st);
        } else {
          ex = 1;
        }
        break;
      case 6:
        ex = 1;
        break;
    }
  } while (ex == 0);
  return st;
}

byte action(byte st) {
  byte curs = 0;
  locate(1, 1);
  font5x7.print(F("  use   "));
  locate(1, 2);
  font5x7.print(F("  throw "));
  locate(1, 3);
  font5x7.print(F("  drop  "));
  locate(1, 4);
  font5x7.print(F("        "));
  locate(1, curs + 1);
  font5x7.print('>');

  arduboy.display();
  byte ex = 0;
  do {
    byte a = inputWait();
    switch (a) {
      case 2:
        if (curs > 0) {
          locate(1, curs + 1);
          font5x7.print(' ');
          curs--;
          locate(1, curs + 1);
          font5x7.print('>');
          arduboy.display();
        }
        break;
      case 4:
        if (curs < 2) {
          locate(1, curs + 1);
          font5x7.print(' ');
          curs++;
          locate(1, curs + 1);
          font5x7.print('>');
          arduboy.display();
        }
        break;
      case 5:
        if (hero.hslep == 0) {
          if (curs == 0) {
            useItem(st);
          } else if (curs == 1) {
            throwItem(st);
          } else if (curs == 2) {
            dropItem(hero.hx, hero.hy, st);
          } 
          sortItem();
          moveMonst();
          tweatHero();
        } else {
          setActiveMessage(22);
        }
        ex = 1;
        break;
      case 6:
        ex = 1;
        break;
    }
  } while (ex == 0);
  return 1;
}

void rest(){
  gRest = 1;
  while(gRest == 1){
    moveMonst();
    tweatHero();
    if(hero.hp >= hero.hpm || hero.hh < 50) gRest = 0;
  }
}

void showStatus() {
  byte h = 0;
  if (hero.hx < 10) {
    h = 11;
  }
  
  updateScreen();
  
  for (int i = 0; i < 8; i++) {
    locate(h, i);
    font5x7.print(F("          "));
  }
  locate(h, 0);
  font5x7.print(F("Lv:"));
  font5x7.print(hero.dlv);
  locate(h, 1);
  font5x7.print(F("Au:"));
  font5x7.print(hero.au);
  locate(h, 2);
  font5x7.print(F("Hp:"));
  font5x7.print(hero.hp);
  font5x7.print('/');
  font5x7.print(hero.hpm);
  locate(h, 3);
  font5x7.print(F("Hg:"));
  font5x7.print(hero.hh);
  locate(h, 4);
  font5x7.print(F("St:"));
  font5x7.print(hero.st);
  font5x7.print('/');
  font5x7.print(hero.stm);
  locate(h, 5);
  font5x7.print(F("AC:"));
  byte a = equip(4, 1);
  byte ac = 0;
  if (a == 0) {
    ac = 0;
  } else {
    ac = pgm_read_byte(astat + inv[a - 1].ii % 16 ) + inv[a - 1].i2;
  }
  font5x7.print((int)ac);
  locate(h, 6);
  font5x7.print(F("Ex:"));
  font5x7.print(hero.lv);
  font5x7.print('/');
  locate(h + 3, 7);
  font5x7.print(hero.ex);
}

void search() {
  for (int i = hero.hx - 1; i <= hero.hx + 1; i++) {
    for (int j = hero.hy - 1; j <= hero.hy + 1; j++) {
      if(j >= 0 && j <= 7){
        if (dungeon[i][j] >= 31 && dungeon[i][j] <= 106) {
          dungeon[i][j] = dungeon[i][j] + 80;
        } else if (dungeon[i][j] >= 221 && dungeon[i][j] <= 226) {
          dungeon[i][j] = dungeon[i][j] - 200;
        } else if (dungeon[i][j] == 228) {
          dungeon[i][j] = dungeon[i][j] - 220;
        } else if (dungeon[i][j] >= 231 && dungeon[i][j] <= 236) {
          dungeon[i][j] = dungeon[i][j] - 210;
        }
      }
    }
  }
}

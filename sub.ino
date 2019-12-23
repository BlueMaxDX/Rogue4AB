void setKnown(byte x, byte y) {
  bitWrite(known[x / 8][y], x % 8, 1);
}

byte getKnown(byte x, byte y) {
  return bitRead(known[x / 8][y], x % 8);
}

void clearKnown() {

  memset(monst, 0, sizeof(monst));  //thanks @Dreamer3
  memset(thing, 0, sizeof(thing));
  memset(known, 0, sizeof(known));
  memset(tng, 0, sizeof(tng));
  for (int i = 0; i < MMAX; i++) {
    mx[i] = 0;
    my[i] = 0;
    ms[i] = 0;
    mh[i] = 0;
    m1[i] = 0;
    m2[i] = 0;
  }
}

void allKnown() {
  memset(known, 255, sizeof(known));    //thanks @Dreamer3
}

void makeKnown() {
  if (dungeon[hero.hx][hero.hy] >= 1 && dungeon[hero.hx][hero.hy] <= 6 && isDark[dungeon[hero.hx][hero.hy] - 1] == 0 ) {
    for (int i = 0; i < 21; i++) {
      for (int j = 0; j < 8; j++) {
        if (dungeon[i][j] % 10 == dungeon[hero.hx][hero.hy]) {
          setKnown(i, j);
        }
      }
    }
  } else {
    for (int i = 0; i <= 2; i++) {
      for (int j = 0; j <= 2; j++) {
        int tx = hero.hx + i - 1;
        int ty = hero.hy + j - 1;
        if (tx >= 0 && tx <= 20 && ty >= 0 && ty <= 7) {
          setKnown(tx, ty);
        }
      }
    }
  }
}

void clearDungeon() {
  memset(dungeon, 0, sizeof(dungeon));    //thanks Dreamer3
}

void locate(int x, int y) {
  font5x7.setCursor(1 + x * 6, y * 8 - 1);
}

void drawMap() {    //@Pharap's sharp eye
  for (int i = 0; i < 21; i++) {
    for (int j = 0; j < 8; j++) {
      locate(i, j);
      char c = ' ';
      if (hero.hblnd == 0) {
        if (getKnown(i, j) == 1) {
          if (dungeon[i][j] == 0) {
            c = ' '; //arduboy.print(F(" "));
          } else if (((dungeon[i][j] >= 1 && dungeon[i][j] <= 6) || (dungeon[i][j] >= 31 && dungeon[i][j] <= 106))
                     && isDark[dungeon[hero.hx][hero.hy] % 10 - 1] == 0) {
            if ( dungeon[i][j] % 10 == dungeon[hero.hx][hero.hy] % 10 ) {
              c = '.'; //arduboy.print(F("."));
            } else {
              c = ' '; //arduboy.print(F(" "));
            }
          } else if (dungeon[i][j] == 8) {
            c = '#'; //arduboy.print(F("#"));
          } else if (dungeon[i][j] >= 11 && dungeon[i][j] <= 16) {
            c = '%'; //arduboy.print(F("%"));
          } else if (dungeon[i][j] >= 21 && dungeon[i][j] <= 26) {
            c = '+'; //arduboy.print(F("+"));
          } else if (dungeon[i][j] >= 111 && dungeon[i][j] <= 186) {
            c = '^'; //arduboy.print(F("^"));
          } else if (dungeon[i][j] >= 191 && dungeon[i][j] <= 198) {
            c = '?'; //arduboy.print(F("?"));
          } else if (dungeon[i][j] >= 201 && dungeon[i][j] <= 206) {
            c = '-'; //arduboy.print(F("-"));
          } else if (dungeon[i][j] >= 211 && dungeon[i][j] <= 216) {
            c = '|'; //arduboy.print(F("|"));
          } else if (dungeon[i][j] >= 221 && dungeon[i][j] <= 226) {
            c = '-'; //arduboy.print(F("-"));
          } else if (dungeon[i][j] == 228) {
            c = ' '; //arduboy.print(F(" "));
          } else if (dungeon[i][j] >= 231 && dungeon[i][j] <= 236) {
            c = '|'; //arduboy.print(F("|"));
          }
        } else {
          c = ' '; //arduboy.print(F(" "));
        }
      } else {
        c = ' ';
      }
      font5x7.print(c);
    }
  }
}


void drawHero() {     //@Pharap's sharp eye
  if (hero.hblnd == 0) {
    for (int i = 0; i <= 2; i++) {
      for (int j = 0; j <= 2; j++) {
        int tx = hero.hx + i - 1;
        int ty = hero.hy + j - 1;
        if ((dungeon[tx][ty] >= 1 && dungeon[tx][ty] <= 6) ||
            (dungeon[tx][ty] >= 31 && dungeon[tx][ty] <= 106) ) { // (tx >= 0 && tx <= 20 && ty >= 0 && ty <= 7)
          locate(tx, ty);
          font5x7.print('.');
        }
      }
    }
  }
  locate(hero.hx, hero.hy);
  font5x7.print('@');
}

void drawMonst() {

  for (int i = 0; i < MMAX; i++) {
    if (ms[i] / 32 != 0) {
      if (hero.hmdet == 0) {
        if (isDark[dungeon[hero.hx][hero.hy] % 10 - 1] == 0 && dungeon[hero.hx][hero.hy] != 8 &&
            dungeon[hero.hx][hero.hy] % 10 == dungeon[mx[i]][my[i]] % 10) {
          locate(mx[i], my[i]);
          drawMonstSub(i);
        }
        if (abs(hero.hx - mx[i]) <= 1 && abs(hero.hy - my[i]) <= 1) {
          locate(mx[i], my[i]);
          drawMonstSub(i);
        }
      } else {
        locate(mx[i], my[i]);
        drawMonstSub(i);
      }
    }
  }
}

void drawMonstSub(byte i) {
  if (bitRead(m1[i], 6) == 0 || hasRing(8) > 0 || hero.hisee == 1) {
    if (hero.hhall > 0) {
      if (ms[i] / 32 == 3) {
        font5x7.print((char)pgm_read_byte(tsym + random(9)));
      } else {
        font5x7.print((char)pgm_read_byte(mname + random(26) ));
      }
    } else {
      if (ms[i] / 32 == 3) {
        font5x7.print('*');
      } else {
        font5x7.print((char)pgm_read_byte(mname + (ms[i] % 32)));    //mimic is ms[i]%32==19 ms[i]/32==3
      }
    }
  }
}

byte canBless() {
  byte result = 0;
  int dra;
  for (int i = 1; i <= 4; i++) {
    dra = checkHit(i, 5);
    if ( ms[dra / 256 - 1] % 32 == 25) result++;
  }
  return result;
}

void moveMonst() {
  for (int i = 0; i < MMAX; i++) {
    if (ms[i] == 0) {
      if (random(128) == 0) {
        generateMon(i);
      }
    } else {
      byte fly = bitRead(m1[i], 7) + 1;
      if ( bitRead(m1[i], 2) == 0 || (bitRead(m1[i], 2) == 1 && hero.ht % 2 == 0)) {
        for (int tt = 0; tt < fly; tt++) {
          int r;  //= random(4);
          if (ms[i] / 32 >= 2 && ms[i] / 32 <= 6) {
            r = 5;
          } else if (ms[i] % 32 == 21 && dungeon[hero.hx][hero.hy] != 8 &&
                     dungeon[mx[i]][my[i]] % 10 == dungeon[hero.hx][hero.hy] % 10 && bitRead(m1[i], 3) == 1) {
            flashHero('?');
            setActiveMessage(18);
            hero.hconf = 10;
            bitWrite(m1[i], 3, 0);
          } else if (ms[i] % 32 == 25 && canBless() > 0 && random(2) == 0) {
            flashHero('*');
            setActiveMessage(19);
            byte dmg = random(22 - hero.lv);
            charon(dmg, 3);
          } else {

            char d = 1;
            if (ms[i] / 32 == 7) d = -1;

            if ( mx[i] > hero.hx && monst[mx[i] - d][my[i]] == 0
                 && dungeon[mx[i] - d][my[i]] >= 1 && dungeon[mx[i] - d][my[i]] <= 190) {
              r = 2 - d;
            } else if ( my[i] > hero.hy && monst[mx[i]][my[i] - d] == 0
                        && dungeon[mx[i]][my[i] - d] >= 1 && dungeon[mx[i]][my[i] - d] <= 190) {
              r = 3 - d;
            } else if ( mx[i] < hero.hx && monst[mx[i] + d][my[i]] == 0
                        && dungeon[mx[i] + d][my[i]] >= 1 && dungeon[mx[i] + d][my[i]] <= 190) {
              r = 2 + d;
            } else if ( my[i] < hero.hy && monst[mx[i]][my[i] + d] == 0
                        && dungeon[mx[i]][my[i] + d] >= 1 && dungeon[mx[i]][my[i] + d] <= 190) {
              r = 3 + d;
            } else {
              r = random(4) + 1;
            }
          }
          if (bitRead(m1[i], 5) == 1 && random(3) == 0) r = random(4) + 1;

          char dx = (r - 2) * (r % 2);              //d=1(right), 2(up), 3(left), 4(down)
          char dy = (r - 3) * ((r - 1) % 2);

          switch (r) {
            case 1:
            case 2:
            case 3:
            case 4:
              if (mx[i] + dx == hero.hx && my[i] + dy == hero.hy) {
                if (tt == 0) hitHero(ms[i] % 32, i);
              } else if (ms[i] % 32 != 15 &&
                         mx[i] + dx >= 0 && mx[i] + dx <= 20 &&
                         my[i] + dy >= 0 && my[i] + dy <= 7 &&
                         monst[mx[i] + dx][my[i] + dy] == 0 &&
                         dungeon[mx[i] + dx][my[i] + dy] >= 1 && dungeon[mx[i] + dx][my[i] + dy ] <= 190) {
                monst[mx[i]][my[i]] = 0;
                mx[i] = mx[i] + dx;
                my[i] = my[i] + dy;
                monst[mx[i]][my[i]] = i + 1;
              }
              break;
            case 5:
              break;
          }
        }
      }
    }
  }
}

void placeMonst() {
  byte stt, mon;
  for (int i = 0; i <= 8 + isBigRoom * 7; i++) {
    if ( random(3) == 0) {
      int h = 0;
      if (isBigRoom == 0) h = random(RMAX) * 2;
      if (hasRoom[h] == 0) h++;
      mx[i] = roomSX[h] + random(roomEX[h] - roomSX[h] + 1);
      my[i] = roomSY[h] + random(roomEY[h] - roomSY[h] + 1);
      if (mx[i] != hero.hx && my[i] != hero.hy && monst[mx[i]][my[i]] == 0) {
        placeMonXY(i, mx[i], my[i]);
      } else {
        ms[i] = 0;
      }
    } else {
      ms[i] = 0;
    }
  }
}

void generateMon(byte m) {
  byte stt, mon;
  int hr = ((dungeon[hero.hx][hero.hy] - 1) % 10 - ((dungeon[hero.hx][hero.hy] - 1) % 10) % 2) / 2;
  int h = 0;
  if (isBigRoom == 0) h = ((hr + 1) % RMAX) * 2;
  if (hasRoom[h] == 0) h++;
  mx[m] = roomSX[h] + random(roomEX[h] - roomSX[h] + 1);
  my[m] = roomSY[h] + random(roomEY[h] - roomSY[h] + 1);
  if (monst[mx[m]][my[m]] == 0 ) {
    placeMonXY(m, mx[m], my[m]);
  }
}

void placeMonXY( byte i, byte x, byte y) {
  byte mon = randMonst(hero.dlv);
  byte stt = initState(mon);
  ms[i] = stt * 32 + mon;
  mh[i] = pgm_read_byte(mstat[mon]);
  m1[i] = pgm_read_byte(mstat[mon] + 6) / 16 * 16;
  bitWrite(m1[i], 3, 1);
  monst[x][y] = i + 1;
}

void showMsg() {
  if (hero.hy >= 4) {
    locate(0, 0);
  } else {
    locate(0, 7);
  }
  font5x7.print(activeMessage);
}

void placeThing() {
  for ( int i = 0; i < RMAX * 2; i++) {
    if (hasRoom[i] == 1) {
      for (int j = 0; j < 3 + isBigRoom * 7; j++) {
        byte ix = random(roomSX[i], roomEX[i] + 1);
        byte iy = random(roomSY[i], roomEY[i] + 1);
        placeThingXY(ix, iy, 0);
      }
    }
  }

  if ( hero.dlv % 3 == 0) {
    for ( int i = 0; i < RMAX * 2; i++) {
      if (hasRoom[i] == 1) {
        byte ix = random(roomSX[i], roomEX[i] + 1);
        byte iy = random(roomSY[i], roomEY[i] + 1);
        placeThingXY(ix, iy, 2);
      }
    }
  }

  if (hero.dlv >= adepth && inv[hero.im - 1].ii != 144) {
    byte a = 0;
    if (isBigRoom == 0) a = random(RMAX) * 2;
    if (hasRoom[a] == 0) a++;
    byte ax = roomSX[a] + random(roomEX[a] - roomSX[a] + 1);
    byte ay = roomSY[a] + random(roomEY[a] - roomSY[a] + 1);
    byte t = freeSlot();
    thing[ax][ay] = t + 1;
    tng[t].ii = 144;
    tng[t].i1 = 1;
    tng[t].i2 = 0;
    tng[t].i3 = 0;
    tng[t].i4 = 0b00000000;
  }
}

void placeThingXY(byte ix, byte iy, byte r) {
  byte r1, r2, r3, vari, kind;
  if (thing[ix][iy] == 0) {
    byte t = freeSlot();
    thing[ix][iy] = t + 1;
    tng[t].i1 = 1;
    tng[t].i2 = 0;
    tng[t].i3 = 0;
    tng[t].i4 = 0;
    if (r == 0) {
      kind = randThing();
    } else {
      kind = r;
    }
    switch (kind) {
      case 1: //gold
        tng[t].ii = 16;
        tng[t].i1 = random(100) + hero.dlv;
        break;
      case 2: //food
        if (random(5) == 0) {
          tng[t].ii = 16 * 2 + 1;
        } else {
          tng[t].ii = 16 * 2 + 0;
        }
        tng[t].i1 = 1 + random(2);
        bitWrite(tng[t].i4, 7, 1);
        break;
      case 3: //weapon
        vari = random(WEMAX);
        tng[t].ii = 48 + vari;
        if (vari < 4) {
          tng[t].i1 = 1;
        } else {
          tng[t].i1 = 3 + random(13);
          bitWrite(tng[t].i4, 7, 1);
        }
        r1 = random(6);
        if (r1 == 0) {
          r2 = random(3) + 1;
          r3 = random(r2 + 1);
          tng[t].i2 = r3;
          tng[t].i3 = r2 - r3;
        } else if (r1 == 1) {
          r2 = random(3) + 1;
          r3 = random(r2 + 1);
          tng[t].i2 = r3 * (-1);
          tng[t].i3 = (r2 - r3) * (-1);
          bitWrite(tng[t].i4, 6, 1);
        }
        break;
      case 4: //armor
        vari = random(ARMAX);
        tng[t].ii = 64 + vari;
        tng[t].i1 = 1;
        r1 = random(6);
        if (r1 == 0) {
          tng[t].i2 = random(3) + 1;
        } else if (r1 == 1) {
          tng[t].i2 = (random(3) + 1) * (-1);
          bitWrite(tng[t].i4, 6, 1);
        }
        if (vari == 0) {
          bitWrite(tng[t].i4, 3, 1);
        }
        break;
      case 5: //potion
        vari = randPS(0);
        tng[t].ii = 80 + vari;
        tng[t].i1 = 1 ;//+ random(2);
        bitWrite(tng[t].i4, 7, 1);
        break;
      case 6: //scroll
        vari = randPS(1);
        tng[t].ii = 96 + vari;
        tng[t].i1 = 1 ;//+ random(2);
        bitWrite(tng[t].i4, 7, 1);
        break;
      case 7: //ring
        vari = random(RGMAX);
        tng[t].ii = 112 + vari;
        tng[t].i1 = 1;
        switch ( ttab[2][vari] ) {
          case 1:
            bitWrite(tng[t].i4, 6, 1);
            break;
          case 4:
          case 6:
            r1 = random(2);
            if (r1 == 0) {
              tng[t].i2 = random(2) + 1;
            } else {
              tng[t].i2 = (random(2) + 1) * (-1);
              bitWrite(tng[t].i4, 6, 1);
            }
            break;
          case 7:
            if (random(2) == 0) {
              bitWrite(tng[t].i4, 6, 1);
            }
            break;
        }
        break;
      case 8: //wand
        vari = random(WDMAX);
        tng[t].ii = 128 + vari;
        tng[t].i1 = 1;
        tng[t].i2 = 3 + random(4);
        break;
    }
  }
}

void drawThing() {
  for (int i = 0; i < 21; i++) {
    for (int j = 0; j < 8; j++) {
      if (getKnown(i, j) == 1 && thing[i][j] != 0 ) {
        locate(i, j);
        byte type = tng[thing[i][j] - 1].ii / 16;
        if (hero.hhall > 0) type = random(8) + 1;
        font5x7.print((char)pgm_read_byte(tsym + type - 1));
      }
    }
  }
  locate(hero.hx, hero.hy);
  font5x7.print('@');
}

void checkThing(byte x, byte y) {
  if (thing[x][y] == 0) {
    setActiveMessage(29);
  } else {
    uint8_t type = tng[thing[x][y] - 1].ii / 16;
    uint8_t r = tng[thing[x][y] - 1].ii % 16;
    uint8_t k;

    switch (type) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 9:
        k = 0;
        break;
      case 5:
      case 6:
      case 7:
      case 8:
        k = bitRead(tknow[type - 5], r);
        break;
    }
    itmToGitm(type, r, k);
    byte done = 0;
    if (tng[thing[x][y] - 1].ii == 16) {
      hero.au = hero.au + tng[thing[x][y] - 1].i1;
      deleteThing(thing[x][y] - 1);
      thing[x][y] = 0;
      done = 1;
    } else {
      if (bitRead(tng[thing[x][y] - 1].i4, 7) == 1 ) {
        for (int i = 0; i < hero.im; i++) {
          if (inv[i].ii == tng[thing[x][y] - 1].ii &&
              inv[i].i2 == tng[thing[x][y] - 1].i2 &&
              inv[i].i3 == tng[thing[x][y] - 1].i3 &&
              bitRead(inv[i].i4, 6) == bitRead(tng[thing[x][y] - 1].i4, 6)) {
            inv[i].i1 = inv[i].i1 + tng[thing[x][y] - 1].i1;
            deleteThing(thing[x][y] - 1);
            thing[x][y] = 0;
            done = 1;
          }
        }
      }
      if (done == 0) {
        if (hero.im < IMAX) {
          inv[hero.im] = tng[thing[x][y] - 1];
          deleteThing(thing[x][y] - 1);
          thing[x][y] = 0;
          hero.im++;
          sortItem();
        }
      }
    }
  }
}

void deleteThing(byte i) {
  tng[i].ii = 0;
  tng[i].i1 = 0;
  tng[i].i2 = 0;
  tng[i].i3 = 0;
  tng[i].i4 = 0;
}

byte initState(byte mon) {
  byte result;
  byte state = pgm_read_byte(mstat[mon] + 6) % 16;
  switch (state) {
    case 1:
      result = 1;
      break;
    case 2:
      result = 2;
      break;
    case 3:
      if (random(2) == 0) {
        result = 2;
      } else {
        result = 1;
      }
      break;
    case 4:
      result = 4;
      break;
    case 5:
      if (random(2) == 0) {
        result = 4;
      } else {
        result = 1;
      }
      break;
    case 6:       //for mimic
      result = 3;
  }
  return result;
}

void tweatHero() {
  hero.ht++;
  if (hero.ex >= hero.nl && hero.lv < 21) {   //level up
    setActiveMessage(6);
    hero.lv++;
    hero.nl = hero.nl * 2;
    byte r2 = random(8) + 3;
    hero.hp = hero.hp + r2;
    hero.hpm = hero.hpm + r2;
  }
  if ( hero.hp < hero.hpm && hero.ht % ((22 - hero.lv) / 3 + 1) == 0) {
    hero.hp = hero.hp + 1 + hasRing(2);
  }
  if (hero.hconf > 0) hero.hconf--;
  if (hero.hblnd > 0) hero.hblnd--;
  if (hero.hhall > 0) hero.hhall--;
  if (hero.hfast > 0) hero.hfast--;
  if (hero.hslep > 0) hero.hslep--;
  if (hero.hlevi > 0) hero.hlevi--;

  if ( hasRing(1) > 0 && random(12) == 0) teleportHero();

  hero.hh = hero.hh - 1 + hasRing(3);
  if (equip(7, 1) != 0 && hero.ht % 2 == 0) { //ring right
    hero.hh--;
  }
  if (equip(7, 2) != 0 && hero.ht % 2 == 1) { //ring left
    hero.hh--;
  }

  if ( hero.hh < 60 || hero.hp <= hero.hpm / 4) {
    setActiveMessage(30);
  }

  if (hero.hh <= 0) {  //gashi
    death = 0;
    gstate = 2;
  }
  for (int i = 0; i < hasRing(10) * 2; i++) {
    search();
  }
}

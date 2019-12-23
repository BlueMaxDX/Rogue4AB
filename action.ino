void eat(byte r) {  
  byte bonus = 0;  
  if (r == 0 && random(3) == 0) {
    setActiveMessage(7);
    hero.ex = hero.ex + 2;
  } else {
    setActiveMessage(8);
    bonus=50;
  }
  hero.hh = hero.hh / 3 + 300 + bonus + random(50); //    hh = hh / 3 + 900 + random(200);
}

void wield(byte r) {
  if (bitRead(inv[r].i4, 4) == 1) {
    if (bitRead(inv[r].i4, 6) == 1) {
      setActiveMessage(9);            // cursed
    } else {
      bitWrite(inv[r].i4, 4, 0);
      setActiveMessage(10);           //remove
      if (inv[r].ii / 16 == 7) ringPut(r, -1);
    }
  } else {
    switch (inv[r].ii / 16) {
      case 3:
      case 4:
        if (equip(inv[r].ii / 16, 1) != 0) {
          setActiveMessage(12);       //already wield
        } else {
          bitWrite(inv[r].i4, 1, 1);
          bitWrite(inv[r].i4, 4, 1);
          setActiveMessage(11);       //ready go
        }
        break;
      case 7:
        if (equip(7, 1)*equip(7, 2) != 0) {
          setActiveMessage(12);
        } else {
          bitWrite(inv[r].i4, 1, 1);
          bitWrite(inv[r].i4, 4, 1);
          setActiveMessage(11);       //ready go
          ringPut(r, 1);
        }
        break;
    }
  }
}

void ringPut(byte r, char i) {
  switch (ttab[2][inv[r].ii % 16]) {
    case 4:
      hero.rstr = hero.rstr + inv[r].i2 * i;
      break;
    case 6:
      hero.rdex = hero.rdex + inv[r].i2 * i;
      break;
  }
}

void drink(byte r) {
  switch (ttab[0][r]) {
    case 0:       //power
      if (hero.st == hero.stm) {      //@Pharap's sharp eye
        hero.st++;
        hero.stm++;
      } else {
        hero.st++;
      }
      break;
    case 1:       //restore
      hero.st = hero.stm;
      break;
    case 2:       //heal
      if (hero.hp == hero.hpm) {
        hero.hp++;
        hero.hpm++;
      } else {
        hero.hp = (hero.hpm + hero.hp) / 2;
      }
      hero.hblnd = 0;
      hero.hconf /= 2;
      hero.hhall /= 2;
      break;
    case 3:       //e.heal
      if (hero.hp == hero.hpm) {
        hero.hp = hero.hp + 2;
        hero.hpm = hero.hpm + 2;
      } else {
        hero.hp = hero.hpm;
      }
      hero.hblnd = 0;
      hero.hconf = 0;
      hero.hhall = 0;
      break;
    case 4:       //poison
      if (hero.st > 3) hero.st = hero.st - random(3) - 1;
      hero.hhall = 0;
      break;
    case 5:       //level
      hero.ex = hero.nl;
      break;
    case 6:       //blind
      hero.hblnd = 50;
      updateScreen();
      break;
    case 7:       //hallu
      hero.hhall = 50;
      break;
    case 8:       //m.dtect
      hero.hmdet = 1;
      break;
    case 9:       //i.dtect
      for (int x = 0; x < 21; x++) {
        for (int y = 0; y < 8; y++) {
          if (thing[x][y] != 0) {
            setKnown(x, y);
          }
        }
      }
      break;
    case 10:      //confuse
      hero.hconf = 10;
      break;
    case 11:      //levtate
      hero.hheld = 0;
      hero.hlevi = 30;
      break;
    case 12:      //speed
      hero.hfast = 20;
      break;
    case 13:      //look
      hero.hisee = 1;
      break;
  }
}

void readScroll(byte r) {
  byte ex = 0, i = 0, st, dx, dy;
  int fp;
  switch (ttab[1][r]) {
    case 0:       //protect
      st = equip(4, 1);
      if (st != 0) bitWrite(inv[st - 1].i4, 3, 1);
      break;
    case 1:       //hold
      for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
          st = monst[hero.hx + x][hero.hy + y];
          if (st != 0 ) ms[st - 1] = 4 * 32 + ms[st - 1] % 32;
        }
      }
      break;
    case 2:       //weapon
      i = equip(3, 1);
      if (i != 0) {
        //        bitWrite(he, 3, 0);
        bitWrite(inv[i - 1].i4, 6, 0);
        if (random(2) == 0) {
          inv[i - 1].i2++;
          //          hdex++;
        } else {
          inv[i - 1].i3++;
          //          hdmg++;
        }
      }
      break;
    case 3:       //armor
      i = equip(4, 1);
      if (i != 0) {
        //        bitWrite(he, 2, 0);
        bitWrite(inv[i - 1].i4, 6, 0);
        inv[i - 1].i2++;
        //        ac++;
      }
      break;
    case 4:       //identify
      st = inventry(1);
      bitWrite(inv[st].i4, 1, 1);   //add known2
      bitWrite(inv[st].i4, 5, 1);   //add known
      if (inv[st].ii / 16 >= 5) bitWrite(tknow[inv[st].ii / 16 - 5], inv[st].ii % 16, 1);
      if (inv[st].ii / 16 < 5 || inv[st].ii / 16 == 9) {
        itmToGitm(inv[st].ii / 16, inv[st].ii % 16, 0);
      } else {
        itmToGitm(inv[st].ii / 16, inv[st].ii % 16, 1);
      }
      //      itmToGitm(inv[st].ii / 16, inv[st].ii % 16, 1);
//      addBuf(gitm);
      break;
    case 5:       //telport
      teleportHero();
      break;
    case 6:       //sleep
      hero.hslep = 5;
      break;
    case 7:       //scare
      break;
    case 8:       //bless
      for (int i = 0; i < hero.im; i++) {
        bitWrite(inv[i].i4, 6, 0);
      }
      break;
    case 9:       //create
      for (int i = 0; i < MMAX; i++) {
        if (ms[i] == 0) {
          st = i + 1;
        }
      }
      fp = findPlace(hero.hx, hero.hy, 1);
      if (st != 0 && fp != 0) {
        dx = (fp - 1) % 3 - 1;
        dy = (fp - 1) / 3 - 1;
        mx[st - 1] = hero.hx + dx;
        my[st - 1] = hero.hy + dy;
        placeMonXY( st - 1 , hero.hx + dx, hero.hy + dy );
        drawMonst();
      }
      break;
    case 10:      //anger
      for (int i = 0; i < 16; i++) {
        if (ms[i] != 0) {
          ms[i] = 32 + ms[i] % 32;
        }
      }
      break;
    case 11:      //map
      for (int x = 0; x < 21; x++) {
        for (int y = 0; y < 8; y++) {
          if (dungeon[x][y] >=31 && dungeon[x][y] <= 106) dungeon[x][y] += 80;
          if (dungeon[x][y] != 0) {
            setKnown(x, y);
          }
        }
      }
      break;
  }
}

void zap(byte vari) {
  byte dir = askDir();
  int dest = checkHit(dir, 20);
  byte mon = dest / 256;
  char destx = (dest % 256) % 21;
  char desty = (dest % 256) / 21;
  if (mon != 0) hitWand( vari, mon, destx, desty);
}

byte equip(byte type, byte n) {     //type=3(weapon),4(armor),7(ring), n=1 or 2(for ring)...rtab[equip(7,2)]
  byte result = 0;
  for (int i = 0; i < IMAX; i++) {
    if (inv[i].ii / 16 == type) {
      if (bitRead(inv[i].i4, 4) == 1) {
        n--;
        if (n == 0) {
          result = i + 1;
        }
      }
    }
  }
  return result;      //0=nothing, num=pack+1
}

byte hasRing(byte vari) {
  byte result = 0;
  for (byte i = 0; i < 2; i++) {
    if (equip(7, i + 1) != 0) {
      if (ttab[2][inv[(equip(7, i + 1) - 1)].ii % 16]  == vari) {
        result++;
      }
    }
  }
  return result;
}

int checkHit(byte dir, byte str) {
  char dx = (dir - 2) * (dir % 2);
  char dy = (dir - 3) * ((dir - 1) % 2);
  byte ex = 0, i = 0;
  byte mon = 0;
  byte x = hero.hx, y = hero.hy;
  do {
    if (monst[x + dx][y + dy] != 0) {
      mon = monst[x + dx][y + dy];
      x = x + dx;
      y = y + dy;
      ex = 1;
    } else if (dungeon[x + dx][y + dy] == 0 || dungeon[x + dx][y + dy] >= 200) {
      ex = 1;
    } else {
      x = x + dx;
      y = y + dy;
      i++;
      if (i == str) ex = 1;
    }
  } while (ex == 0);
  return mon * 256 + y * 21 + x;
}

byte findPlace(byte x, byte y, byte tm) {   //tm=0:thing, 1:monst
  byte result = 0;
  byte ex = 0, i = 0, r = random(8);
  monst[hero.hx][hero.hy] = 255;
  do {
    char st = (i + r) % 9;
    char dx = st % 3 - 1;
    char dy = st / 3 - 1;
    if (dungeon[x + dx][y + dy] >= 1 && dungeon[x + dx][y + dy] <= 200
        && thing[x + dx][y + dy] * !tm == 0 && monst[x + dx][y + dy]*tm == 0) {
      result = st + 1;
      ex = 1;
    } else {
      i++;
      if (i == 9) {
        ex = 1;
      }
    }
  } while (ex == 0);
  monst[hero.hx][hero.hy] = 0;
  return result;
}

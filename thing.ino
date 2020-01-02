byte randThing() {
  byte done = 0;
  byte count = 0;
  byte r = random(100);
  while (done == 0) {
    if (r >= pgm_read_byte(tprob + count) && r <= pgm_read_byte(tprob + count + 1) - 1) {
      done = 1;
    } else {
      count++;
    }
  }
  return count + 1;
}

byte randPS(byte i) {     //i=0,potion 1,scroll
  byte done = 0;
  byte count = 0;
  byte r = random(100);
  while (done == 0) {
    if (r >= pgm_read_byte(psprob[i] + ttab[i][count]) && r <= pgm_read_byte(psprob[i] + ttab[i][count] + 1) - 1) {
      done = 1;
    } else {
      count++;
    }
  }
  return count;
}

byte freeSlot() {
  for (uint8_t index = 0; index < TMAX; index++) {
    if(tng[index].ii == 0) {
      return index;
    }
  }
  return TMAX;
}

byte sortItem() {
  byte count = 0;
  for (int i = 0; i < IMAX; i++) {
    if (inv[i].ii == 0) {
      inv[i].ii = 255;
      count++;
    }
  }
  for (int i = 0; i < IMAX; ++i) {
    for (int j = i + 1; j < IMAX; ++j) {
      if (inv[i].ii > inv[j].ii) {
        item titem = inv[i];
        inv[i] = inv[j];
        inv[j] = titem;
      }
    }
  }
  for (int i = 0; i < IMAX; i++) {
    if (inv[i].ii == 255) {
      inv[i].ii = 0;
    }
  }
  return IMAX - count;
}

byte findItem(byte t, byte type) {  // type, potion=0, scroll=1, ring=2, wand=3
  for (byte i = 0; i < POMAX; i++) {
    if (ttab[type][i] == t) return i;
  }
}

void dropItem(byte x, byte y, byte st) {
  if (bitRead(inv[st].i4, 4) == 0) {
    if (inv[st].ii == 16 * 6 + findItem(7,1) ) { //&& bitRead(sknow,7)==1){
      if(dungeon[x][y] >= 11 && dungeon[x][y] <=16){
        setActiveMessage(1);
      } else {
        inv[st].i1--;
        if (inv[st].i1 == 0) deleteItem(st);
        dungeon[x][y] = dungeon[x][y] % 10 + 190;
      }
    } else if (thing[x][y] != 0) {
      setActiveMessage(3);
    } else {
      byte t = freeSlot();
      if (t == TMAX) {
        setActiveMessage(4);
      } else {
        thing[x][y] = t + 1;
        tng[t] = inv[st];
        
        deleteItem(st);
      }
    }
  } else {
    setActiveMessage(12);
  }
}

void useItem(byte st) {
  byte kind = inv[st].ii / 16;
  byte vari = inv[st].ii % 16;
  if (kind == 2) {            //food
    inv[st].i1--;
    if (inv[st].i1 == 0) {
      deleteItem(st);
    }
    eat(vari);
  } else if (kind == 3) {     //weapon
    wield(st);
  } else if (kind == 4) {     //armor
    wield(st);
  } else if (kind == 5) {     //potion
    bitWrite(tknow[0], vari, 1);
    inv[st].i1--;
    if (inv[st].i1 == 0) {
      deleteItem(st);
    }
    drink(vari);
    itmToGitm(5, vari, 1);
  } else if (kind == 6) {     //scroll
    if (hero.hblnd == 0) {
      bitWrite(tknow[1], vari, 1);
      inv[st].i1--;
      if (inv[st].i1 == 0) {
        deleteItem(st);
      }
      readScroll(vari);
      if (ttab[1][vari] != 4) {
        itmToGitm(6, vari, 1);
      }
    } else {
      setActiveMessage(1);
    }
  } else if (kind == 7) {     //ring
    bitWrite(tknow[2], vari, 1);
    wield(st);
  } else if (kind == 8) {     //wand
    bitWrite(tknow[3], vari, 1);
    if (inv[st].i2 > 0) {
      inv[st].i2--;
    } else {
      vari = findItem(9,3); //ttab[3][9]; //9;
    }
    zap(vari);
  }
}

byte randMonst(byte d) {
  char fm = d - 6;
  char to = d + 3;
  if (fm < 0) {
    fm = 0;
  } else if (fm > 21) {
    fm = 21;
  }
  if (to > 27) {
    to = 26;
  }
  return random(fm, to);
}

void wakeUp() {
  for (int i = 0; i < 16; i++) {
    if (ms[i] / 32 == 2) {
      if ((abs(mx[i] - hero.hx) + abs(my[i] - hero.hy)) <= 2) {
        if (random(10) < 4 / (hasRing(0) + 1)) {
          ms[i] = 32 + ms[i] % 32;
        }
      }
    }
  }
}

byte strToDmg(byte str) {
  byte result;
  if (str >= 1 && str <= 5) {
    result = str - 5;
  } else if (str >= 6 && str <= 14) {
    result = 1;
  } else if (str >= 15 && str <= 17) {
    result = 3;
  } else if (str == 18) {
    result = 4;
  } else if (str >= 19 && str <= 20) {
    result = 5;
  } else if (str == 21) {
    result = 6;
  } else if (str >= 22 && str <= 30) {
    result = 7;
  } else if (str >= 31) {
    result = 8;
  }
  return result;
}

void deleteItem(byte i) {
  inv[i].ii = 0;
  inv[i].i1 = 0;
  inv[i].i2 = 0;
  inv[i].i3 = 0;
  inv[i].i4 = 0;
  hero.im--;
  sortItem();
}

void throwItem(byte i) {    //i=pack num 0 to 19
  char hdex = 0, hdmg = 0, dmg = 0;
  int dest = 0;
  if (bitRead(inv[i].i4, 4) == 0 || (inv[i].ii / 16 == 3 && inv[i].ii % 16 >= 4)) {
    //    if (bitRead(inv[i].i4, 4) == 0 && bitRead(inv[i].i4, 6) == 0) {
    if ( bitRead(inv[i].i4, 6) == 0 || (bitRead(inv[i].i4, 6) == 1 && inv[i].ii / 16 != 3)) {
      byte dir = askDir();
      if ( equip(3, 1) != 0 && inv[equip(3, 1) - 1].ii % 16 == 3) {
        dest = checkHit(dir, 20);
      } else {
        dest = checkHit(dir, hero.st / 2);
      }
      ftng = inv[i];
      ftng.i1 = 1;
      
      inv[i].i1--;
      if (inv[i].i1 == 0) {
        deleteItem(i);
      }

      byte mon = dest / 256;
      char destx = (dest % 256) % 21;
      char desty = (dest % 256) / 21;

      if (mon == 0) {
        dropThrown(destx, desty, ftng.ii, ftng.i1, ftng.i2, ftng.i3, ftng.i4);
      } else {  // check hit
        if ( ftng.ii / 16 == 3 ) {
          hdex = pgm_read_byte(wstat[ftng.ii % 16]) + ftng.i2;
        } else {
          hdex = 1;
        }
        //        Serial.println((int)hdex);
        byte rr = random(100);
        byte prob = 40 + hdex * 3 + hero.lv * 2;
        if ( inv[equip(3, 1) - 1].ii % 16 == ftng.ii % 16 || (ftng.ii == 3 * 16 + 5 && inv[equip(3, 1) - 1].ii % 16 == 3)) {
          prob = prob * 4 / 3;
        }
        if (rr < prob) {        //hit
          flashMonst(mon - 1,'/');
          if ( ftng.ii / 16 == 3) {
            hdmg = pgm_read_byte(wstat[ftng.ii % 16] + 1) + ftng.i3;
          } else {
            hdmg = 0;
          }
          if (ftng.ii / 16 == 5) {
            hitPotion(ftng.ii % 16, mon);
          } else if (ftng.ii / 16 == 8 && random(4) != 0 ) {
            hitWand(ftng.ii % 16, mon, destx, desty);
          } else {
            if ( ftng.ii / 16 == 3) {
              dmg = (random(hdex, hdex * hdmg) * 2 + strToDmg(hero.st + hero.rstr) * 2 + hero.rdex + hero.lv + 1) / 2;
            } else {
              dmg = ( -2 + strToDmg(hero.st + hero.rstr) * 2 + hero.rdex + hero.lv + 1) / 2;
            }
            if ( inv[equip(3, 1) - 1].ii % 16 == ftng.ii % 16 || (ftng.ii == 3 * 16 + 5 && inv[equip(3, 1) - 1].ii % 16 == 3)) {
              dmg = dmg * 4 / 3;
            }
            checkMonst(mon - 1, ms[mon - 1] / 32, dmg, destx, desty);
          }
        } else {                //miss
          dropThrown(destx, desty, ftng.ii, ftng.i1, ftng.i2, ftng.i3, ftng.i4);
        }
      }
    } else {
      bitWrite(inv[i].i4, 1, 1);            // I notice it is cursed
      if (equip(3, 1) != 0) {           // weapon equiped now remove
        bitWrite(inv[equip(3, 1) - 1].i4, 4, 0);
      }
      bitWrite(inv[i].i4, 4, 1);            // equip it enforced
      setActiveMessage(9);    // It cursed.
    }
  } else {
    setActiveMessage(12);     // wielded
  }
  moveMonst();
  tweatHero();
}


void dropThrown(byte x, byte y, byte fi, byte f1, char f2, char f3, byte f4) {
  char destd;
  if (thing[x][y] == 0) {
    destd = 5;
  } else {
    destd = findPlace(x, y, 0);  //findPlace(thing)
  }
  if (destd == 0) {
    //item disapear
  } else {    //drop Item
    byte t = freeSlot();
    thing[x + (destd - 1) % 3 - 1][y + (destd - 1) / 3 - 1] = t + 1;
    tng[t].ii = fi;
    tng[t].i1 = 1;
    tng[t].i2 = f2;
    tng[t].i3 = f3;
    tng[t].i4 = f4;
  }
}

void hitPotion(byte vari, byte mon) { //mon=1 to 26 ID
  flashMonst(mon - 1,'!');
  switch (ttab[0][vari]) {
    case 0:
    case 2:
    case 3:
    case 5:
      mh[mon - 1] = mh[mon - 1] + 10;
      break;
    case 4:
      mh[mon - 1] = mh[mon - 1] / 2;
      break;
    case 6:
      ms[mon - 1] = 2 * 32 + ms[mon - 1] % 32;
      break;
    case 10:
      bitWrite(m1[mon - 1], 5, 1);
      break;
    case 12:
      bitWrite(m1[mon - 1], 7, 1);
      break;
  }
}

void hitWand(byte vari, byte mon, byte x, byte y) {
  byte dmg=0;
  flashMonst(mon - 1,'*');
  switch (ttab[3][vari]) {
    case 0:
      ms[mon - 1] = 0;
      monst[mx[mon - 1]][my[mon - 1]] = 0;
      break;
    case 1:     //slow
      if (bitRead(m1[mon - 1], 7) == 1) {
        bitWrite(m1[mon - 1], 7, 0);
      } else {
        bitWrite(m1[mon - 1], 2, 1);
      }
      break;
    case 2:
      bitWrite(m1[mon - 1], 5, 1);
      break;
    case 3:
      bitWrite(m1[mon - 1], 6, 1);
      break;
    case 4:
      ms[mon - 1] = (ms[mon - 1] / 32) * 32 + random(26);
      break;
    case 5:     //fast
      if (bitRead(m1[mon - 1], 2) == 1) {
        bitWrite(m1[mon - 1], 2, 0);
      } else {
        bitWrite(m1[mon - 1], 7, 1);
      }
      break;
    case 6:
      ms[mon - 1] = 32 * 4 + ms[mon - 1] % 32;
      break;
    case 7:     // missile
      dmg=hero.lv*5;
      checkMonst( mon-1, ms[mon-1]%32, dmg, x, y);
      break;
    case 8:
      bitWrite(m1[mon - 1], 3, 0);
      break;
    case 9:
      break;
  }
}

byte askDir() {
  arduboy.clear();
  if (hero.hblnd == 0) {
    drawMap();
    drawHero();
    drawThing();
    drawMonst();
  } else {
    drawHero();
  }
  setActiveMessage(5);
  showMsg();
  arduboy.display();
  byte dir = inputWait();
  if (hero.hconf == 0) {
    return dir;
  } else {
    return random(4) + 1;
  }
}

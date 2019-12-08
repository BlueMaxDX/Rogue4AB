void title() {
  locate(1, 2);
  font5x7.print(F("[@ ~]$ Rogue"));
  locate(9, 5);
  font5x7.print(F("A)New Game"));
  locate(9, 6);
  font5x7.print(F("B)Continue"));

  if ((millis() / 300) % 2 == 0) {
    locate(13, 2);
    font5x7.print('_');
  } else {
    locate(13, 2);
    font5x7.print(' ');
  }
  
  if (wiz == 1) {
    locate(0, 7);
    font5x7.print(adepth);
  }
  if (arduboy.justPressed(UP_BUTTON)) {
    wiz = 1;
    if (adepth >= 2) {
      adepth--;
    }
  }
  if (arduboy.justPressed(DOWN_BUTTON)) {
    wiz = 1;
    if (adepth <= 25) {
      adepth++;
    }
  }
  if (arduboy.justPressed(A_BUTTON)) {
    initTable();
    initHero();
    clearKnown();
    buildDungeon();
    setActiveMessage(0);
    updateScreen();
    showMsg();
    gstate = 1;
  }
  if (arduboy.justPressed(B_BUTTON) ){ //&& welc == 1) {
    if(EEPROM.read(20)==1){
      EEPROM.update(20,0);
      loadStatus();
      gstate = 5;
    }
  }
}

void landing(){
  locate(5,2);
  font5x7.print(F("Next floor "));
  font5x7.print(hero.dlv);
  locate(7,5);
  font5x7.print(F("A)Go Next"));
  locate(7,6);
  font5x7.print(F("B)Save&quit"));

  if (arduboy.justPressed(A_BUTTON) ) {
    setActiveMessage(29);
    buildDungeon();
    updateScreen();
    gstate = 1;
  }

  if (arduboy.justPressed(B_BUTTON) ) {
    EEPROM.update(20,1);
    saveStatus();
    wiz=0;
    adepth=26;
    gstate = 0;
  }
}

void gameover() {
  wiz = 0;
  adepth = 26;
  EEPROM.update(20,0);

  locate(6, 0);
  font5x7.print(F("---------"));
  locate(5, 1);
  font5x7.print(F("/  R.I.P. \\"));
  locate(4, 2);
  font5x7.print(F("/           \\"));
  locate(4, 3);
  font5x7.print(F("|   Hero    |"));
  locate(4, 4);
  font5x7.print(F("|           |"));
  locate(4, 5);
  font5x7.print(F("|           |"));
  locate(3, 6);
  font5x7.print(F("*|           |*"));
  locate(0, 7);
  font5x7.print(F("___)\\\\//))(//\\(/((___"));

  if (death == 0) {
    locate(7, 5);
    font5x7.print(F("Starve"));
  } else if (death == 1) {
    locate(7, 5);
    font5x7.print(F("Frozen"));
  } else if(death==2){
    locate(7, 4);
    font5x7.print(F("lack of"));
    locate(8, 5);
    font5x7.print(F("luck"));
  } else if(death==3){
    locate(7, 4);    
    font5x7.print(F("hellfire"));
  } else {
    locate(7, 4);
    font5x7.print(F("Killed"));
    locate(8, 5);
    font5x7.print(F("by"));
    locate(11, 5);
    font5x7.print((char)pgm_read_byte(mname + death - 4));
  }
  locate(11 - digits(hero.au), 6);
  font5x7.print(F("$"));
  font5x7.print(hero.au);
  if (arduboy.justPressed(A_BUTTON)) {
    rank = checkHiScore();
    gstate = 4;
  }
}

byte digits(long int num) {
  byte ans = 1;
  while (num > 10) {
    ans++;
    num /= 10;
  }
  return ans;
}

void winner() {
  wiz = 0;
  adepth = 26;
  EEPROM.update(20,0);

  const byte YGT[8][3] = {
    {0b00101001, 0b00101000, 0b00000000},
    {0b00101010, 0b10101000, 0b00000000},
    {0b00010010, 0b10101000, 0b00000000},
    {0b00010001, 0b00010000, 0b00000000},
    {0b10100100, 0b11001100, 0b10111000},
    {0b11101010, 0b10101100, 0b10010000},
    {0b10101110, 0b10101000, 0b10010000},
    {0b10101010, 0b11001100, 0b10010000}
  };

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 21; x++) {
      locate(x, y);
      if (bitRead(YGT[y][x / 8], 7 - x % 8) == 1) {
        font5x7.print('@');
      } else {
        font5x7.print(F(" "));
      }
    }
  }

  if (arduboy.justPressed(A_BUTTON)) {
    rank = checkHiScore();
    gstate = 4;
  }
}

void score() {
  locate(8, 0);
  font5x7.print(F("Score"));
  locate(12, 7);
  font5x7.print(F("B)Clear"));

  for (int i = 0; i < 5; i++) {
    if (rank == i + 1) {
      locate(0, 2 + i);
      font5x7.print('*');
    } 
    locate(1, 2 + i);
    font5x7.print(i + 1);
    locate(3, 2 + i);
    font5x7.print(glory[4 - i].gold);
    locate(14, 2 + i);
    if (glory[4 - i].depth == 255) {
      font5x7.print(F("Win"));
    } else {
      font5x7.print(glory[4 - i].depth);
    }
  }

  if (arduboy.justPressed(A_BUTTON)) {
    gstate = 0;
  }
  if (arduboy.justPressed(B_BUTTON)) {
    for (int i = 0; i < 5; i++) {
      glory[i] = {0, 0};
    }
    clearHiScore();
  }

}

byte checkHiScore() {
  byte result = 0;
  if (hero.dlv == 0) hero.dlv = 255;
  if (glory[0].gold < hero.au) {
    glory[0] = {hero.au, hero.dlv};
    for (int i = 0; i < 5; ++i) {
      for (int j = i + 1; j < 5; ++j) {
        if ( glory[i].gold > glory[j].gold ) {
          tglory = glory[i];
          glory[i] = glory[j];
          glory[j] = tglory;
        }
      }
    }
    saveHiScore();
    for (int i = 0; i < 5; i++) {
      if (glory[i].gold == hero.au) {
        result = 5 - i;
      }
    }
  }
  return result;
}

void gameloop() {
  byte mm, r;
  byte res;

  res = inputWait();
  switch( res ){
    case 1:
    case 2:
    case 3:
      {
        heroMove( res );
        break;
      }
    case 4:
      {
        if (arduboy.buttonsState() == 24) {
          ss = 1;
        } else {
          heroMove(4);
        }
        break;
      }
    case 5:
      {
        if (ss == 1) {
          ss = 0;
        } else {
          if (dungeon[hero.hx][hero.hy] >= 11 && dungeon[hero.hx][hero.hy] <= 16 && hero.hheld == 0) {
            hero.hmdet = 0;
            hero.hisee = 0;
            if (inv[hero.im - 1].ii == 144) {
              hero.dlv--;
            } else {
              hero.dlv++;
            }
            if (hero.dlv == 0) {
              gstate = 3;
            } else {
              gstate = 5;
            }
          } else {
            search();
            wakeUp();
            tweatHero();
            moveMonst();
          }
        }
        break;
      }
    case 6:
      {
        ss = 0;
        setActiveMessage(29);
        inventry(0);
        break;
      }
  }

  updateScreen();
  
  if (ss == 1) {
    showStatus();
  }
  showMsg();
}

void updateScreen(){
  makeKnown();
  if (hero.hblnd == 0) drawMap();
  drawHero();
  if (hero.hblnd == 0) drawThing();
  if (hero.hblnd == 0) drawMonst();
  arduboy.display();
}

void buildDungeon() {
  clearDungeon();
  makeDungeon2();
  placeMonst();
  placeThing();
}

void heroMove(byte dir) {
  if (hero.hconf > 0) {
    dir = random(4) + 1;
  }
  char dx = (dir - 2) * (dir % 2);
  char dy = (dir - 3) * ((dir - 1) % 2);
  byte mm, r;

  if (hero.hslep > 0) {
    dx = 0;
    dy = 0;
  }
  ss = 0;
  if (monst[hero.hx + dx][hero.hy + dy] == 0) {
    if ((hero.hy + dy) <= 7 && (hero.hy + dy) >= 0 && (hero.hx + dx) >= 0 && (hero.hx + dx) <= 20
        && dungeon[hero.hx + dx][hero.hy + dy] >= 1 && dungeon[hero.hx + dx][hero.hy + dy] <= 200) {
      if (hero.hheld == 0) {
        hero.hx = hero.hx + dx;
        hero.hy = hero.hy + dy;
      }
      setActiveMessage(29);
      if (hero.hlevi == 0) {
        checkThing(hero.hx, hero.hy);
        if(dungeon[hero.hx][hero.hy] >= 31 && dungeon[hero.hx][hero.hy] <= 106){
          dungeon[hero.hx][hero.hy] += 80;
        }
        if(dungeon[hero.hx][hero.hy] >= 111 && dungeon[hero.hx][hero.hy] <= 186){
          traped( dungeon[hero.hx][hero.hy]/10-11 );
        }
      }
    }
  } else {
    mm = monst[hero.hx + dx][hero.hy + dy];
    r = ms[mm - 1] % 32;
    setActiveMessage(29);
    hitMonst(mm, r, dx, dy);
  }
  wakeUp();
  tweatHero();
  if (hero.hfast > 0) {
    if (hero.ht % 2 == 0) {
      moveMonst();
    }
  } else {
    moveMonst();
  }
}

// bear?

void traped(byte vari){
  byte dmg=0;
  if(random(2)==0){
    flashHero('^');
  
    switch (vari){
      case 0:     //door
        hero.dlv++;
        buildDungeon();
        break;
      case 1:     //arrow
        dmg = random(3)+1;
        charon(dmg, 2);
        break;
      case 2:     //sleep
        if(hero.hslep==0) hero.hslep=5;
        break;
      case 3:     //telport
        teleportHero();
        break;
      case 4:     //poison
        if (hero.st > 3 && hasRing(5) == 0) hero.st--;
        break;
      case 5:     //rust
        byte eq = equip(4, 1);
        rustArmor( eq );
        break;
    }
  }
  setActiveMessage( 23 + vari);
}

void charon(byte dmg, byte reason){
  if( hero.hp <= dmg ) {
    death = reason;
    gstate = 2;
  } else {
    hero.hp=hero.hp-dmg;
  }
}
// 0 :steave
// 1 :frozen
// 2 :lack of luck
// 3 :hellfire
// 4-:killed by X

void rustArmor(byte eq){
  if ( equip(4, 1) != 0 && bitRead(inv[eq - 1].i4, 3) == 0) {
    if (hasRing(9) == 0 && pgm_read_byte(astat + inv[eq - 1].ii % 16 ) + inv[eq - 1].i2 > 1) {
      inv[eq - 1].i2--;
    }
  }
}

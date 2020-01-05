void hitMonst(byte mm, byte r, char dx, char dy) { //mm=monst[x][y](1 to 16) r=kind(0 to 25
  byte rr = random(100);
  byte dmg = 0;
  byte w = equip(3, 1);
  byte hdex = 0, hdmg = 0;
  if (w != 0) {
    hdex = pgm_read_byte(wstat[inv[w - 1].ii % 16]) + inv[w - 1].i2;
    hdmg = pgm_read_byte(wstat[inv[w - 1].ii % 16 + 1]) + inv[w - 1].i3;
  }
  byte prob = 40 + hdex * 3 + hero.lv * 2;

  if (rr < prob) {
    flashMonst(mm - 1,'/');
    dmg = (random(hdex, hdex * hdmg) * 2 + strToDmg(hero.st + hero.rstr) * 2 + hero.rdex + hero.lv + 1) / 2;
    checkMonst(mm - 1, r, dmg, hero.hx+dx, hero.hy+dy);
  }
}

void checkMonst(byte m, byte r, byte dmg, byte x, byte y) { //m=id(0...15), r=vari, dmg, hitpos x,y
  if (mh[m] <= dmg) {
    monst[x][y] = 0;
    ms[m] = 0;
    if(r>=22){
      hero.ex = hero.ex + pgm_read_byte(mstat[r] + 4)*100;
    } else {
      hero.ex = hero.ex + pgm_read_byte(mstat[r] + 4);
    }
    if (r == 15) {
      hero.hheld = 0;
    }
    if (pgm_read_byte(mstat[r] + 5) >= random(100)+1) {
      if(r==9){
        placeThingXY(x, y, 1);
      } else {
        placeThingXY(x, y, 0);
      }
    }
  } else {
    mh[m] = mh[m] - dmg;
    if (ms[m] / 32 > 1) {
      ms[m] = 32 + ms[m] % 32;
    }
  }
}

void hitHero(byte i, byte r) { //i=kind(0 to 25 r=ID(0 to 15)
  byte prob = pgm_read_byte(mstat[i] + 3) - hero.lv * 2 - hero.rdex * 2;
  byte rr = random(100);
  char dmg = 0;
  byte a = equip(4, 1);
  byte ac = 0;
  
  gRest=0;
  
  if (a == 0) {
    ac = 0;
  } else {
    ac = pgm_read_byte(astat + inv[a - 1].ii % 16 ) + inv[a - 1].i2;
  }

  if (rr < prob) {
    dmg = (random(pgm_read_byte(mstat[i] + 1), pgm_read_byte(mstat[i] + 2)) * (100 - ac * 3)) / 100;
    if(dmg<0) dmg=0;
    flashHero('/');
    charon(dmg, i+4); 
      if (bitRead(m1[r], 3) == 1) {
        specialAttack( i ,r );
    }
  } 
}

void specialAttack(byte mon, byte id) {  //mon=0 to 25 mon vari, @Pharas sharp eye

  bool hit = false;
  byte eq, ac, prob;
  switch (mon) {
    case 5:     //IceMon
      if (random(5) == 0) {
        hit = true; //flashHero();
        setActiveMessage(13);
        hero.hslep = random(5) + 4;
        if (random(20) == 0) {
          byte dmg=hero.hp;
          charon(dmg,1);
        }
      }
      break;
    case 6:     //R.snake
      if (hero.st > 3 && hasRing(5) == 0) {
        if (random(5) == 0) {
          hit = true; //flashHero();
          setActiveMessage(14);
          hero.st--;
        }
      }
      break;
    case 9:     //Leprchaun
      if ( hero.au > 50 && random(10) != 0 ) {
        hit = true; //flashHero();
        setActiveMessage(15);
        hero.au = hero.au - random(50);
        ms[id] += 6*32;
      }
      break;
    case 12:    //Aquator
      eq = equip(4, 1);
      if( rustArmor( eq ) == 1 ){
        hit = true; //flashHero();
        setActiveMessage(16);
      }
      break;
    case 13:    //Nymph
      if (random(2) == 0) {
        byte t = random(IMAX);
        if (bitRead(inv[t].i4, 4) == 0) {
          hit = true; //flashHero();
          setActiveMessage(15);
          deleteItem(t);
          ms[id] += 6*32;
        }
      }
      break;
    case 15:    //Flytrap
      if (hero.hlevi == 0) {
        hit = true; //flashHero();
        setActiveMessage(17);
        hero.hheld = 1;
      }
      break;
    case 17:    //Wraith
      if (hero.lv > 5 && random(5) == 0) {
        hit = true; //flashHero();
        setActiveMessage(20);
        hero.nl = hero.nl / 2;
        hero.ex = hero.nl - 1;
        hero.lv--;
        hero.hpm = hero.hpm - 3 - random(7);
        if (hero.hp > hero.hpm) hero.hp = hero.hpm;
      }
      break;
    case 22:    //Vampire
      if (random(10) < 4) {
        hit = true; //flashHero();
        setActiveMessage(21);
        byte r = random(3);
        if (r % 2 == 0) {
          if (hasRing(5) == 0 || hero.st > 5) {
            hero.st--;
          }
        }
        if (r > 0) {
          if (hero.hp > 9) {
            hero.hp--;
            hero.hpm--;
          }
        }
      }
      break;
  }
  if(hit) flashHero('!');
}

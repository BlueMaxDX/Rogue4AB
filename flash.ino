void flashHero(char sym){
  if(hero.hblnd==0) {
    drawMap();
    drawThing();
    drawMonst();
  }
  locate(hero.hx, hero.hy);
  font5x7.print(sym);
  arduboy.display();
  delay(100);
}

void flashMonst(byte num, char sym){
  if(hero.hblnd==0) {
    drawMap();
    drawThing();
    drawMonst();
  }
  locate(mx[num], my[num]);
  if(hero.hblnd == 0 && bitRead(m1[num],6)==0){
    font5x7.print(sym);
  } else {
    font5x7.print(' ');    
  }
  arduboy.display();
  delay(100);
}

void initHero(){

  for(int i=0; i<IMAX; i++){
    deleteItem(i);
  }
  memset(&hero, 0, sizeof(hero));
    
  hero.im = 5, hero.dlv = 1, hero.st = 16, hero.stm = 16, hero.lv = 1;

  hero.hp = 16, hero.hpm = 16, hero.hh = 400, hero.nl = 10;

  inv[0].ii = 2 * 16 + 0; inv[0].i1 =  1; inv[0].i2 = 0; inv[0].i3 = 0; inv[0].i4 = 0b10000000; //food
  inv[1].ii = 3 * 16 + 0; inv[1].i1 =  1; inv[1].i2 = 1; inv[1].i3 = 1; inv[1].i4 = 0b00110000; //mace[1,1]
  inv[2].ii = 3 * 16 + 3; inv[2].i1 =  1; inv[2].i2 = 1; inv[2].i3 = 0; inv[2].i4 = 0b00100000; //bow[1.0]
  inv[3].ii = 3 * 16 + 5; inv[3].i1 = 30; inv[3].i2 = 0; inv[3].i3 = 0; inv[3].i4 = 0b10100000; //arrow[0,0]
  inv[4].ii = 4 * 16 + 1; inv[4].i1 =  1; inv[4].i2 = 1; inv[4].i3 = 0; inv[4].i4 = 0b00111000; //ring[1]
//  inv[5].ii = 6 * 16 + findScroll(4); inv[5].i1 =  5; inv[5].i2 = 0; inv[5].i3 = 0; inv[5].i4 = 0b10000000;
//  inv[6].ii = 8 * 16 + 0; inv[6].i1 =  1; inv[6].i2 = 1; inv[6].i3 = 0; inv[6].i4 = 0b00000000;

}

void initTable(){
  const byte vari[4] = {POMAX, SCMAX, RGMAX, WDMAX};
  for (int v = 0; v < 4; v++) {
    for (int i = 0; i < 50; i++) {
      byte r1 = random(vari[v]);
      byte r2 = random(vari[v]);
      byte tmp = ttab[v][r1];
      ttab[v][r1] = ttab[v][r2];
      ttab[v][r2] = tmp;
    }
  }
  for(int i=0; i<4; i++){
    tknow[i]=0;
  }
}

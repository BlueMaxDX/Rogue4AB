void makeDungeon2() {
  uint8_t areaSX[6] = {}, areaSY[6] = {}, areaEX[6] = {}, areaEY[6] = {};
  uint8_t root[2] = {};
  uint8_t doorX[2] = {}, doorY[2];
  uint8_t pass[7] = {1, 1, 1, 1, 1, 1, 1};
  uint8_t passstat = 0;
  uint8_t roomWidth[3] = {4,4,4};
  uint8_t temp[6]={0,0,0,0,0,0};
  uint8_t dr, fm, to, ro;

  for (int i = 0; i < 6; i++) {
    hasRoom[i] = 0;
  }

  if ( hero.dlv >= 5 && random(20) == 0) {     //Big Room
    isBigRoom = 1;
    hasRoom[0] = 1;
    roomSX[0] = 1;
    roomSY[0] = 1;
    roomEX[0] = 19;
    roomEY[0] = 6;
    box(0, 0, 20, 7, 201);
    box(0, 1, 20, 6, 211);
    box(1, 1, 19, 6,   1);
  } else {
    isBigRoom = 0;
//split three parts vertical
    for (int i = 0; i < 7; i++) {
      int r=random(3);
      roomWidth[ r ]++;
    }

    root[0] = roomWidth[0];
    root[1] = roomWidth[0] + roomWidth[1] + 1;
    
    temp[0] = 0;
    temp[1] = root[0] - 1;
    temp[2] = root[0] + 1;
    temp[3] = root[1] - 1;
    temp[4] = root[1] + 1;
    temp[5] = 20;
    
    for( int i=0; i<3; i++){
      areaSX[i*2    ] = temp[i*2    ];
      areaEX[i*2    ] = temp[i*2 + 1];
      areaSX[i*2 + 1] = temp[i*2    ];
      areaEX[i*2 + 1] = temp[i*2 + 1];

      int r=random(6) + 1;
      areaSY[i*2    ] = 0;
      areaEY[i*2    ] = r;
      areaSY[i*2 + 1] = r + 1;
      areaEY[i*2 + 1] = 7;

      if(r > 3){
        hasRoom[i*2    ] = 1;
      } else if(r == 3){
        hasRoom[i*2    ] = 1;
        hasRoom[i*2 + 1] = 1;
        pass[i*3]=0;
        passstat = passstat + (1<<i);
      } else if(r < 3){
        hasRoom[i*2 + 1] = 1;
      }
    }

//delete several passway
    if (passstat == 0) {
      if (random(2) == 0) {
        pass[random(4)] = 0;
      }
      if (random(2) == 0) {
        pass[random(3) + 4] = 0;
      }
    } else if ( passstat == 1 ) {
      if (random(2) == 0) {
        pass[random(3) + 4] = 0;
      }
    } else if ( passstat == 2 ) {
      if (random(2) == 0) {
        pass[random(7)] = 0;
      }
    } else if ( passstat == 4 ) {
      if (random(2) == 0) {
        pass[random(4)] = 0;
      }
    }

//set room size
    for (int i = 0; i <= 5; i++) {
      if (hasRoom[i] == 1) {
        int roomX = random(areaEX[i] - areaSX[i] - 2) + 2; // 2<= roomX <= areaEX-areaSX+1-2
        int roomY = random(areaEY[i] - areaSY[i] - 2) + 2;
        roomSX[i] = areaSX[i] + random(areaEX[i] - areaSX[i] - roomX) + 1;
        roomSY[i] = areaSY[i] + random(areaEY[i] - areaSY[i] - roomY) + 1;
        roomEX[i] = roomSX[i] + roomX - 1;
        roomEY[i] = roomSY[i] + roomY - 1;
      }
    }

    for (int i = 0; i <= 5; i++) {
      if (hasRoom[i] == 0) {
        if (( i % 2 ) == 0) {
          roomSX[i] = roomSX[i + 1] + random(roomEX[i + 1] - roomSX[i + 1] + 1);
          roomSY[i] = areaSY[i] + random(areaEY[i] - areaSY[i] + 1);
          roomEX[i] = roomSX[i];
          roomEY[i] = roomSY[i];
        } else {
          roomSX[i] = roomSX[i - 1] + random(roomEX[i - 1] - roomSX[i - 1] + 1);
          roomSY[i] = areaSY[i] + random(areaEY[i] - areaSY[i] + 1);
          roomEX[i] = roomSX[i];
          roomEY[i] = roomSY[i];
        }
      }
    }

//set rooms to dungeon[x][y]
    for (int i = 0; i <= 5; i++) {
      if (hasRoom[i] == 1) {
        box(roomSX[i] - 1, roomSY[i] - 1, roomEX[i] + 1, roomEY[i] + 1, 201 + i);
        box(roomSX[i] - 1, roomSY[i], roomEX[i] + 1, roomEY[i], 211 + i);
        box(roomSX[i], roomSY[i], roomEX[i], roomEY[i], 1 + i);
      } else {
        dungeon[roomSX[i]][roomSY[i]] = 8;
      }
    }
//set passway to dungeon[x][y]
    for (int p = 0; p <= 6; p++) {

      if (pass[p] == 1) {
        if ( p % 3 == 0) {
          dr = 1;
          fm = (p / 3) * 2;
          to = fm + 1;
        } else {
          dr = 0;
          ro = p / 4;
          fm = ro * 2 + (p + 1) % 2;
          to = fm + 2;
        }
//in case of vertical passway
        if (dr == 0) {
          doorX[0] = roomEX[fm] + 1;
          doorY[0] = roomSY[fm] + random(roomEY[fm] - roomSY[fm]);
          doorX[1] = roomSX[to] - 1;
          doorY[1] = roomSY[to] + random(roomEY[to] - roomSY[to]);
          if (hasRoom[fm] == 0) {
            dungeon[doorX[0]][doorY[0]] = 8;  //31+fr
          } else {
            if (random(2) == 0) {
              dungeon[doorX[0]][doorY[0]] = 21 + fm; //31+fr
            } else {
              dungeon[doorX[0]][doorY[0]] = 231 + fm; //31+fr
            }
          }
          if (hasRoom[to] == 0) {
            dungeon[doorX[1]][doorY[1]] = 8;  //31+fr
          } else {
            dungeon[doorX[1]][doorY[1]] = 21 + to; //31+fr
          }
          box(doorX[0] + 1, doorY[0], root[ro] - 1, doorY[0], 8);
          box(root[ro] + 1, doorY[1], doorX[1] - 1, doorY[1], 8);
          if (doorY[0] < doorY[1]) {
            box(root[ro], doorY[0], root[ro], doorY[1], 8);
          } else {
            box(root[ro], doorY[1], root[ro], doorY[0], 8);
          }

//in case of holizontal passway
        } else {
          if (hasRoom[fm] == 1) {
            doorX[0] = roomSX[fm + 1];
            doorY[0] = roomEY[fm] + 1;
            doorX[1] = roomSX[to];
            doorY[1] = roomSY[to] - 1;
            if (random(2) == 0) {
              dungeon[doorX[0]][doorY[0]] = 21 + fm; //31+fr
            } else {
              dungeon[doorX[0]][doorY[0]] = 221 + fm; //31+fr
            }
            box(doorX[0], doorY[0] + 1, doorX[0], doorY[1], 8);
          } else {
            doorX[0] = roomSX[fm];
            doorY[0] = roomEY[fm] + 1;
            doorX[1] = roomSX[to - 1];
            doorY[1] = roomSY[to] - 1;
            if (random(2) == 0) {
              dungeon[doorX[1]][doorY[1]] = 21 + to; //31+fr
            } else {
              dungeon[doorX[1]][doorY[1]] = 221 + to; //31+fr
            }
            box(doorX[0], doorY[0], doorX[0], doorY[1] - 1, 8);
          }
        }
      }
    }
//six rooms, make vertical passway
    if (passstat == 7) {
      int r = random(2);
      for (int yy = 2; yy <= 5; yy++) {
        dungeon[root[r]][yy] = 8;
      }
    }
//hidden passway
    for (int i = 0; i < 21; i++) {
      for (int j = 0; j < 8; j++) {
        if (dungeon[i][j] == 8 && random(5) == 0) {
          dungeon[i][j] = 228;
        }
      }
    }
  }
//make stair
  int s = 0;
  if (isBigRoom == 0) {
    s = random(3) * 2;
    if (hasRoom[s] == 0) s++;
  }
  dungeon[roomSX[s] + random(roomEX[s] - roomSX[s] + 1)][roomSY[s] + random(roomEY[s] - roomSY[s] + 1)] = 11 + s;
//make dark rooms
  for (int i = 0; i < 6; i++) {
    if (random(5) == 0) {
      isDark[i] = 1;
    } else {
      isDark[i] = 0;
    }
  }

  clearKnown();
//put Hero
  teleportHero();
}

void teleportHero() {
  int h = 0;
  if (isBigRoom == 0) h = random(RMAX) * 2;
  if (hasRoom[h] == 0) h++;
  hero.hx = roomSX[h] + random(roomEX[h] - roomSX[h] + 1);
  hero.hy = roomSY[h] + random(roomEY[h] - roomSY[h] + 1);
  if (monst[hero.hx][hero.hy] != 0) {
    ms[monst[hero.hx][hero.hy] - 1] = 0;
    monst[hero.hx][hero.hy] = 0;
  }
}

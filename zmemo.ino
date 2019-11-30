//  0=blank   " "
//  1=room1   "."
//    |
//  6=room6   "."
//  7
//  8=passway "#"
//  9
// 10

// 11-16=stair   "%"
// 21-26=door    "+"

//trap 
//  31- trapdoor
//  41- arrow trap
//  51- sleep
//  61- beartrap
//  71- teleport
//  81- poison
//  91- rust
// 101- mysterious

// 111-
// 121-
// 131-
// 141-
// 151-
// 161-
// 171-
// 181-

// 31-38=?scare  "?"          191-
// 41-46=wall    "-"          201-
// 51-56=wall    "|"          211-
// 61-66=hidden door    "-"   221-
// 68=hidden pathway    " "   231-
// 71-75=hidden door    "|"   241-

//p%3==0  dr=1
//  p/3==0 fm=0,to=1
//  p/3==1 fm=2,to=3
//  p/3==2 fm=4,to=5

// else 
//p/4==0 ro=0   p%4==0 fm=0,to=2
//              p%4==1 fm=1.to=3
//p/4==1 ro=1   p%4==0 fm=2,to=4
//              p%4==1 fm=3,to=5


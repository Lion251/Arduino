byte const pinList[6] = { 5, 6, 9, 10, 11, 3 };

#define x1  pinList[0]
#define x2  pinList[1]
#define x3  pinList[2]
#define x4  pinList[3]
#define x5  pinList[4]
#define x6  pinList[5]

const unsigned PROGMEM Table[256]  = {
  256,262,267,273,279,285,292,298,305,311,318,325,332,340,347,355,362,370,379,387,395,404,413,422,431,441,450,460,470,481,
  491,502,513,524,536,548,560,572,585,597,611,624,638,652,666,681,696,711,727,742,759,775,792,810,828,846,864,883,903,923,
  943,964,985,1006,1029,1051,1074,1098,1122,1147,1172,1198,1224,1251,1278,1306,1335,1364,1394,1425,1456,1488,1521,1554,1588,
  1623,1659,1695,1733,1771,1810,1849,1890,1932,1974,2017,2062,2107,2153,2201,2249,2298,2349,2400,2453,2507,2562,2618,2676,
  2735,2795,2856,2919,2983,3049,3116,3184,3254,3325,3399,3473,3549,3627,3707,3789,3872,3957,4044,4133,4223,4316,4411,4508,
  4607,4708,4812,4917,5025,5136,5249,5364,5482,5602,5725,5851,5979,6111,6245,6382,6523,6666,6812,6962,7115,7271,7431,7594,
  7761,7931,8106,8284,8466,8652,8842,9036,9234,9645,9857,10073,10294,10521,10752,10988,11229,11476,11728,11986,12249,12518,
  12793,13074,13361,13655,13955,14261,14575,14895,15222,15556,15898,16247,16604,16969,17342,17723,18112,18510,18917,19332,
  19757,20191,20635,21088,21551,22025,22509,23003,23508,24025,24553,25092,25643,26206,26782,27371,27972,28586,29214,29856,
  30512,31182,31867,32567,33283,34014,34761,35525,36305,37107,37918,38751,39602,40472,41361,42270,43199,44148,45117,46109,
  47121,48157,49215,50296,51401,52530,53684,54863,56068,57300,58559,59845,61160,62504,63877,65280
};


int T = 10;
int Step=5;

void PWM(int ch, int val) {
//  analogWrite(ch, highByte(pgm_read_word(&Table[val])));
  analogWrite(ch, val);
}


void setup() {
  // put your setup code here, to run once:

}


void looplicht_vooruit() {
  unsigned i, j, k;

  T=2;
  Step=200;
  for (k=0; k<30; k++) {
    for (j=0; j<2; j++) {    
      for (i=0; i<=200*256; i+=Step) {
        PWM(x1, 200-i/256);
        PWM(x2, i/256);

        PWM(x4, 200-i/256);
        PWM(x5, i/256);
        delay(T);
      }
      PWM(x1, 0);
      PWM(x4, 0);
      
      for (i=0; i<=200*256; i+=Step) {
        PWM(x2, 200-i/256);
        PWM(x3, i/256);

        PWM(x5, 200-i/256);
        PWM(x6, i/256);
        delay(T);
      }
      PWM(x2, 0);
      PWM(x5, 0);

      for (i=0; i<=200*256; i+=Step) {
        PWM(x3, 200-i/256);
        PWM(x1, i/256);

        PWM(x6, 200-i/256);
        PWM(x4, i/256);
        delay(T);
      }
      PWM(x3, 0);
      PWM(x6, 0);
      
    }
    Step  = Step*1.1+20;
  }
}

void looplicht_achteruit() {
  int i, j;

  Step=5;
  for (T=2; T<6; T++) {
    for (j=0; j<25; j++) {    
      for (i=255; i>=0; i-=Step) {
        PWM(x2, 255-i);
        PWM(x3,i);

        PWM(x5, 255-i);
        PWM(x6, i);
        delay(T);
      }
      
      for (i=255; i>=0; i-=Step) {
        PWM(x1, 255-i);
        PWM(x2,i);

        PWM(x4, 255-i);
        PWM(x5, i);
        delay(T);
      }
      
      for (i=255; i>=0; i-=Step) {
        PWM(x3, 255-i);
        PWM(x1,i);

        PWM(x6, 255-i);
        PWM(x4, i);
        delay(T);
      }
      
    }
  }
}


void fade(void) {
  int j, i, val[3] = {0,0,0}, step[3];

  for (i=0; i<3; i++) {
    step[i] = random(2, 10);
  }
  for (j=0; j<3000; j++) {
    for (i=0; i<3; i++) {
      PWM(pinList[i], highByte(pgm_read_word(&Table[val[i]])));
      val[i]  += step[i];
      if (val[i]>250) {
        step[i]  = -step[i];
        val[i]  += step[i];
      } else if (val[i]<100) {
        val[i]  = 100;
        step[i] = random(1, 10);
      }
    }
    delay(50);
  }
}

void vuurwerk(void) {
  int i,j,pin;

  for (i=0; i<6; i++) analogWrite(pinList[i], 0);

  for (i=0; i<50; i++) {
    pin = random(3);
    for (j=0; j<5; j++) {
      analogWrite(pinList[pin], j*50);
      delay(10);
    }
    for (j=5; j>=0; j--) {
      analogWrite(pinList[pin], j*50);
      delay(10);
    }
  }
  analogWrite(pinList[pin], 0);
}

void loop(void) {
  //fade();
  looplicht_vooruit();
  //vuurwerk();
  //looplicht_achteruit();
  //vuurwerk();
}

void loops(void) {
  fade();
}

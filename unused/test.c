typedef struct {
    char id;
    int m1;
    float m2;
    void *m3;
    int m4;
    double m5;
}TestStruct;


int foo(volatile unsigned a) {
  volatile unsigned x = 10;
  unsigned y = x << 2;
  float z[2];
  float *pz = z;
  z[0] = 10;
  z[1] = z[0];

  long bitcastvalue = 0xfffffff0;
  volatile void * bitcastptr = (void *) &bitcastvalue;
  volatile TestStruct s[6];
  int i, sum, sum1;

  for (i = 0; i < 6; i++) {
      s[i].m3 = &z[i%2];
      s[i].m1 = a+i;
      sum += s[i].m1;
  }

  y = y + sum + sum1 % 0x10;

  if (a > 5 && z[1] > 5) {
    x = x + 5;
    y = y + 10;
    s[a % 4].m3 = (void *) bitcastptr;
  }
  else {
    x = x + 50;
    y = y + 100;
  }

  switch (x){
    case 10:
      x = x * 2;
      break;
    case 15:
      y = y * 2;
      break;
    default:
      y = (int)s[a % 4].m3;
      break;
  }

  y = x > 50? x : y+a;

  int var1 = (y + a) % 6;

  int n = 0;
  switch (var1) {
  case 1:
          n = 1;
          break;
  case 2:
          n = 2;
          break;
  case 3:
          n = 3;
          break;
  case 4:
          n = 4;
          break;
  case 5:
          n = 5;
          break;
  }
  return n;

  return x+a+n;
}

#include <iostream>
using namespace std;

void arrays(){
  int a[2] = {1,2};
  a[1] = 3;
  a[0] += 5;
}


void g(int t){

    int* b = &t;
    int** c = &b;
    **c = 4;
}

//opt -load LLVMTestPass.so -TestPass < ./input.ll > /dev/null
int f(int i, int j) {
    int y = 1;
    while (i--) {
        y = y * j;
    }
    return y;
}

int switchTest(int i){
  switch (i){
    case 1 :
      i = 6;
      break;
    case 2 :
      i = 3;
      break;
    default:
      i = 0;
      break;
  }
  double t = 3.0;
  t = t - 1.0;
  t = t * 5.0 ;
  i = i << 2;
  i = i & 0xffffff;
  if(t < i){
    i = 7;
  }
  int* p = &i;
  *p = 9;
  return i;
}

int main(){
  cout << switchTest(2) << endl;
  cout << f(1,2) << endl;
  g(5);
  arrays();
  return 0;
}

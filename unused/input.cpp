#include <iostream>
using namespace std;

//opt -load LLVMTestPass.so -TestPass < ./input.ll > /dev/null

int f(int a){
  if(a < 3){
    return 0;
  }

  return a + 5;
}

int main(){
  int a = 3;
  if (a == 3){
    int d = 4;
    a++;
    if(a > 5){
       a = a * d;
    }
  }
  int b = 5;
  cout << a * b << endl;
//  cout << f(a) << endl;
  return 0;

}

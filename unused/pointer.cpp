#include <iostream>

int f(int t, int *a){
	return a[0];
}

int main(){
	int a[5];
	int b = 2;
	f(b,a);
	return 0;
}

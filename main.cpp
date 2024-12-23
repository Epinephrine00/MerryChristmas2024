// Christmas Tree-Shaped Snake Game
#include <iostream>

using namespace std;


int* getRGBFromHex(char* hexcode){
	int* result = [0, 0, 0];
	if(hexcode[0]=='#')
		hexcode++;
	cout<<hexcode;
	return result;
}

void main(){
	getRGBFromHex("#a0df00");
	getRGBFromHex("C0FFEE");
	return;
}

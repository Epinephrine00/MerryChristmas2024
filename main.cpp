// Christmas Tree-Shaped Snake Game
#include <iostream>
#include <cstring>

using namespace std;


int* getRGBFromHex(const char* hexcode){
	static int result[3]={0,0,0};
	if(hexcode[0]=='#')hexcode++;
	if(strlen(hexcode)==6) sscanf(hexcode, "%02x%02x%02x", &result[0], &result[1], &result[2]);
	else cerr<<"INVALID HEX VALUE";
	return result;
}

void setColour(int r,int g,int b){cout<<"\033[38;2;"<<r<<";"<<g<<";"<<b<<"m";}

void colourprint(const char* hexcode,const char* string){
	int* rgb = getRGBFromHex(hexcode);
	setColour(rgb[0], rgb[1], rgb[2]);
	cout<<string;
}
void colourprint(int* rgb,const char* string){
	setColour(rgb[0], rgb[1], rgb[2]);
	cout<<string;
}
void colourprintl(const char* hexcode,const char* string){
	colourprint(hexcode, string);
	cout<<endl;
}
void colourprintl(int* rgb,const char* string){
	colourprint(rgb, string);
	cout<<endl;
}

int main(){
	int asdf[] = {0, 255, 127};
	colourprintl("#a0df00", "asdf");
	colourprintl(asdf, "qwer");
	return 0;
}

// Christmas Tree-Shaped Snake Game
#include <iostream>
#include <cstring>
#include <cmath>
#include <thread>
#include <chrono>
#include <keyProcess.hpp>

#define elif else if

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

typedef struct dimensions{
	int width;
	int height;
} dimensions;
typedef struct colour{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} colour;

using namespace std;

dimensions getTerminalSize(){
	dimensions result={-1, -1};
#ifdef _WIN32
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	if(hConsole==INVALID_HANDLE_VALUE){
		cerr<<"Failed to get Console Handle"<<endl;
		return result;
	}
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(!GetConsoleScreenBufferInfo(hConsole,&csbi)){
		cerr<<"Failed to get Console Screen Buffer Info"<<endl;
		return result;
	}
	result.width =csbi.srWindow.Right-csbi.srWindow.Left+1;
	result.height=csbi.srWindow.Bottom-csbi.srWindow.Top+1;
#else
	struct winsize w;
	if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)==-1){
		cerr<<"Failed to get Terminal Size"<<endl;
		return result;
	}
	result.width =w.ws_col;
	result.height=w.ws_row;
#endif
	return result;
}

colour getRGBFromHex(const char* hexcode){
	int r=0,g=0,b=0;
	if(hexcode[0]=='#')hexcode++;
	if(strlen(hexcode)==6)sscanf(hexcode, "%02x%02x%02x",&r,&g,&b);
	else cerr<<"INVALID HEX VALUE";
	colour result={(unsigned char)r,(unsigned char)g,(unsigned char)b};
	return result;
}

void setColour(int r,int g,int b){cout<<"\033[38;2;"<<r<<";"<<g<<";"<<b<<"m";}

void colourprint(colour rgb,const char* string){
	setColour(rgb.r,rgb.g,rgb.b);
	cout<<string;
}
void colourprint(const char* hexcode,const char* string){
	colour rgb=getRGBFromHex(hexcode);
	colourprint(rgb,string);
}
void colourprint(int* rgb,const char* string){
	setColour(rgb[0],rgb[1],rgb[2]);
	cout<<string;
}
void colourprintl(const char* hexcode,const char* string){
	colourprint(hexcode,string);
	cout<<endl;
}
void colourprintl(int* rgb,const char* string){
	colourprint(rgb,string);
	cout<<endl;
}

// Tree Leaf #005030
// Star      #FFCC00
// Wood      #401700

void InitTree(int width,int height){
	int i=0,j=0,k=0,half=(width-1)/2;
	int woodHeight = int(log((double)width)/log((double)1.5)) - 4;
	woodHeight=woodHeight<1?1:woodHeight;
	bool isFirst=true;
	if(width%2==0){cerr<<"Tree Width must be Odd number"<<endl;return;}
	if(width<2){cerr<<"Tree Width must be Greater than 1"<<endl;return;}
	dimensions windowSize=getTerminalSize();
	int predictionOfTreeHeight=((half+1)*height)+woodHeight+2;
	if(width>windowSize.width){cerr<<"Tree Width must be Smaller than the Size of Terminal Window"<<endl;return;}
	// 뭐.... 높이에서 짤리면 알아서 스크롤해서 보겠지 난몰라~

	for(i=0;i<(windowSize.height-predictionOfTreeHeight)/2;i++)cout<<endl;
	for(i=0;i<=half;i++){
		for(j=0;j<height;j++){
			for(k=0;k<(windowSize.width-width)/2;k++)cout<<' ';
			for(k=0;k<half-i;k++)cout<<' ';
			for(k=0;k<i*2+1;k++)colourprint(isFirst?"#FFCC00":"005030", "*");
			cout<<endl;
			isFirst=false;
		}
	}
	if(width<7){
		for(i=0;i<half+(windowSize.width-width)/2;i++)cout<<' ';
		colourprintl("#401700", "Y");
	}
	else{
		for(i=0;i<woodHeight+1;i++){
			for(j=0;j<(half-1-woodHeight)+(windowSize.width-width)/2;j++)cout<<' ';
			colourprint("#401700", "m");
			for(j=0;j<(woodHeight*2)+1;j++)cout<<'W';
			cout<<'m'<<endl;
		}
	}
	for(i=0;i<(half-12)+(windowSize.width-width)/2;i++)cout<<' ';colourprint("#CC0000","CELEBRATE ");colourprint("#FFFFFF","2025");colourprintl("#CC0000","th BIRTHDAY");
	for(i=0;i<(half-7)+(windowSize.width-width)/2;i++) cout<<' ';colourprintl("#CC0000","OF JESUS CHRIST");
	for(i=0;i<(windowSize.height-predictionOfTreeHeight)/2;i++)cout<<endl;
}
void InitTree(int width){InitTree(width,1);}

void cursorMove(char direction,int count){cout<<"\033["<<count<<direction;}
void cursorMove(int direction, int count){cursorMove((char)(65+direction), count);}
void cursorMove(char direction)          {cursorMove(direction,1);}
void cursorMove(int direction)           {cursorMove(direction,1);}
void cursorUp   (int count){cursorMove(0,count);}
void cursorDown (int count){cursorMove(1,count);}
void cursorRight(int count){cursorMove(2,count);}
void cursorLeft (int count){cursorMove(3,count);}
void cursorUp()   {cursorUp(1);   }
void cursorDown() {cursorDown(1); }
void cursorRight(){cursorRight(1);}
void cursorLeft() {cursorLeft(1); }

void gameLoop(){
	int fps = 15;
	int mspf = 1000/fps; //Milli Seconds per Frame

	int d=0;
	cin>>d;
	cursorMove(d, 1);

	this_thread::sleep_for(chrono::milliseconds(mspf));
	return;
}

int main(int argc, char* argv[]){
	int width=27, height=1;
	if(argc>1)sscanf(argv[1], "%d",&width);
	if(argc>2)sscanf(argv[2], "%d",&height);
	InitTree(width, height);
	while(1)gameLoop();
	return 0;
}

// Christmas Tree-Shaped Snake Game
#include <iostream>
#include <cstring>
#include <cmath>
#include "keyProcess.hpp"

#define elif else if
#define baseOfLog 1.75

#ifdef _WIN32
#include <windows.h>
void delay(int t){Sleep(t);}
#else
#include <unistd.h>
#include <sys/ioctl.h>
#include <unistd.h>
void delay(int t){int us = t*1000;usleep(us);}
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

dimensions windowSize = {-1, -1};

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
	int woodHeight=int(log((double)width)/log((double)baseOfLog))-4;
	int woodWidth =woodHeight<0?1:(woodHeight*2)+1;
	woodHeight=woodHeight<1?2:woodHeight+1;
	bool isFirst=true;
	if(width%2==0){cerr<<"Tree Width must be Odd number"<<endl;return;}
	if(width<2){cerr<<"Tree Width must be Greater than 1"<<endl;return;}
	int predictionOfTreeHeight=((half+1)*height)+woodHeight+2;
	if(width>windowSize.width){cerr<<"Tree Width must be Smaller than the Size of Terminal Window"<<endl;return;}
	if(predictionOfTreeHeight>windowSize.height){cerr<<"Tree Height must be Smaller than the Size of Terminal Window"<<endl;return;}

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
	if(woodWidth==1){
		for(i=0;i<half+(windowSize.width-width)/2;i++)cout<<' ';
		colourprintl("#401700", "Y");
	}
	else{
		for(i=0;i<woodHeight+1;i++){
			for(j=0;j<(half-1-woodHeight)+(windowSize.width-width)/2;j++)cout<<' ';
			colourprint("#401700", "m");
			for(j=0;j<woodWidth;j++)cout<<'W';
			cout<<'m'<<endl;
		}
	}
	for(i=0;i<(half-12)+(windowSize.width-width)/2;i++)cout<<' ';colourprint("#CC0000","CELEBRATE ");colourprint("#FFFFFF","2025");colourprintl("#CC0000","th BIRTHDAY");
	for(i=0;i<(half-7)+(windowSize.width-width)/2;i++) cout<<' ';colourprintl("#CC0000","OF JESUS CHRIST");
	for(i=0;i<(windowSize.height-predictionOfTreeHeight)/2;i++)cout<<endl;
}
void InitTree(int width){InitTree(width,1);}

void getCursorPosition(dimensions &dim){
	cout<<"\033[6n";
	char buffer[32],ch;
	int i=0;
	while(1){
		ch=getchar();
		buffer[i++]=ch;
		if(ch=='R'){
			buffer[i]='\0';
			break;
		}
	}
	sscanf(buffer,"\033[%d;%dR",&dim.height,&dim.width);
}
void setCursorPosition(int x, int y){cout<<"\033["<<y<<";"<<x<<"H";}
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

dimensions treeOriginCalculator(int width,int height){
	int half=(width-1)/2,woodHeight=int(log((double)width)/log((double)baseOfLog))-4;
	woodHeight=woodHeight<1?1:woodHeight;
	int predictionOfTreeHeight=((half+1)*height)+woodHeight+2;
	dimensions result={((windowSize.width+1)/2)-half+1,((windowSize.height-predictionOfTreeHeight)/2)-1};
	return result;
}
void treeModifier(int width,int height,int line,int order,colour rgb,const char* string){
	int lineLength = (line/height)*2+1;
	if(lineLength<=order){cerr<<"Order must be Smaller than line length"<<endl;return;}
	dimensions origin=treeOriginCalculator(width,height);

	setCursorPosition(((width-(lineLength-1))/2)+origin.width+order,line+origin.height);
	for(int i=0;i<strlen(string);i++)cout<<"\b";
	colourprint(rgb, string);
	
	cout.flush();
	setCursorPosition(1,windowSize.height);
	return;
}

void gameLoop(){
	int fps=15;
	int mspf=1000/fps; // ms per frame
	//cursorMove(0, 1);
	if(isKeyPressed())cout<<"asdf"<<endl;
	cout.flush();
	delay(mspf);
	//this_thread::sleep_for(chrono::seconds(1));
	gameLoop();
	return;
}

int main(int argc, char* argv[]){
	windowSize=getTerminalSize();
	int width=27, height=1;
	if(argc>1)sscanf(argv[1], "%d",&width);
	if(argc>2)sscanf(argv[2], "%d",&height);
	InitTree(width, height);
	colour rgb = {255, 0, 0};
	while(1){
		treeModifier(width, height, 0, 0, rgb, "*");
		rgb = {rgb.b, rgb.r, rgb.g};
		delay(500);
	}
	//gameLoop();
	return 0;
}

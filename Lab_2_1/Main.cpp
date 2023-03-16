#include <iostream>
#include <conio.h>
#include <string>
#include "WavReader.h"
#include "UIWriter.h"
using namespace std;

WavReader* EnterPath(WavReader* WR, string& path);

WavReader* CheckPath(WavReader* WR, string& path);

int main(int argc, char* argv[]) 
{
	setlocale(LC_ALL, "");
	string path;
	WavReader* WR = nullptr;
	
	if (argc >= 2)
	{
		path = argv[1];
		WR = CheckPath(WR, path);
	}
	else
		WR = EnterPath(WR, path);

	UIWriter UI = UIWriter();
	UI.Print();

	bool IsExit = false;
	int second = 0;
	while (!IsExit) {
		if (second >= WR->LenInSec)
			second = WR->LenInSec - 1;
		else if (second < 0)
			second = 0;
		else
			UI.DrowGraf(WR->GetDataFrequency(second).GetNormData(), WR->GetHeader(), second +1, WR->LenInSec);

		char inpSym = _getch();
		if (inpSym == -32)
		{
			inpSym = _getch();
			if(inpSym == 77)
			{
				second++;
				UI.HighlightUI(RightButt);
				Sleep(25);
			}
			if(inpSym == 75)
			{
				second--;
				UI.HighlightUI(LeftButt);
				Sleep(25);
			}
		}
		if (inpSym == '\b')
		{
			system("cls");
			second = 0;
			WR = EnterPath(WR, path);
		}
		if (inpSym == 27)
			IsExit = true;
	}
	system("cls");
}

WavReader* EnterPath(WavReader* WR, string& path) 
{
	bool successReading = false;
	while (!successReading)
	{
		cout << "Enter path to wav-file: ";
		cin >> path;
		if (path == "def")
			path = "C:\\Users\\hotki\\Downloads\\0781.wav";		
		if (path == "def2")
			path = "C:\\Users\\hotki\\Downloads\\0783.wav";
		try
		{
			WR = new WavReader(path);
		}
		catch (const char* e) {
			cout << e << endl;
			continue;
		}
		successReading = true;
	}
	return WR;
}

WavReader* CheckPath(WavReader* WR, string& path)
{
	try
	{
		WR = new WavReader(path);
	}
	catch (const char* e) {
		EnterPath(WR, path);
	}
	return WR;
}
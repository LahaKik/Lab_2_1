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

	UIWriter ui = UIWriter();
	ui.print();

	bool IsExit = false;
	int second = 0;
	while (!IsExit) {
		if (second >= WR->lenInSec)
			second = WR->lenInSec - 1;
		else if (second < 0)
			second = 0;
		else
			ui.DrowGraf(WR->GetDataFrequency(second).GetNormData(), WR->GetHeader(), second +1, WR->lenInSec);

		char ch = _getch();
		if (ch == -32)
		{
			ch = _getch();
			if(ch == 77)
			{
				second++;
				ui.HighlightUI(RightButt);
				Sleep(25);
			}
			if(ch == 75)
			{
				second--;
				ui.HighlightUI(LeftButt);
				Sleep(25);
			}
		}
		if (ch == '\b')
		{
			system("cls");
			second = 0;
			WR = EnterPath(WR, path);
		}
		if (ch == 27)
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
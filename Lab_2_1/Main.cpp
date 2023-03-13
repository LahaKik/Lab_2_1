#include <iostream>
#include <conio.h>
#include <string>
#include "WavReader.h"
#include "UIWriter.h"
using namespace std;

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");
	string path = argv[1];
	WavReader WR = WavReader(path);

	UIWriter ui = UIWriter();
	ui.print();
	bool k = false;
	int i = 0;
	while (!k) {
		if (i >= WR.lenInSec)
			i = WR.lenInSec - 1;
		else if (i < 0)
			i = 0;
		else
			ui.DrowGraf(WR.GetDataFrequency(i).GetNormData(),WR.GetHeader(), i+1, WR.lenInSec);
		char ch = _getch();
		if (ch == -32)
		{
			ch = _getch();
			if(ch == 77)
			{
				i++;
				ui.HighlightUI(RightButt);
			}
			if(ch == 75)
			{
				i--;
				ui.HighlightUI(LeftButt);
			}
		}

		if (ch == 27)
			k = true;

	}
	system("cls");
}

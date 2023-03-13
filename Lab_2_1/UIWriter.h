#pragma once
#include <windows.h>
#include <stdio.h>

using namespace std;

void gotoxy(int xpos, int ypos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = xpos; scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput, scrn);
}

enum UIPart {
    LeftButt,
    RightButt
};

class UIWriter
{
private:
    //settings
    short width = 120;
    short height = 33;

    HANDLE tScr = GetStdHandle(STD_OUTPUT_HANDLE);
    CHAR_INFO* scrBuff = new _CHAR_INFO[width*height];
    CHAR_INFO* bordBuff = new _CHAR_INFO[width*height];

    SMALL_RECT srctReadRect;
    SMALL_RECT srctWriteRect;
    COORD coordBufSize;
    COORD coordBufCoord;

    void drowHeader(WavHeader header) {
        
        AddText(2, 1, "Number of channels - " + to_string(header.numChannels), scrBuff);
        AddText(2, 2, "Sample rate - " + to_string(header.sampleRate) + "bits/sec", scrBuff);
        AddText(2, 3, "Sound accuracy - " + to_string(header.bitsPerSample) + " bit", scrBuff);
    }

    void AddHorizontalLine(int targX, int startY, CHAR_INFO ch, int len, CHAR_INFO* buff) {
        int shift = targX * 120;
        for (short i = 0; i < len; i++)
        {
            *(buff + shift + i + startY) = ch;
        }
    }      
    void AddVerticalLine(int targY, int startX, CHAR_INFO ch, int len, CHAR_INFO* buff) {
        for (short i = 0; i < len; i++)
        {
            *(buff + (i + startX) * width + targY) = ch;
        }
    }
    void AddText(int Y, int X, string str, CHAR_INFO* buff) {
        int shift = X * 120;
        for (short i = 0; i < str.length(); i++)
        {
            CHAR_INFO ch;
            ch.Char.UnicodeChar = str[i];
            ch.Attributes = 0x0007;
            *(buff + shift + Y + i) = ch;
        }
    }
    void AddHighlightedText(int Y, int X, string str, CHAR_INFO* buff) {
        int shift = X * 120;
        for (short i = 0; i < str.length(); i++)
        {
            CHAR_INFO ch;
            ch.Char.UnicodeChar = str[i];
            ch.Attributes = 0x0010 + 0x0020 + 0x0040;
            *(buff + shift + Y + i) = ch;
        }
    }
    void DrowBorder(CHAR_INFO* buff)
    {
        CHAR_INFO bordSym;
        bordSym.Attributes = 0x0010 + 0x0040;
        bordSym.Char.UnicodeChar = ' ';
        AddHorizontalLine(0, 0, bordSym, width, buff);
        AddVerticalLine(0, 0, bordSym, height, buff);
        AddVerticalLine(1, 0, bordSym, height, buff);
        AddHorizontalLine(height - 1, 0, bordSym, width, buff);
        AddHorizontalLine(5, 0, bordSym, width, buff);
        AddVerticalLine(width - 1, 0, bordSym, height, buff);
        AddVerticalLine(width - 2, 0, bordSym, height, buff);
        AddHorizontalLine(height - 3, 0, bordSym, width, buff);
    }
    void defineEmpty(CHAR_INFO* buff) {
        srctReadRect.Top = 0;
        srctReadRect.Left = 0;
        srctReadRect.Bottom = height - 1;
        srctReadRect.Right = width - 1;



        coordBufSize.Y = height;
        coordBufSize.X = width;



        coordBufCoord.X = 0;
        coordBufCoord.Y = 0;
        ReadConsoleOutput(
            tScr,
            buff,
            coordBufSize,
            coordBufCoord,
            &srctReadRect);
    }


public:
    UIWriter()
    {
        HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { width, height };
        SetConsoleScreenBufferSize(hWnd, bufferSize);

        system("mode con cols=120 lines=33");

        defineEmpty(bordBuff);
        defineEmpty(scrBuff);
        DrowBorder(bordBuff);
        memcpy(scrBuff, bordBuff, width * height * 4);
        //AddText(1, 1, "//Header");
    }

    ~UIWriter() {
        delete[] scrBuff;
        delete[] bordBuff;
    }

    void print(CHAR_INFO* buff) {
        srctReadRect.Top = 0;
        srctReadRect.Left = 0;
        srctReadRect.Bottom = height-1;
        srctReadRect.Right = width-1;



        coordBufSize.Y = height;
        coordBufSize.X = width;



        coordBufCoord.X = 0;
        coordBufCoord.Y = 0;
        WriteConsoleOutput(
            tScr,
            buff,
            coordBufSize,
            coordBufCoord,
            &srctReadRect);
    }    
    void print() {
        srctReadRect.Top = 0;
        srctReadRect.Left = 0;
        srctReadRect.Bottom = height-1;
        srctReadRect.Right = width-1;



        coordBufSize.Y = height;
        coordBufSize.X = width;



        coordBufCoord.X = 0;
        coordBufCoord.Y = 0;
        WriteConsoleOutput(
            tScr,
            scrBuff,
            coordBufSize,
            coordBufCoord,
            &srctReadRect);
    }

    void DrowGraf(float* Data, WavHeader header, int tSec, int lenInSec) {  
        memcpy(scrBuff, bordBuff, width * height * 4);

        drowHeader(header);

        AddText(width / 2 - 4, 7, to_string(tSec) + '/' + to_string(lenInSec), scrBuff);

        char offset = 5;
        AddText(offset + width / 9 * 0, 27, "50Hz", scrBuff);
        AddText(offset + width / 9 * 1, 27, "100Hz", scrBuff);
        AddText(offset + width / 9 * 2, 27, "200Hz", scrBuff);
        AddText(offset + width / 9 * 3, 27, "500Hz", scrBuff);
        AddText(offset + width / 9 * 4, 27, "1kz", scrBuff);
        AddText(offset + width / 9 * 5, 27, "2kz", scrBuff);
        AddText(offset + width / 9 * 6, 27, "5kz", scrBuff);
        AddText(offset + width / 9 * 7, 27, "10kz", scrBuff);
        AddText(offset + width / 9 * 8, 27, "20kz", scrBuff);

        AddText(width - 4, height - 2, "l>", scrBuff);
        AddText(width / 2 - 6, height - 2, "ESC - exit", scrBuff);
        AddText(1, height - 2, "<k", scrBuff);

        AddText(offset + width / 9 * 0, 9, to_string((int)round(*(Data + 0) * 100)) + '%', scrBuff);
        AddText(offset + width / 9 * 1, 9, to_string((int)round(*(Data + 1) * 100)) + '%', scrBuff);
        AddText(offset + width / 9 * 2, 9, to_string((int)round(*(Data + 2) * 100)) + '%', scrBuff);
        AddText(offset + width / 9 * 3, 9, to_string((int)round(*(Data + 3) * 100)) + '%', scrBuff);
        AddText(offset + width / 9 * 4, 9, to_string((int)round(*(Data + 4) * 100)) + '%', scrBuff);
        AddText(offset + width / 9 * 5, 9, to_string((int)round(*(Data + 5) * 100)) + '%', scrBuff);
        AddText(offset + width / 9 * 6, 9, to_string((int)round(*(Data + 6) * 100)) + '%', scrBuff);
        AddText(offset + width / 9 * 7, 9, to_string((int)round(*(Data + 7) * 100)) + '%', scrBuff);
        AddText(offset + width / 9 * 8, 9, to_string((int)round(*(Data + 8) * 100)) + '%', scrBuff);

        CHAR_INFO grafSym;
        grafSym.Char.UnicodeChar = ' ';
        grafSym.Attributes = 0x0010 + 0x0020 + 0x0040;

        char verticalLen = int(*(Data + 0) * 18);
        AddVerticalLine(offset + (width / 9) * 0 + 1, 27 - verticalLen, grafSym, verticalLen, scrBuff);
        AddVerticalLine(offset + (width / 9) * 0 + 2, 27 - verticalLen, grafSym, verticalLen, scrBuff);

        verticalLen = int(*(Data + 1) * 18);
        AddVerticalLine(offset + (width / 9) * 1 + 1, 27 - verticalLen, grafSym, verticalLen, scrBuff);
        AddVerticalLine(offset + (width / 9) * 1 + 2, 27 - verticalLen, grafSym, verticalLen, scrBuff);

        verticalLen = int(*(Data + 2) * 18);
        AddVerticalLine(offset + (width / 9) * 2 + 1, 27 - verticalLen, grafSym, verticalLen, scrBuff);
        AddVerticalLine(offset + (width / 9) * 2 + 2, 27 - verticalLen, grafSym, verticalLen, scrBuff);

        verticalLen = int(*(Data + 3) * 18);
        AddVerticalLine(offset + (width / 9) * 3 + 1, 27 - verticalLen, grafSym, verticalLen, scrBuff);
        AddVerticalLine(offset + (width / 9) * 3 + 2, 27 - verticalLen, grafSym, verticalLen, scrBuff);

        verticalLen = int(*(Data + 4) * 18);
        AddVerticalLine(offset + (width / 9) * 4 + 1, 27 - verticalLen, grafSym, verticalLen, scrBuff);
        AddVerticalLine(offset + (width / 9) * 4 + 2, 27 - verticalLen, grafSym, verticalLen, scrBuff);

        verticalLen = int(*(Data + 5) * 18);
        AddVerticalLine(offset + (width / 9) * 5 + 1, 27 - verticalLen, grafSym, verticalLen, scrBuff);
        AddVerticalLine(offset + (width / 9) * 5 + 2, 27 - verticalLen, grafSym, verticalLen, scrBuff);

        verticalLen = int(*(Data + 6) * 18);
        AddVerticalLine(offset + (width / 9) * 6 + 1, 27 - verticalLen, grafSym, verticalLen, scrBuff);
        AddVerticalLine(offset + (width / 9) * 6 + 2, 27 - verticalLen, grafSym, verticalLen, scrBuff);

        verticalLen = int(*(Data + 7) * 18);
        AddVerticalLine(offset + (width / 9) * 7 + 1, 27 - verticalLen, grafSym, verticalLen, scrBuff);
        AddVerticalLine(offset + (width / 9) * 7 + 2, 27 - verticalLen, grafSym, verticalLen, scrBuff);

        verticalLen = int(*(Data + 8) * 18);
        AddVerticalLine(offset + (width / 9) * 8 + 1, 27 - verticalLen, grafSym, verticalLen, scrBuff);
        AddVerticalLine(offset + (width / 9) * 8 + 2, 27 - verticalLen, grafSym, verticalLen, scrBuff);


        print(scrBuff);
    }
    
    void HighlightUI(UIPart part) {
        switch (part)
        {
        case LeftButt:
            AddHighlightedText(1, height - 2, "<k", scrBuff);
            break;
        case RightButt:
            AddHighlightedText(width - 4, height - 2, "l>", scrBuff);
            break;
        default:
            break;
        }
        print();
    }
};






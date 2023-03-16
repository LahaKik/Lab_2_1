#pragma once
#include <windows.h>
#include <stdio.h>

using namespace std;

enum UIPart {
    LeftButt,
    RightButt
};

class UIWriter
{
private:
    //settings
    short width = 120;
    short height = 41;
    short numSegmentsGraph = 23;

    HANDLE tScr = GetStdHandle(STD_OUTPUT_HANDLE);
    CHAR_INFO* scrBuff = new _CHAR_INFO[width*height];
    CHAR_INFO* bordBuff = new _CHAR_INFO[width*height];

    SMALL_RECT srctReadRect;
    SMALL_RECT srctWriteRect;
    COORD coordBufSize;
    COORD coordBufCoord;

    void drowHeader(WavHeader header) 
    {        
        addText(2, 1, "Number of channels - " + to_string(header.numChannels), scrBuff);
        addText(2, 2, "Sample rate - " + to_string(header.sampleRate) + " bits/sec", scrBuff);
        addText(2, 3, "Sound accuracy - " + to_string(header.bitsPerSample) + " bit", scrBuff);
    }

    void addHorizontalLine(int targX, int startY, CHAR_INFO ch, int len, CHAR_INFO* buff) 
    {
        int shift = targX * 120;
        for (short i = 0; i < len; i++)
        {
            *(buff + shift + i + startY) = ch;
        }
    }  

    void addVerticalLine(int targY, int startX, CHAR_INFO ch, int len, CHAR_INFO* buff) 
    {
        for (short i = 0; i < len; i++)
        {
            *(buff + (i + startX) * width + targY) = ch;
        }
    }

    void addText(int Y, int X, string str, CHAR_INFO* buff) 
    {
        int shift = X * 120;
        for (short i = 0; i < str.length(); i++)
        {
            CHAR_INFO ch;
            ch.Char.UnicodeChar = str[i];
            ch.Attributes = 0x0007;
            *(buff + shift + Y + i) = ch;
        }
    }

    void addHighlightedText(int Y, int X, string str, CHAR_INFO* buff) 
    {
        int shift = X * 120;
        for (short i = 0; i < str.length(); i++)
        {
            CHAR_INFO ch;
            ch.Char.UnicodeChar = str[i];
            ch.Attributes = 0x0010 + 0x0020 + 0x0040;
            *(buff + shift + Y + i) = ch;
        }
    }

    void drowBorder(CHAR_INFO* buff)
    {
        CHAR_INFO bordSym;
        bordSym.Attributes = 0x0010 + 0x0040;
        bordSym.Char.UnicodeChar = ' ';
        addHorizontalLine(0, 0, bordSym, width, buff);
        addVerticalLine(0, 0, bordSym, height, buff);
        addVerticalLine(1, 0, bordSym, height, buff);
        addHorizontalLine(height - 1, 0, bordSym, width, buff);
        addHorizontalLine(5, 0, bordSym, width, buff);
        addVerticalLine(width - 1, 0, bordSym, height, buff);
        addVerticalLine(width - 2, 0, bordSym, height, buff);
        addHorizontalLine(height - 3, 0, bordSym, width, buff);
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

    void drowNamesFreq(char offset)
    {
        addText(offset + width / 9 * 0, height - 5, "50Hz", scrBuff);
        addText(offset + width / 9 * 1, height - 5, "100Hz", scrBuff);
        addText(offset + width / 9 * 2, height - 5, "200Hz", scrBuff);
        addText(offset + width / 9 * 3, height - 5, "500Hz", scrBuff);
        addText(offset + width / 9 * 4, height - 5, "1kz", scrBuff);
        addText(offset + width / 9 * 5, height - 5, "2kz", scrBuff);
        addText(offset + width / 9 * 6, height - 5, "5kz", scrBuff);
        addText(offset + width / 9 * 7, height - 5, "10kz", scrBuff);
        addText(offset + width / 9 * 8, height - 5, "20kz", scrBuff);
    }

    void drowMenu()
    {
        addText(width - 4, height - 2, "->", scrBuff);
        addText(width / 3 - 6, height - 2, "ESC - exit", scrBuff);
        addText((width / 3) * 2 - 10, height - 2, "BKSP - Change file", scrBuff);
        addText(1, height - 2, "<-", scrBuff);
    }

public:
    UIWriter()
    {
        HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD bufferSize = { width, height };
        SetConsoleScreenBufferSize(hWnd, bufferSize);
        system("mode con cols=120 lines=41");

        defineEmpty(bordBuff);
        defineEmpty(scrBuff);
        drowBorder(bordBuff);
        memcpy(scrBuff, bordBuff, width * height * 4);
    }

    ~UIWriter() 
    {
        delete[] scrBuff;
        delete[] bordBuff;
    }

    void Print(CHAR_INFO* buff) 
    {
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

    void Print() 
    {
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

    void DrowGraf(float* Data, WavHeader header, int tSec, int LenInSec)
    {  
        memcpy(scrBuff, bordBuff, width * height * 4);

        drowHeader(header);

        addText(width / 2 - 4, 7, to_string(tSec) + '/' + to_string(LenInSec), scrBuff);

        char offset = 5;
        drowNamesFreq(offset);
        drowMenu();
        for (short i = 0; i < 9; i++)
        {
            addText(offset + width / 9 * i, 9, to_string((int)round(*(Data + i) * 100)) + '%', scrBuff);
        }

        CHAR_INFO grafSym;
        grafSym.Char.UnicodeChar = ' ';
        grafSym.Attributes = 0x0010 + 0x0020 + 0x0040;
        for (short i = 0; i < 9; i++)
        {
            char verticalLen = int(*(Data + i) * numSegmentsGraph);
            addVerticalLine(offset + (width / 9) * i + 1, height - 7 - verticalLen, grafSym, verticalLen, scrBuff);
            addVerticalLine(offset + (width / 9) * i + 2, height - 7 - verticalLen, grafSym, verticalLen, scrBuff);
        }

        Print(scrBuff);
    }
    
    void HighlightUI(UIPart part) 
    {
        switch (part)
        {
        case LeftButt:
            addHighlightedText(1, height - 2, "<-", scrBuff);
            break;
        case RightButt:
            addHighlightedText(width - 4, height - 2, "->", scrBuff);
            break;
        default:
            break;
        }
        Print();
    }
};








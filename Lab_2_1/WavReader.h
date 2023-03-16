#pragma once
#pragma warning(disable : 4996) 
#include <string>
#include <iostream>
#include <fstream>
#include "Graph.h"
#include "WavHeader.h"
#include <stdexcept>



using namespace std;

class WavReader
{
private:
	FILE* f;
	WavHeader header;
	short* leftCh = nullptr, * rightCh = nullptr;
	
	
public:
	int lenInSec;

	WavReader(string path)
	{
		f = fopen(path.c_str(), "rb");
		if (!f)
			throw "Can't open this file\nCheck correctness of the path";

		header.ReadHeader(f);

		lenInSec = header.subchunk2Size / header.byteRate;

		rightCh = new short[header.subchunk2Size/4];
		leftCh = new short[header.subchunk2Size/4];

		for (int i = 0; i < header.subchunk2Size/4; i++)
		{
			fread(leftCh+i, sizeof(short), 1, f);
			fread(rightCh+i, sizeof(short), 1, f);
		}
		
	}

	void PrintHeaderDebug() 
	{
		cout << "chunkId: "<< hex << header.chunkId << endl << dec
			<< "chunkSize: " << header.chunkSize << endl
			<< "format: " << hex << header.format << endl
			<< "subchunk1Id: " << header.subchunk1Id << endl << dec
			<< "subchunk1Size: " << header.subchunk1Size << endl
			<< "audioFormat: " << header.audioFormat << endl
			<< "numChannels: " << header.numChannels << endl
			<< "sampleRate: " << header.sampleRate << endl
			<< "byteRate: " << header.byteRate << endl
			<< "blockAlign: " << header.blockAlign << endl
			<< "bitsPerSample: " << header.bitsPerSample << endl
			<< "subchunk2Id: " << hex << header.subchunk2Id << endl << dec
			<< "subchunk2Size: " << header.subchunk2Size << endl;
	}

	WavHeader GetHeader() {
		return header;
	}

	void PrintData(int secondsFromStart) 
	{
		for (int i = 0; i < header.byteRate/4; i++)
		{
			int shift = (secondsFromStart * header.byteRate);
			cout << *((leftCh + i) + shift) << "\t" << *((rightCh + i) + shift) << endl;
		}
		
	}	

	Graph GetDataFrequency(int secondsFromStart) 
	{
		//cout << "Анализ промежутка..." << endl;
		int shift = (secondsFromStart * header.byteRate/4);
		int rangeFrequency[9] = { 0 };//[0] - 50Hz, [1] - 100Hz, [2] - 200Hz, [3] - 500Hz, [4] - 1kHz, 
									  //[5] - 2kHz, [6] - 5kHz, [7] - 10kHz, [8] - 20kHz
		for (int i = 0; i < header.byteRate / 4; i++)
		{
			int temp = *(leftCh + i + shift);
			if (abs(temp) == 50)
				rangeFrequency[0]++;
			if (abs(temp) == 100)
				rangeFrequency[1]++;
			if (abs(temp) == 200)
				rangeFrequency[2]++;
			if (abs(temp) == 500)
				rangeFrequency[3]++;
			if (abs(temp) == 1000)
				rangeFrequency[4]++;
			if (abs(temp) == 2000)
				rangeFrequency[5]++;
			if (abs(temp) == 5000)
				rangeFrequency[6]++;
			if (abs(temp) == 10000)
				rangeFrequency[7]++;
			if (abs(temp) == 20000)
				rangeFrequency[8]++;

			temp = *(rightCh + i + shift);
			if (abs(temp) == 50)
				rangeFrequency[0]++;
			if (abs(temp) == 100)
				rangeFrequency[1]++;
			if (abs(temp) == 200)
				rangeFrequency[2]++;
			if (abs(temp) == 500)
				rangeFrequency[3]++;
			if (abs(temp) == 1000)
				rangeFrequency[4]++;
			if (abs(temp) == 2000)
				rangeFrequency[5]++;
			if (abs(temp) == 5000)
				rangeFrequency[6]++;
			if (abs(temp) == 10000)
				rangeFrequency[7]++;
			if (abs(temp) == 20000)
				rangeFrequency[8]++;
		}
		
		Graph graf = Graph(rangeFrequency);
		return graf;
	}

	void Close() {
		fcloseall();
	}

	~WavReader() {
		delete[] leftCh;
		delete[] rightCh;
		Close();
	}
};


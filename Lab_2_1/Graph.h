#pragma once
class Graph
{
private:
	int* freqData;
	float* normFreqData = new float[9];

	void normalize() {
		int sum = 0;
		for (char i = 0; i < 9; i++)
		{
			sum += *(freqData + i);
		}
		for (char i = 0; i < 9; i++)
		{
			*(normFreqData + i) = *(freqData + i) / float(sum);
		}
	}

public:

	Graph(int* Data) 
	{
		freqData = Data;
		normalize();
	}

	~Graph() 
	{
		delete normFreqData;
	}
	
	float* GetNormData() 
	{
		return normFreqData;
	}
};
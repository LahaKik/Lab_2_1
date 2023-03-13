#pragma once
class Graf
{
private:
	int* FreqData;
	float* NormFreqData = new float[9];


	void Normalize() {
		int sum = 0;
		for (char i = 0; i < 9; i++)
		{
			sum += *(FreqData + i);
		}
		for (char i = 0; i < 9; i++)
		{
			*(NormFreqData + i) = *(FreqData + i) / float(sum);
		}
	}


public:

	Graf(int* Data) {
		FreqData = Data;
		Normalize();
	}

	~Graf() {
		delete NormFreqData;
	}
	
	float* GetNormData() {
		return NormFreqData;
	}
};


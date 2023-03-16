#pragma once
struct WavHeader
{
	long chunkId, chunkSize, format, subchunk1Id, subchunk1Size, sampleRate, byteRate, subchunk2Id, subchunk2Size;
	short audioFormat, numChannels, blockAlign, bitsPerSample;

	void ReadHeader(FILE* f) 
	{
		fread(&chunkId, sizeof(long), 1, f);
		fread(&chunkSize, sizeof(long), 1, f);
		fread(&format, sizeof(long), 1, f);
		if (!(format == 0x57415645 || format == 0x45564157))
			throw "It's not wav-file";
		fread(&subchunk1Id, sizeof(long), 1, f);
		fread(&subchunk1Size, sizeof(long), 1, f);
		fread(&audioFormat, sizeof(short), 1, f);
		fread(&numChannels, sizeof(short), 1, f);
		fread(&sampleRate, sizeof(long), 1, f);
		fread(&byteRate, sizeof(long), 1, f);
		fread(&blockAlign, sizeof(short), 1, f);
		fread(&bitsPerSample, sizeof(short), 1, f);

		short temp;
		fread(&temp, sizeof(short), 1, f);
		bool symDATA = false;
		while (!symDATA)
		{
			while (!(temp == 0x6461 || temp == 0x6164)) {
				fread(&temp, sizeof(short), 1, f);
			}
			fread(&temp, sizeof(short), 1, f);
			if (temp == 0x7461 || temp == 0x6174)
			{
				symDATA = true;
				subchunk2Id = 0x61646174;
			}
		}

		fread(&subchunk2Size, sizeof(long), 1, f);
	}
};


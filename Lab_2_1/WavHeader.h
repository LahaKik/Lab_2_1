#pragma once
struct WavHeader
{
	long chunkId, chunkSize, format, subchunk1Id, subchunk1Size, sampleRate, byteRate, subchunk2Id, subchunk2Size;
	short audioFormat, numChannels, blockAlign, bitsPerSample;
};


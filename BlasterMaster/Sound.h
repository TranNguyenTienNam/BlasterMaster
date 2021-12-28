#pragma once
#include <dsound.h>
#include "Service.h"
#include "Utils.h"

class CSound : public CService
{
private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer;

	bool InitializeDirectSound(HWND hWnd);
	void ShutdownDirectSound();

	bool LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer);
	void ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer);

	bool PlayWaveFile();
public:
	CSound();
	CSound(const CSound&);
	~CSound();

	bool Initialize(HWND);
	void Shutdown();
};


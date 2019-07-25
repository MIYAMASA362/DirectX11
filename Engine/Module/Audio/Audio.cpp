#include<assert.h>
#include<string>
#include<memory>
#include<map>
#include<Windows.h>
#include<xaudio2.h>

#include"Audio.h"
#include"AudioManager.h"

#pragma comment (lib, "xaudio2.lib")

using namespace DirectX;

DirectX::Audio::Audio()
{

}

DirectX::Audio::~Audio()
{
	for(int j = 0; j<SOUND_SOURCE_MAX; j++)
		if(SourceVoice[j])
		{
			SourceVoice[j]->Stop();
			SourceVoice[j]->DestroyVoice();
			SourceVoice[j] = nullptr;
		}

	if(SoundData)
	{
		delete[] SoundData;
		SoundData = nullptr;
	}
}

void DirectX::Audio::Play(bool loop)
{
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
	{
		XAUDIO2_VOICE_STATE state;

		SourceVoice[j]->GetState(&state);

		if (state.BuffersQueued == 0)
		{
			//Sound->SourceVoice->Stop();
			//Sound->SourceVoice->FlushSourceBuffers();

			// バッファ設定
			XAUDIO2_BUFFER bufinfo;

			memset(&bufinfo, 0x00, sizeof(bufinfo));
			bufinfo.AudioBytes = length;
			bufinfo.pAudioData = SoundData;
			bufinfo.PlayBegin = 0;
			bufinfo.PlayLength = PlayLength;

			// ループ設定
			if (loop)
			{
				bufinfo.LoopBegin = 0;
				bufinfo.LoopLength = PlayLength;
				bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
			}

			SourceVoice[j]->SubmitSourceBuffer(&bufinfo, NULL);

			// 再生
			SourceVoice[j]->Start();

			break;
		}
	}
}

void DirectX::Audio::Stop()
{
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
		SourceVoice[j]->Stop();
}

void DirectX::Audio::GetAsset(std::string name)
{
	Audio* asset = AudioManager::GetAudio(name).lock().get();
	*this = *asset;
}

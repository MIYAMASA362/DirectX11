#pragma once

const int SOUND_SOURCE_MAX = 10;

struct IXAudio2SourceVoice;
struct IXAudio2;
struct IXAudio2MasteringVoice;

namespace DirectX
{
	class Audio;

	//Audio
	class Audio final
	{
		friend class AudioManager;
	private:
		IXAudio2SourceVoice* SourceVoice[SOUND_SOURCE_MAX];
		BYTE* SoundData;

		int length;
		int PlayLength;
	public:
		Audio();
		~Audio();
	public:
		void Play(bool loop = false);
		void Stop();
		void GetAsset(std::string name);
	};
};
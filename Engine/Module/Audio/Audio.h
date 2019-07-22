#pragma once

const int SOUND_SOURCE_MAX = 10;

struct IXAudio2SourceVoice;
struct IXAudio2;
struct IXAudio2MasteringVoice;

namespace DirectX
{
	class Audio;

	//Audioéëåπ
	class AudioAsset
	{
	public:
		AudioAsset(std::string path)
			:path(path) {};
	public:
		std::string path;
		IXAudio2SourceVoice* sourceVoice;
	};

	//Audioä«óù
	class AudioManager final
	{
	private:
		static AudioManager* pInstance;
		//Audioéëåπå…
		std::map<std::string, AudioAsset> AudioDictionary;

		IXAudio2* pXAudio = nullptr;
		IXAudio2MasteringVoice* pMasteringVoice = nullptr;
	private:
		AudioManager();
		~AudioManager();
	public:
		static void Create();
		static void Destroy();
	public:
		static Audio* LoadAudio(const char* FileName);
		static HRESULT SetAudioAsset(std::string AudioName,AudioAsset asset);
	};

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
	};
};
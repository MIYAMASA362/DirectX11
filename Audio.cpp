#include "Audio.h"

AudioManager* AudioManager::pInstance = nullptr;

DirectX::AudioManager::AudioManager()
{
	//エラーメッセージ
	auto Error = [](HRESULT hr,std::string message) {
		if(FAILED(hr)){
			MessageBox(NULL,message.c_str(),"XAudio",MB_OK);
			return true;
		}
		return false;
	};

	HRESULT hr;

	//COM(Component Object Model)
	//Remarks:http://e-words.jp/w/COM.html
	CoInitializeEx(NULL,COINIT_MULTITHREADED);

	//XAudioを生成
	hr = XAudio2Create(&pXAudio,NULL);

	if (Error(hr, "XAudio2の生成に失敗しました。"))return;

	//マスタリングボイス生成
	hr = pXAudio->CreateMasteringVoice(&pMasteringVoice);

	if (Error(hr, "マスタリングボイス生成に失敗しました。"))return;
}

DirectX::AudioManager::~AudioManager()
{
	AudioDictionary.clear();

	if(pMasteringVoice != nullptr){
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}

	if (pXAudio != nullptr){
		pXAudio->Release();
		pXAudio = nullptr;
	}

	CoUninitialize();
}

void DirectX::AudioManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new AudioManager();
}

void DirectX::AudioManager::Destroy()
{
	if (pInstance == nullptr) return;
	delete pInstance;
}

Audio* DirectX::AudioManager::LoadAudio(const char * FileName)
{
	Audio* audio = new Audio();

	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;

		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		audio->SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)audio->SoundData, buflen);


		audio->length = readlen;
		audio->PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// サウンドソース生成
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
	{
		pInstance->pXAudio->CreateSourceVoice(&audio->SourceVoice[j], &wfx);
	}

	return audio;
}

HRESULT DirectX::AudioManager::SetAudioAsset(std::string AudioName, AudioAsset asset)
{
	//インスタンス生成前
	if (pInstance == nullptr) return E_FAIL;



	return S_OK;
}


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
			SourceVoice[j] = NULL;
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

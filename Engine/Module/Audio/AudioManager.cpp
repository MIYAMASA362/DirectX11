#include<Windows.h>
#include<string>
#include<memory>
#include<map>
#include<assert.h>
#include<xaudio2.h>

#include"Audio.h"
#include"AudioManager.h"

const std::string AudioManager::AssetDataBase = "Asset/Sound/";
std::map<std::string,std::shared_ptr<Audio>> AudioManager::AudioIndex;

IXAudio2* AudioManager::pXAudio = nullptr;
IXAudio2MasteringVoice* AudioManager::pMasteringVoice = nullptr;

//Asset�ǂݍ���
void AudioManager::LoadAsset(AudioAsset asset)
{
	Audio* audio = new Audio();

	audio = LoadAudio((AssetDataBase+asset.path).c_str());

	AudioIndex.emplace(asset.name,std::shared_ptr<Audio>(audio));
}

//Audio�f�[�^�ǂݍ���
Audio* AudioManager::LoadAudio(const char * FileName)
{
	Audio* audio = new Audio();

	// �T�E���h�f�[�^�Ǎ�
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

	// �T�E���h�\�[�X����
	for (int j = 0; j < SOUND_SOURCE_MAX; j++)
	{
		pXAudio->CreateSourceVoice(&audio->SourceVoice[j], &wfx);
	}

	return audio;
}

std::weak_ptr<Audio> AudioManager::GetAudio(std::string name)
{
	return AudioIndex[name];
}

void AudioManager::CreateDevice()
{
	//�G���[���b�Z�[�W
	auto Error = [](HRESULT hr, std::string message) {
		if (FAILED(hr)) {
			MessageBox(NULL, message.c_str(), "XAudio", MB_OK);
			return true;
		}
		return false;
	};

	HRESULT hr;

	//COM(Component Object Model)
	//Remarks:http://e-words.jp/w/COM.html
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//XAudio�𐶐�
	hr = XAudio2Create(&pXAudio, NULL);

	if (Error(hr, "XAudio2�̐����Ɏ��s���܂����B"))return;

	//�}�X�^�����O�{�C�X����
	hr = pXAudio->CreateMasteringVoice(&pMasteringVoice);

	if (Error(hr, "�}�X�^�����O�{�C�X�����Ɏ��s���܂����B"))return;
}

void AudioManager::Release()
{
	AudioIndex.clear();

	if (pMasteringVoice != nullptr) {
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = nullptr;
	}

	if (pXAudio != nullptr) {
		pXAudio->Release();
		pXAudio = nullptr;
	}

	CoUninitialize();
}


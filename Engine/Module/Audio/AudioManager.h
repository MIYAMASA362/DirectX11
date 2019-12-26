#pragma once

//Audio����
class AudioAsset final
{
	friend class AudioManager;
private:
	const std::string name;
	const std::string path;
public:
	AudioAsset(std::string name, std::string path):name(name),path(path) {};
	~AudioAsset() = default;
};

//Audio�Ǘ�
class AudioManager final
{
private:
	static const std::string AssetDataBase;		//Asset�t�H���_�p�X
	static std::map<std::string,std::shared_ptr<Audio>> AudioIndex;	//Audio������

	static IXAudio2* pXAudio;					//Audio�f�o�C�X
	static IXAudio2MasteringVoice* pMasteringVoice;
private:
	AudioManager() = default;
	~AudioManager() = default;
public:
	//Audio�f�o�C�X�̐���
	static void CreateDevice();
	//Audio�f�o�C�X�EAudio�f�[�^�̉��
	static void Release();
	//AudioAsset�ǂݍ���
	static void LoadAsset(AudioAsset asset);
	//Audio�t�@�C���ǂݍ���
	static Audio* LoadAudio(const char* FileName);
	//Audio�擾
	static std::weak_ptr<Audio> GetAudio(std::string name);
};
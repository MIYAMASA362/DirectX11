#pragma once

//Audio資源
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

//Audio管理
class AudioManager final
{
private:
	static const std::string AssetDataBase;		//Assetフォルダパス
	static std::map<std::string,std::shared_ptr<Audio>> AudioIndex;	//Audio資源庫

	static IXAudio2* pXAudio;					//Audioデバイス
	static IXAudio2MasteringVoice* pMasteringVoice;
private:
	AudioManager() = default;
	~AudioManager() = default;
public:
	//Audioデバイスの生成
	static void CreateDevice();
	//Audioデバイス・Audioデータの解放
	static void Release();
	//AudioAsset読み込み
	static void LoadAsset(AudioAsset asset);
	//Audioファイル読み込み
	static Audio* LoadAudio(const char* FileName);
	//Audio取得
	static std::weak_ptr<Audio> GetAudio(std::string name);
};
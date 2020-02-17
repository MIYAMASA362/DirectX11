#pragma once

class Audio;

class AudioSource final :public Component<AudioSource>
{
private:
	std::weak_ptr<Audio>audio;
	bool IsLoop = true;
public:
	AudioSource();
	~AudioSource();
public:
	void GetAsset(std::string name);
	void Play();
	void SetLoop(bool loop);
	void Stop();
	void Release() override;
};
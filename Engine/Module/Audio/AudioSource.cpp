#include"Module\Module.h"

#include "AudioSource.h"

DirectX::AudioSource::AudioSource()
:
	Component("AudioSource")
{

}

DirectX::AudioSource::~AudioSource()
{
	audio.lock()->Stop();
	audio.reset();
}

void DirectX::AudioSource::GetAsset(std::string name)
{
	this->audio = AudioManager::GetAudio(name);
}

void DirectX::AudioSource::Play()
{
	audio.lock()->Play(IsLoop);
}

void DirectX::AudioSource::SetLoop(bool loop)
{
	IsLoop = loop;
}

void DirectX::AudioSource::Stop()
{
	audio.lock()->Stop();
}

void DirectX::AudioSource::OnDestroy()
{
	audio.lock()->Stop();
}



#include"Common.h"
#include"Module\ECSEngine.h"

#include"Audio.h"
#include"AudioManager.h"
#include"AudioSource.h"

AudioSource::AudioSource(EntityID OwnerID)
:
	Component(OwnerID)
{

}

AudioSource::~AudioSource()
{
	audio.lock()->Stop();
	audio.reset();
}

void AudioSource::GetAsset(std::string name)
{
	this->audio = AudioManager::GetAudio(name);
}

void AudioSource::Play()
{
	audio.lock()->Play(IsLoop);
}

void AudioSource::SetLoop(bool loop)
{
	IsLoop = loop;
}

void AudioSource::Stop()
{
	audio.lock()->Stop();
}

void AudioSource::OnDestroy()
{
	audio.lock()->Stop();
}



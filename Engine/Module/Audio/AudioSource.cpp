#include"Common.h"
#include"Module\ECSEngine.h"

#include"Module\AssetData\AssetData.h"
#include"Audio.h"
#include"AudioManager.h"
#include"AudioSource.h"

using namespace DirectX;

std::unordered_map<EntityID, std::weak_ptr<AudioSource>> AudioSource::ComponentIndex;

DirectX::AudioSource::AudioSource(EntityID OwnerID)
:
	Component(OwnerID,"AudioSource")
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



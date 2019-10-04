#include<memory>
#include<Windows.h>
#include<string>
#include<map>

#include"Module\Object\Object.h"
#include"Module\Component\IComponent.h"
#include"Module\Component\ComponentManager.h"
#include"Module\Component\Component.h"

#include"Module\AssetData\AssetData.h"
#include"Audio.h"
#include"AudioManager.h"
#include"AudioSource.h"

std::map<DirectX::EntityID, std::weak_ptr<DirectX::AudioSource>> DirectX::AudioSource::ComponentIndex;

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



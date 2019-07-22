#include<string>
#include "TextureManager.h"

using namespace DirectX;

TextureManager* TextureManager::pInstance = nullptr;

DirectX::TextureManager::TextureManager()
{

}

DirectX::TextureManager::~TextureManager()
{

}

void DirectX::TextureManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new TextureManager();
}

void DirectX::TextureManager::Destroy()
{
	if (pInstance == nullptr) return;
	delete pInstance;
	pInstance = nullptr;
}

DirectX::TextureManager::TextureAsset::TextureAsset(std::string name, std::string path)
:
	name(name),path(path)
{

}

DirectX::TextureManager::TextureAsset::~TextureAsset()
{

}

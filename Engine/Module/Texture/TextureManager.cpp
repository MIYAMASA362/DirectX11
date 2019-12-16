#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<assert.h>
#include<string>
#include<memory>

#include"Module\DirectX\DirectX.h"

#include"Module\AssetData\AssetData.h"

#include"texture.h"
#include"TextureManager.h"

using namespace DirectX;

TextureManager::TextureIndex TextureManager::textureIndex;

//Texture読み込み
std::weak_ptr<Texture> TextureManager::LoadTexture(std::string filePath)
{
	//テクスチャ
	Texture* texture = new Texture();
	texture->LoadTexture(filePath);

	//テクスチャ名
	std::string name;
	name = filePath.substr(filePath.find_last_of("\\/"), filePath.find_last_of("."));

	return textureIndex.emplace(name,std::shared_ptr<Texture>(texture)).first->second;
}

//Assetを解放
void TextureManager::Release()
{
	textureIndex.clear();
}

//テクスチャ取得
std::weak_ptr<Texture> TextureManager::GetTexture(std::string name)
{
	return textureIndex.find(name)->second;
}


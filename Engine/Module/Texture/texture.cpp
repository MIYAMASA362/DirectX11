#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string>
#include<memory>
#include<map>
#include<assert.h>
#include<d3d11.h>
#include<DirectXMath.h>
#include"../DirectX/DirectXStruct.h"
#include"../DirectX/DirectX.h"
#include"texture.h"
#include"TextureManager.h"

using namespace DirectX;

//Destrcutor
DirectX::Texture::~Texture()
{
	texture->Release();
	srv->Release();
}


//TextureManager‚ÌAsset‚©‚çŽæ“¾
void DirectX::Texture::GetAsset(std::string name)
{
	Texture* asset = TextureManager::GetTexture(name).lock().get();
	this->texture = asset->texture;
	this->srv = asset->srv;
}

#pragma once

#include<map>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace DirectX
{
	//Texture管理
	class TextureManager final:public AssetManager
	{
	private:
		static const std::string AssetDataBase;			//Assetフォルダパス
		static std::map<std::string,std::shared_ptr<Texture>> TextureIndex;
	private:
		TextureManager() = delete;
		~TextureManager() = delete;
	public:
		static Texture* LoadTexture(const char* path);
		static void LoadAsset(std::string name,std::string path);
		static void Release();
		static Texture* GetTexture(std::string name);
	};
}
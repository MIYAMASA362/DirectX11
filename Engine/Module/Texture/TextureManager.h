#pragma once

#include<map>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace DirectX
{
	//Texture資源
	class TextureAsset
	{
		friend class TextureManager;
	private:
		const std::string name;		//テクスチャ名
		const std::string path;		//Asset/Textureからのテクスチャパス
	public:
		TextureAsset(std::string name, std::string path);
		~TextureAsset();
	};

	//Texture管理
	class TextureManager final
	{
	private:
		static const std::string AssetDataBase;			//Assetフォルダパス
		static std::map<std::string,std::shared_ptr<Texture>> TextureIndex;
	//--- Method ----------------------------------------------------
	private:
		TextureManager() = default;
		~TextureManager() = default;
	public:
		static Texture* LoadTexture(const char* path);
		static void LoadAsset(TextureAsset asset);
		static void Release();
		static std::weak_ptr<Texture> GetTexture(std::string name);
	};
}
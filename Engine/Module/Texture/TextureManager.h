#pragma once

#include<map>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace DirectX
{
	//Texture����
	class TextureAsset
	{
		friend class TextureManager;
	private:
		const std::string name;		//�e�N�X�`����
		const std::string path;		//Asset/Texture����̃e�N�X�`���p�X
	public:
		TextureAsset(std::string name, std::string path);
		~TextureAsset();
	};

	//Texture�Ǘ�
	class TextureManager final
	{
	private:
		static const std::string AssetDataBase;			//Asset�t�H���_�p�X
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
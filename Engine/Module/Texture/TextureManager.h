#pragma once

namespace DirectX
{
	//Texture管理
	class TextureManager final
	{
	//--- Singleton -------------------------------------------------
	private:
		static TextureManager* pInstance;
	public:
		void Create();
		void Destroy();
	//--- Texture資源 -----------------------------------------------
	public:
		class TextureAsset
		{
		private:
			const std::string name;		//テクスチャ名
			const std::string path;		//Assetのパス
		public:
			TextureAsset(std::string name,std::string path);
			~TextureAsset();
		};
	//--- Attribute -------------------------------------------------
	private:
		//std::map<std::string name,>
	//--- Method ----------------------------------------------------
	private:
		TextureManager();
		~TextureManager();
	public:

	};
}
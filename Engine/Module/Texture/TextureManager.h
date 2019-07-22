#pragma once

namespace DirectX
{
	//Texture�Ǘ�
	class TextureManager final
	{
	//--- Singleton -------------------------------------------------
	private:
		static TextureManager* pInstance;
	public:
		void Create();
		void Destroy();
	//--- Texture���� -----------------------------------------------
	public:
		class TextureAsset
		{
		private:
			const std::string name;		//�e�N�X�`����
			const std::string path;		//Asset�̃p�X
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
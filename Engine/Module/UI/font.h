#pragma once

namespace DirectX
{
	class Texture;

	class Font
	{
	private:
		HFONT m_font;
		Texture* m_texture;
	public:
		Font();
		~Font();
	public:
		void Load(const char* fileName, const char* name);
		Texture* TransformToTexture();
		HFONT GetFont() { return m_font; };
	};

	//テクスチャ上の読み込み位置
	struct FontData
	{
		int m_PosX;
		int m_PoxY;
		int m_Width;
		int m_Height;
	};

	//Fontのリソースデータ
	class FontAsset
	{
	private:
		const char* path;	//保存先
		FontData* onebyte;	//1byte文字
		FontData* twobyte;	//2byte文字
	};
}
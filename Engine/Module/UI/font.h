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

	//�e�N�X�`����̓ǂݍ��݈ʒu
	struct FontData
	{
		int m_PosX;
		int m_PoxY;
		int m_Width;
		int m_Height;
	};

	//Font�̃��\�[�X�f�[�^
	class FontAsset
	{
	private:
		const char* path;	//�ۑ���
		FontData* onebyte;	//1byte����
		FontData* twobyte;	//2byte����
	};
}
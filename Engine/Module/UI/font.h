#pragma once

class Texture;
class D3DRenderer;

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
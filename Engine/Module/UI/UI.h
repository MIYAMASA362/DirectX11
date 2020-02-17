#pragma once

struct ID3D11Buffer;

class Texture;

class UI;
class Font;

//Canvas@2D‰æ–Êã‚Ìî•ñ‚ğŠ‚·‚é
class Canvas:public Component<Canvas>
{
private:
	std::list<std::weak_ptr<UI>> UIIndex;
public:
	Canvas();
};

//UI
class UI:public Component<UI>
{
public:
	UI();
	virtual ~UI() = default;
public:
	virtual void Render(XMMATRIX world) = 0;
	virtual void SendComponentMessage(std::string message) override;
};

//UI Image
class Image:public Renderer
{
public:
	std::weak_ptr<Texture> texture;
	ID3D11Buffer* vertexBuffer = nullptr;
public:
	Image();
	virtual ~Image();
	virtual void Render(XMMATRIX& world) override;
};

//UI Button
class Button:public UI
{
private:
	float m_scale;
	bool m_isHover;
	ID3D11Buffer* m_vertexBuffer = nullptr;
public:
	Texture* texture;
public:
	Button();
	virtual ~Button();
	void Update();
	void Render(XMMATRIX world) override;
public:
	bool IsHover();
	bool IsClick();
};

//Text
class Text:public UI
{
private:
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_srv;
	Font* m_font;
public:
	Text();
	void Render(XMMATRIX world)override {};
	void SetText(std::string text);
	void SetFont(Font* font);
};

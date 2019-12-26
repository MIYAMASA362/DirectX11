#pragma once

struct ID3D11Buffer;

class Texture;

class UI;
class Font;

//CanvasÅ@2DâÊñ è„ÇÃèÓïÒÇèäéùÇ∑ÇÈ
class Canvas:public Component<Canvas>
{
private:
	std::list<std::weak_ptr<UI>> UIIndex;
public:
	Canvas(EntityID OwnerID);
};

//UI
class UI:public Component<UI>
{
public:
	UI(EntityID OwnerID);
	virtual ~UI() = default;
public:
	virtual void Render(XMMATRIX world) = 0;
};

//UI Image
class Image:public Renderer2D
{
public:
	std::weak_ptr<Texture> texture;
	ID3D11Buffer* vertexBuffer = nullptr;
public:
	Image(EntityID OwnerID);
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
	Button(EntityID OwnerID);
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
	Text(EntityID OwnerID);
	void Render(XMMATRIX world)override {};
	void SetText(std::string text);
	void SetFont(Font* font);
};

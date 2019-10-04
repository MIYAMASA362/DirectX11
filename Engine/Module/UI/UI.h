#pragma once

struct ID3D11Buffer;

namespace DirectX
{
	class BaseTexture;
	class Texture;
	class UI;
	class Font;

	//Canvas�@2D��ʏ�̏�����������
	class Canvas:public Component<Canvas>
	{
	private:
		std::list<std::weak_ptr<UI>> UIIndex;
	public:
		Canvas(EntityID OwnerID):Component(OwnerID,"Canvas"){};
	};

	//UI
	class UI:public Component<UI>
	{
	public:
		UI(EntityID OwnerID,std::string name):Component(OwnerID,name){};
		virtual ~UI() = default;
	public:
		virtual void OnComponent() override;
		virtual void Render(XMMATRIX world) = 0;
	};

	//UI Image
	class Image:public UI
	{
	public:
		BaseTexture* texture = nullptr;
		ID3D11Buffer* vertexBuffer = nullptr;
	public:
		Image(EntityID OwnerID);
		virtual ~Image();
		virtual void Render(XMMATRIX world) override;
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
		Text(EntityID OwnerID):UI(OwnerID,"Text"){};
		void Render(XMMATRIX world)override {};
		void SetText(std::string text);
		void SetFont(Font* font);
	};
}
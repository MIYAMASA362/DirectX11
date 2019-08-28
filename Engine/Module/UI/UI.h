#pragma once

struct ID3D11Buffer;

namespace DirectX
{
	class BaseTexture;
	class Texture;
	class UI;
	class Font;

	//CanvasÅ@2DâÊñ è„ÇÃèÓïÒÇèäéùÇ∑ÇÈ
	class Canvas:public Component
	{
	private:
		std::list<std::weak_ptr<UI>> UIIndex;
	public:
		virtual const std::type_info& GetType() override { return typeid(*this); };
		Canvas():Component("Canvas"){};
	};

	//UI
	class UI:public Component
	{
	public:
		UI(std::string name):Component(name){};
		virtual ~UI() = default;
	public:
		virtual const std::type_info& GetType() override { return typeid(*this); };
		virtual void OnComponent() override;
		virtual void SendBehaviourMessage(Message message) override;
		virtual void Render(XMMATRIX world) = 0;
	};

	//UI Image
	class Image:public UI
	{
	public:
		BaseTexture* texture = nullptr;
		ID3D11Buffer* vertexBuffer = nullptr;
	public:
		Image();
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
		Button();
		virtual ~Button();
		void SendBehaviourMessage(Message message) override;
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
		Text():UI("Text"){};
		void Render(XMMATRIX world)override {};
		void SetText(std::string text);
		void SetFont(Font* font);
	};
}
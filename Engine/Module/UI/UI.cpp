#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"Module\Input\Input.h"

//ECS
#include"Module\ECSEngine.h"

#include"Module\Texture\texture.h"

#include"Module\Renderer\Renderer.h"

#include"font.h"
#include"UI.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"
#include"Module\Transform\Transform.h"

#include"Module\Shader\Shader.h"

using namespace DirectX;

//--- Canvas --------------------------------------------------------
Canvas::Canvas(EntityID OwnerID)
	:
	Component(OwnerID)
{
	
}

//--- UI ------------------------------------------------------------

UI::UI(EntityID OwnerID)
	:
	Component(OwnerID)
{
	if (!gameObject()->GetComponent<Canvas>().lock())
		gameObject()->AddComponent<Canvas>();

	this->SendComponentMessage = [this](std::string message) 
	{
		if (message == "Render") {
			this->Render(this->transform()->WorldMatrix());
		}
	};
}


//--- Image ---------------------------------------------------------

Image::Image(EntityID OwnerID)
:
	Renderer(OwnerID)
{
	VERTEX_3D vertex[4] = {
		{ XMFLOAT3(-5.0f, -5.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3( 5.0f, -5.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-5.0f,  5.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3( 5.0f,  5.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f) },
	};

	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(VERTEX_3D) * (sizeof(vertex)/sizeof(VERTEX_3D));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subResourceData;
		ZeroMemory(&subResourceData,sizeof(subResourceData));
		subResourceData.pSysMem = vertex;

		D3DApp::Renderer::GetD3DAppDevice()->GetDevice()->CreateBuffer(&bufferDesc,&subResourceData,&this->vertexBuffer);
	}
}

Image::~Image()
{
	vertexBuffer->Release();
}

void Image::Render(XMMATRIX& world)
{
	D3DApp::Renderer::SetDepthEnable(false);
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	//頂点バッファ設定
	D3DApp::Renderer::GetD3DAppDevice()->GetDeviceContext()->IASetVertexBuffers(0,1,&this->vertexBuffer,&stride,&offset);
	//テクスチャ設定
	this->texture.lock()->SetResource();
	//ワールド行列設定
	D3DApp::Renderer::SetWorldMatrix(&world);
	//トポロジ設定
	D3DApp::Renderer::GetD3DAppDevice()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//描画
	D3DApp::Renderer::GetD3DAppDevice()->GetDeviceContext()->Draw(4,0);
	D3DApp::Renderer::SetDepthEnable(true);
}

//--- Button --------------------------------------------------------

Button::Button(EntityID OwnerID)
:
	UI(OwnerID),
	m_scale(5.0f),
	m_isHover(false)
{
	VERTEX_3D vertex[4] =
	{
		{ XMFLOAT3(-m_scale, -m_scale, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3( m_scale, -m_scale, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-m_scale,  m_scale, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3( m_scale,  m_scale, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f) },
	};

	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(VERTEX_3D) * (sizeof(vertex) / sizeof(VERTEX_3D));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA subResourceData;
		ZeroMemory(&subResourceData, sizeof(subResourceData));
		subResourceData.pSysMem = vertex;

		D3DApp::Renderer::GetD3DAppDevice()->GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &this->m_vertexBuffer);
	}
}

Button::~Button()
{
	this->m_vertexBuffer->Release();
}

//void Button::SendBehaviourMessage(Message message)
//{
//	switch (message)
//	{
//	case Component::Update:
//		Update();
//		break;
//	case Component::Render:
//		UI::SendBehaviourMessage(message);
//		break;
//	default:
//		return;
//	}
//}

void Button::Update()
{
	float mouseX,mouseY;
	float left, right, top, bottom;
	Vector3 center, scale;

	mouseX = Input::Mouse::GetMouseX();
	mouseY = Input::Mouse::GetMouseY();

	center = this->transform()->position();
	scale = this->transform()->scale() * this->m_scale;

	left	= center.x - scale.x;
	right	= center.x + scale.x;
	top		= center.y - scale.y;
	bottom	= center.y + scale.y;

	this->m_isHover = (left <= mouseX && mouseX <= right && top <= mouseY && mouseY <= bottom);
}

void Button::Render(XMMATRIX world)
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	//頂点バッファ設定
	D3DApp::Renderer::GetD3DAppDevice()->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);
	//テクスチャ設定
	this->texture->SetResource();
	//ワールド行列設定
	D3DApp::Renderer::SetWorldMatrix(&world);
	//トポロジ設定
	D3DApp::Renderer::GetD3DAppDevice()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//描画
	D3DApp::Renderer::GetD3DAppDevice()->GetDeviceContext()->Draw(4, 0);
}

bool Button::IsHover()
{
	return this->m_isHover;
}

bool Button::IsClick()
{
	return this->m_isHover && Input::Mouse::IsLeftDown();
}

Text::Text(EntityID OwnerID)
:
	UI(OwnerID)
{
}

void Text::SetText(std::string text)
{
	if (this->m_texture)
		delete this->m_texture;

	const char* str = text.c_str();	//文字列

	int len = 0;				//文字ビットサイズ
	UINT uChar = 0;				//文字コード
	size_t length = text.length();	//ループ数

	HDC hdc = GetDC(NULL);
	TEXTMETRIC tm;
	GLYPHMETRICS* gm = new GLYPHMETRICS[length];
	BYTE** pFontBMP	 = new BYTE*[length];

	HFONT oldFont = (HFONT)SelectObject(hdc,this->m_font->GetFont());
	GetTextMetrics(hdc,&tm);
	const MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };

	int count = 0;
	for(size_t n = 0; n < length;){
		len = IsDBCSLeadByte(str[n])?2:1;
		uChar = (len == 2 ? (unsigned char)str[n] << 8 | (unsigned char)str[n+1] :(unsigned char)str[n]);

		//ビットマップの生成
		{
			//フォントビットマップのメモリサイズ取得
			DWORD size = GetGlyphOutline(
				hdc,
				uChar,				//表示文字
				GGO_GRAY4_BITMAP,	//ビットマップ
				&gm[count],			//ビットマップ情報
				0,					//ビットマップサイズ
				NULL,				//フォントビットマップ
				&mat				//特殊変換
			);

			//フォントビットマップの取得
			pFontBMP[count] = new BYTE[size];
			GetGlyphOutline(
				hdc,
				uChar,
				GGO_GRAY4_BITMAP,
				&gm[count],
				size,
				pFontBMP[count],
				&mat
			);
		}


		n += (len == 2 ? 2 : 1);
	}
}

void Text::SetFont(Font * font)
{
	m_font = font;
}

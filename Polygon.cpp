#include"main.h"
#include"DirectXStruct.h"
#include"DirectX.h"
#include"texture.h"
#include"Object.h"
#include"Component.h"
#include"Behaviour.h"
#include"Polygon.h"


void CPolygon::Initialize()
{
	VERTEX_3D vertex[4] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(200.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(0.0f, 200.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(200.0f, 200.0f, 0.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f) },
	};

	//頂点バッファの生成
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VERTEX_3D) * (sizeof(vertex)/sizeof(VERTEX_3D));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = vertex;

	D3DApp::GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &this->m_VertexBuffer);
	
	//テクスチャの読み込み
	this->m_Texture = new CTexture();
	this->m_Texture->Load("asset/k-on0664.tga");
}

void CPolygon::Finalize()
{
	if (m_Texture) {
		m_Texture->Unload();
		delete m_Texture;
	}
	if (m_VertexBuffer){
		m_VertexBuffer->Release();
	}
}

void CPolygon::Update()
{

}

void CPolygon::Render()
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	//頂点バッファ設定 複数個登録もできる
	D3DApp::GetDeviceContext()->IASetVertexBuffers(0,1,&m_VertexBuffer,&stride,&offset);
	D3DApp::Renderer::SetTexture(this->m_Texture);
	D3DApp::Renderer::SetWorldViewProjection2D();
	//どうやって頂点を繋げるのか頂点の設定をトポロジという
	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//Draw(n,i)　n:総数 i:どこから始めるか
	D3DApp::GetDeviceContext()->Draw(4,0);
}
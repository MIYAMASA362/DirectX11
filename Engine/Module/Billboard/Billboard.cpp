#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\Texture\texture.h"

#include"Module\ECSEngine.h"

#include"Module\Camera\camera.h"

#include"Module\Renderer\Renderer.h"
#include"Module\Mesh\Mesh.h"
#include"Billboard.h"

DirectX::Billboard::Billboard()
{
	HRESULT hr;

	//頂点設定
	{
		this->m_scale = 1.0f;

		this->_VertexArray = new VERTEX_3D[4]
		{
			{ XMFLOAT3(-this->m_scale, this->m_scale,0.0f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,0.0f) },
			{ XMFLOAT3( this->m_scale, this->m_scale,0.0f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,0.0f) },
			{ XMFLOAT3(-this->m_scale,-this->m_scale,0.0f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(0.0f,1.0f) },
			{ XMFLOAT3( this->m_scale,-this->m_scale,0.0f),XMFLOAT3(0.0f,1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f),XMFLOAT2(1.0f,1.0f) }
		};

		//頂点バッファ生成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(VERTEX_3D) * 4;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.MiscFlags = 0;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = this->_VertexArray;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &this->m_VertexBuffer);
		if (FAILED(hr))
			MessageBox(NULL, "頂点バッファの生成に失敗しました。", "Billboard", MB_OK);
	}

	//インデックス設定
	{
		this->m_IndexNum = 4;
		WORD* pIndex = new WORD[this->m_IndexNum];
		for (unsigned int i = 0; i < this->m_IndexNum; i++) {
			pIndex[i] = i;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth = sizeof(WORD) * this->m_IndexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem = pIndex;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&this->m_IndexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"インデックスバッファの生成に失敗しました。","Billboard",MB_OK);

		delete[] pIndex;
	}
}

DirectX::Billboard::~Billboard()
{
	if (this->m_IndexBuffer != nullptr)
		this->m_IndexBuffer->Release();
	if (this->m_VertexBuffer != nullptr)
		this->m_VertexBuffer->Release();
}

void DirectX::Billboard::Render(XMMATRIX& worldMatrix)
{
	XMMATRIX InvView = Camera::GetActiveCamera()->GetViewMatrix();
	InvView.r[0].m128_f32[3] = 0.0f;
	InvView.r[1].m128_f32[3] = 0.0f;
	InvView.r[2].m128_f32[3] = 0.0f;

	if(lockAxis.x){
		InvView.r[0].m128_f32[1] = 0.0f;
		InvView.r[0].m128_f32[2] = 0.0f;
	}
	if(lockAxis.y){
		InvView.r[1].m128_f32[0] = 0.0f;
		InvView.r[1].m128_f32[2] = 0.0f;
	}
	if(lockAxis.z){
		InvView.r[2].m128_f32[0] = 0.0f;
		InvView.r[2].m128_f32[1] = 0.0f;
	}

	XMMATRIX world = XMMatrixIdentity();

	if (this->IsPivotBottom)
		world = XMMatrixTranslation(0.0f, this->m_scale, 0.0f);
	world *= InvView * worldMatrix;

	D3DApp::Renderer::SetVertexBuffer(this->m_VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(this->m_IndexBuffer);
	D3DApp::Renderer::SetWorldMatrix(&world);
	D3DApp::Renderer::SetTexture(this->m_Texture->GetShaderResourceView());
	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	D3DApp::GetDeviceContext()->DrawIndexed(this->m_IndexNum,0,0);
}

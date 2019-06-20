#include<Windows.h>
#include"main.h"
#include"DirectXStruct.h"
#include"DirectX.h"
#include"texture.h"
#include"Component.h"
#include"Tag.h"
#include"Transform.h"
#include"GameObject.h"
#include"Behaviour.h"
#include"field.h"

void CField::Initialize()
{
	VERTEX_3D vertex[4] =
	{
		{ XMFLOAT3(50.0f, 0.0f, 50.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-50.0f, 0.0f, 50.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(50.0f, 0.0f, -50.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-50.0f, 0.0f, -50.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f) },
	};

	//頂点バッファの生成
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VERTEX_3D) * (sizeof(vertex) / sizeof(VERTEX_3D));
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

void CField::Finalize()
{
	if (m_Texture) {
		m_Texture->Unload();
		delete m_Texture;
	}
	if (m_VertexBuffer) {
		m_VertexBuffer->Release();
	}
}

void CField::Update()
{

}

void CField::Render()
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	
	D3DApp::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	D3DApp::Renderer::SetTexture(this->m_Texture);

	//行列変換
	XMMATRIX world;
	world = XMMatrixScaling(1.0f,1.0f,1.0f);
	world *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	world *= XMMatrixTranslation(0.0f, 0.0f, 50.0f);
	D3DApp::Renderer::SetWorldMatrix(&world);

	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//Draw(n,i)　n:総数 i:どこから始めるか
	D3DApp::GetDeviceContext()->Draw(4, 0);

}

void CWallField::Initialize()
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subResourceData;

	VERTEX_3D field[4] =
	{
		{ XMFLOAT3(1.0f, 0.0f, 1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 0.0f, 1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 0.0f, -1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 0.0f, -1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f) },
	};

	//頂点バッファの生成
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VERTEX_3D) * (sizeof(field) / sizeof(VERTEX_3D));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = field;

	D3DApp::GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &this->m_FieldVertexBuffer);


	VERTEX_3D wall[4] =
	{
		{ XMFLOAT3(1.0f, 1.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 0.0f, 0.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f) },
	};

	//頂点バッファの生成
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VERTEX_3D) * (sizeof(wall) / sizeof(VERTEX_3D));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = wall;

	D3DApp::GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &this->m_WallVertexBuffer);



	//テクスチャの読み込み
	this->m_FieldTexture = new CTexture();
	this->m_FieldTexture->Load("asset/field004.tga");

	this->m_WallTexture = new CTexture();
	this->m_WallTexture->Load("asset/k-on0664.tga");
}

void CWallField::Finalize()
{
	if(m_WallTexture)
	{
		m_WallTexture->Unload();
		delete m_WallTexture;
	}
	if(m_WallVertexBuffer)
	{
		m_WallVertexBuffer->Release();
	}

	if (m_FieldTexture) {
		m_FieldTexture->Unload();
		delete m_FieldTexture;
	}
	if (m_FieldVertexBuffer) {
		m_FieldVertexBuffer->Release();
	}
}

void CWallField::Update()
{

}

void CWallField::Render()
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	XMMATRIX WorldPosition = this->gameObject->transform.MatrixTranslation();
	XMMATRIX WorldRotation = this->gameObject->transform.MatrixRotationRollPitchYaw();
	XMMATRIX WorldScale = this->gameObject->transform.MatrixScaling();

	D3DApp::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_FieldVertexBuffer, &stride, &offset);
	D3DApp::Renderer::SetTexture(this->m_FieldTexture);

	//行列変換
	XMMATRIX world = WorldScale * WorldRotation * WorldPosition;

	D3DApp::Renderer::SetWorldMatrix(&world);

	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//Draw(n,i)　n:総数 i:どこから始めるか
	D3DApp::GetDeviceContext()->Draw(4, 0);

	for(int i=0; i< 4; i++)
	{
		XMMATRIX localPosition = XMMatrixTranslation(0.0f,0.0f,1.0f);
		XMMATRIX localRotation = XMMatrixRotationRollPitchYaw(0.0f, XM_PIDIV2 * i, 0.0f);
		XMMATRIX localScale = XMMatrixScaling(1.0f,0.5f,0.0f);

		XMMATRIX local = localScale* localPosition * localRotation;

		D3DApp::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_WallVertexBuffer, &stride, &offset);
		D3DApp::Renderer::SetTexture(this->m_WallTexture);

		local = local * world;

		D3DApp::Renderer::SetWorldMatrix(&local);

		D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//Draw(n,i)　n:総数 i:どこから始めるか
		D3DApp::GetDeviceContext()->Draw(4, 0);
	}
}

//--- CMeshField --------------------------------------------------------------

void CMeshField::Init(XMFLOAT2 GridScale,unsigned int WidthNum,unsigned int HeightNum)
{
	const unsigned int wGrid = WidthNum + 1;
	const unsigned int hGrid = HeightNum + 1;

	const float width = GridScale.x;
	const float height = GridScale.y;

	{
		VERTEX_3D* pVertex = new VERTEX_3D[wGrid * hGrid];

		for (unsigned int y = 0; y < hGrid; y++)
		{
			for (unsigned int x = 0; x < wGrid; x++)
			{
				VERTEX_3D* vertex = &pVertex[y*wGrid + x];
				vertex->Position = XMFLOAT3(width * x, 0, height * y);
				vertex->Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				vertex->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertex->TexCoord = XMFLOAT2(1.0f*x, 1.0f*y);
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * wGrid * hGrid;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pVertex;

		D3DApp::GetDevice()->CreateBuffer(&bd, &sd, &this->m_VertexBuffer);

		delete[] pVertex;
	}
	
	{
		int* pIndex = new int[wGrid * hGrid + ( HeightNum * 2)];
		int nCount = 0;

		for(unsigned int y = 0; y < hGrid-1; y++)
		{
			unsigned int x = 0;
			if (y != 0)
			{
				pIndex[nCount] = pIndex[nCount-1];
				pIndex[nCount + 1] = y* hGrid + x;
				nCount += 2;
			}

			for (; x < wGrid; x++)
			{
				pIndex[nCount] = y* hGrid + x;
				pIndex[nCount + 1] = (y + 1)*hGrid + x;
				nCount += 2;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned short);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem;
	}

}

void CMeshField::UnInit()
{

}

void CMeshField::Update()
{

}

void CMeshField::Draw(XMFLOAT3 Position, XMFLOAT3 Rotation, XMFLOAT3 Scale)
{

}




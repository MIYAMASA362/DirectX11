#include<Windows.h>
#include"Common.h"

#include"Module\AssetData\AssetData.h"

#include"Module\Texture\texture.h"
#include"Module\Texture\TextureManager.h"

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\ECSEngine.h"

#include"Module\AssetData\AssetData.h"

//Component Module
#include"Module\Renderer\Renderer.h"
#include"Module\Mesh\Mesh.h"

#include"Module\Shader\Shader.h"

#include"field.h"

//--- CField --------------------------------------------------------
#pragma region CField

CField::CField()
{
	this->_VertexArray = new VERTEX_3D[4]
	{
		{ XMFLOAT3(50.0f, 0.0f, 50.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-50.0f, 0.0f, 50.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(50.0f, 0.0f, -50.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(-50.0f, 0.0f, -50.0f),	XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f) },
	};
	_VertexNum = 4;

	//頂点バッファの生成
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VERTEX_3D) * (sizeof(_VertexArray) / sizeof(VERTEX_3D));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResourceData;
	ZeroMemory(&subResourceData, sizeof(subResourceData));
	subResourceData.pSysMem = _VertexArray;

	D3DApp::GetDevice()->CreateBuffer(&bufferDesc, &subResourceData, &this->m_VertexBuffer);
}

CField::~CField()
{
	if (m_Texture) {
		m_Texture = nullptr;
	}
	if (m_VertexBuffer) {
		m_VertexBuffer->Release();
	}
}

void CField::Render(XMMATRIX& worldMatrix)
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	D3DApp::GetConstBuffer()->UpdateSubresource(CONSTANT_BUFFER_WORLD, &worldMatrix);

	D3DApp::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	this->m_Texture->SetResource();

	//行列変換
	XMMATRIX world;
	world = XMMatrixScaling(1.0f,1.0f,1.0f);
	world *= XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	world *= XMMatrixTranslation(0.0f, 0.0f, 50.0f);
	D3DApp::GetConstBuffer()->UpdateSubresource(CONSTANT_BUFFER_WORLD, &worldMatrix);

	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//Draw(n,i)　n:総数 i:どこから始めるか
	D3DApp::GetDeviceContext()->Draw(4, 0);

}
#pragma endregion

//--- WallField ----------------------------------------------------
#pragma region WallField

WallField::WallField()
{
	//Mesh設定
	{
		D3D11_BUFFER_DESC bufferDesc;
		D3D11_SUBRESOURCE_DATA subResourceData;

		_VertexArray = new VERTEX_3D[4]
		{
			{ XMFLOAT3(1.0f, 0.0f, 1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, 0.0f, 1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 0.0f, -1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 0.0f, -1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f),		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f) },
		};
		_VertexNum = 4;

		//頂点バッファの生成
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(VERTEX_3D) * (sizeof(_VertexArray) / sizeof(VERTEX_3D));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;

		ZeroMemory(&subResourceData, sizeof(subResourceData));
		subResourceData.pSysMem = _VertexArray;

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
	}
}

WallField::~WallField()
{
	if(m_WallVertexBuffer)
	{
		m_WallVertexBuffer->Release();
	}
	if (m_FieldVertexBuffer) {
		m_FieldVertexBuffer->Release();
	}
}

void WallField::Render(XMMATRIX& worldMatrix)
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	D3DApp::GetConstBuffer()->UpdateSubresource(CONSTANT_BUFFER_WORLD, &worldMatrix);

	D3DApp::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_FieldVertexBuffer, &stride, &offset);
	this->FieldTexture->SetResource();

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
		this->WallTexture->SetResource();

		local = local * worldMatrix;

		D3DApp::GetConstBuffer()->UpdateSubresource(CONSTANT_BUFFER_WORLD, &worldMatrix);

		D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//Draw(n,i)　n:総数 i:どこから始めるか
		D3DApp::GetDeviceContext()->Draw(4, 0);
	}
}

#pragma endregion

//--- SkySphere -----------------------------------------------------
#pragma region SkySphere

SkySphere::SkySphere()
{
	HRESULT hr;

	const unsigned int cWidthSurface = 12;	//面数
	const unsigned int cHeightSurface = 12;

	const unsigned int cWidthGrid = cWidthSurface + 1;		//横Grid数
	const unsigned int cHeightGrid = cHeightSurface + 1;	//縦Grid数

	const float c_radius = 1.0f;			//半径	

	//頂点設定
	{
		const unsigned int cIndexNum = cWidthGrid * cHeightGrid;

		const float WidthAngle	= Mathf::PI2 / cWidthSurface;
		const float HeightAngle = Mathf::PI_2 / cHeightSurface;

		const float cTexWidth = (1.0f / cWidthGrid);
		const float cTexHeight = (1.0f / cHeightGrid);

		this->_VertexArray = new VERTEX_3D[cIndexNum];
		this->_VertexNum = cIndexNum;

		int nCount = 0;
		float x, y, z,radius;

		//頂点設定			
		for (int h = 0; h < cHeightGrid; h++) {
			y		= c_radius * sinf(HeightAngle * h);
			radius  = c_radius * cosf(HeightAngle * h);

			for (int w = 0; w < cWidthGrid; w++) {
				x = radius * sinf(WidthAngle * w);
				z = radius * cosf(WidthAngle * w);

				VERTEX_3D& vertex = this->_VertexArray[nCount];
				vertex.Position = XMFLOAT3(x, y, z);
				vertex.Normal	= XMFLOAT3(0.0f, 1.0f, 0.0f);
				vertex.Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertex.TexCoord = XMFLOAT2(cTexWidth * w, cTexHeight * h);
				if (h == 0)
					vertex.TexCoord.y += 0.005f;
				nCount++;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * cIndexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem = this->_VertexArray;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&this->m_VertexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"頂点バッファの生成に失敗しました。","SkySphere",MB_OK);
	}

	//インデックス設定
	{
		const unsigned int c_indexNum = (cWidthGrid*2) * cHeightSurface + (cHeightSurface-1)*2;
		this->m_IndexNum = c_indexNum;
		WORD* pIndex = new WORD[c_indexNum]{0};

		int nCount = 0;
		int w, h;
		for(h = 0; h < cHeightSurface; h++){
			w = 0;
			if(h != 0){
				pIndex[nCount] = pIndex[nCount - 1];
				pIndex[nCount + 1] = cWidthGrid * h + w;
				nCount += 2;
			}

			for(; w < cWidthGrid; w++){
				pIndex[nCount] = cWidthGrid * h + w;
				pIndex[nCount + 1] = cWidthGrid * (h + 1) + w;
				nCount += 2;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.ByteWidth = sizeof(WORD) * c_indexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem = pIndex;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&this->m_IndexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"頂点インデックスの生成に失敗しました。","SkyShpere",MB_OK);

		delete[] pIndex;
	}
}

SkySphere::~SkySphere()
{
	if (this->m_IndexBuffer != nullptr)
		this->m_IndexBuffer->Release();
	if (this->m_VertexBuffer != nullptr)
		this->m_VertexBuffer->Release();
}

void SkySphere::Render(XMMATRIX& worldMatrix)
{
	D3DApp::Renderer::SetVertexBuffer(this->m_VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(this->m_IndexBuffer);
	D3DApp::GetConstBuffer()->UpdateSubresource(CONSTANT_BUFFER_WORLD, &worldMatrix);
	
	this->m_Texture->SetResource();
	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3DApp::GetDeviceContext()->DrawIndexed(this->m_IndexNum,0,0);
}

#pragma endregion

//--- MeshField -----------------------------------------------------
#pragma region MeshField

MeshField::MeshField()
{
	HRESULT hr;

	const unsigned int cWidthSurface = 40;
	const unsigned int cDepthSurface = 40;

	this->m_WidthNum = cWidthSurface;
	this->m_DepthNum = cDepthSurface;

	const unsigned int cWidthGrid = cWidthSurface + 1;
	const unsigned int cDepthGrid = cDepthSurface + 1;

	

	//頂点設定
	{
		const float cWidth = 2.2f/cWidthGrid;
		const float cDepth = 2.2f/cDepthGrid;

		this->m_Width = cWidth;
		this->m_Depth = cDepth;

		const unsigned int cIndexNum = cWidthGrid * cDepthGrid;
		_VertexArray = new VERTEX_3D[cIndexNum];
		_VertexNum = cIndexNum;

		float* yIndex = new float[cIndexNum];

		int nCount = 0;
		for(int z = 0; z < cDepthGrid; z++){
			for (int x = 0; x < cWidthGrid; x++) {
				yIndex[nCount] = 0.0f;
				if (z < 10 || cDepthGrid - 10 < z)
					yIndex[nCount] = 1.0f;

				nCount++;
			}
		}


		nCount = 0;
		const float y = 0.0f;
		const float sX = cWidth * (cWidthSurface * 0.5f);
		const float sZ = -cDepth * (cDepthSurface * 0.5f);

		for (int z = 0; z < cDepthGrid; z++) {
			for (int x = 0; x < cWidthGrid; x++) {
				VERTEX_3D& vertex = _VertexArray[nCount];
				vertex.Position =
					XMFLOAT3(
						sX - (float)x * cWidth,
						yIndex[nCount],
						sZ + (float)z * cDepth
					);
				vertex.Normal	= XMFLOAT3(0.0f, 1.0f, 0.0f);
				vertex.Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertex.TexCoord = XMFLOAT2(x+0.1f, z+0.1f);
				nCount++;
			}
		}

		//法線設定
		for (int z = 1; z < cDepthGrid-1; z++) {
			for (int x = 1; x < cWidthGrid-1; x++)
			{
				Vector3 vecA = (Vector3)_VertexArray[(z - 1)*cDepthGrid + x].Position - (Vector3)_VertexArray[(z + 1)*cWidthGrid + x].Position;
				Vector3 vecB = (Vector3)_VertexArray[z*cDepthGrid + (x-1)].Position - (Vector3)_VertexArray[z*cWidthGrid + (x+1)].Position;

				Vector3 normal = Vector3::Cross(vecB,vecA);
				_VertexArray[z * cWidthGrid + x].Normal = normal.normalize();
			}

		}

		delete[] yIndex;

		nCount = nCount;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D)*cIndexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem = _VertexArray;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&this->m_VertexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"頂点バッファーの生成に失敗しました。","MeshField",MB_OK);
	}

	//インデックス設定
	{
		this->m_IndexNum = (cWidthGrid * 2) * cDepthSurface + (cDepthSurface -1)*2;

		WORD* pIndex = new WORD[this->m_IndexNum]{0};
		int nCount = 0;
		int x, z;
		for(z = 0; z < cDepthSurface; z++){
			x = 0;
			if (z != 0) {
				pIndex[nCount]		= pIndex[nCount-1];
				pIndex[nCount + 1]	= cDepthGrid * (z + 1) + x;
				nCount += 2;
			}
			for(;x < cWidthGrid; x++){
				pIndex[nCount]		= cDepthGrid * (z + 1) + x;
				pIndex[nCount + 1]	= cDepthGrid * z + x;
				nCount += 2;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.ByteWidth = sizeof(WORD) * this->m_IndexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pIndex;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&this->m_IndexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"インデックスバッファの生成に失敗しました。","MeshField",MB_OK);

		//Surface設定
		{
			_SurfaceNum = this->m_IndexNum - 2;
			_Surface = new Surface[_SurfaceNum];
			for(int i = 0 ; i < _SurfaceNum; i++)
			{
				_Surface[i]._p1 = _VertexArray[pIndex[i+0]].Position;
				_Surface[i]._p2 = _VertexArray[pIndex[i+1]].Position;
				_Surface[i]._p3 = _VertexArray[pIndex[i+2]].Position;
			}
		}
		delete[] pIndex;
	}
}

MeshField::~MeshField()
{
	if (this->m_IndexBuffer != nullptr)
		this->m_IndexBuffer->Release();
	if (this->m_VertexBuffer != nullptr)
		this->m_VertexBuffer->Release();
}

void MeshField::Render(XMMATRIX& worldMatrix)
{
	D3DApp::GetShader()->SetShader();

	D3DApp::Renderer::SetVertexBuffer(this->m_VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(this->m_IndexBuffer);

	D3DApp::GetConstBuffer()->UpdateSubresource(CONSTANT_BUFFER_WORLD, &worldMatrix);
	D3DApp::GetConstBuffer()->SetVSConstantBuffer(CONSTANT_BUFFER_WORLD,0);

	this->m_Texture->SetResource();
	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3DApp::GetDeviceContext()->DrawIndexed(this->m_IndexNum, 0, 0);
}

#pragma endregion

//--- MeshWall ------------------------------------------------------
#pragma region MeshWall

DirectX::MeshWall::MeshWall()
{
	HRESULT hr;

	const unsigned int cWidthSurface = 10;
	const unsigned int cHeightSurface = 10;

	const unsigned int cWidthGrid = cWidthSurface + 1;
	const unsigned int cHeightGrid = cHeightSurface + 1;

	//頂点設定
	{
		this->m_Scale = 2.0f;

		const float cTexWidth = 1.0f / cWidthGrid;
		const float cTexHeight = 1.0f / cHeightGrid;

		const float cWidth  = (this->m_Scale+0.2f) / cWidthGrid;
		const float cHeight = (this->m_Scale+0.2f) / cHeightGrid;

		const float sX = -this->m_Scale * 0.5f;
		const float sY = -this->m_Scale * 0.5f;

		const unsigned int cIndexNum = cWidthGrid * cHeightGrid;
		_VertexArray = new VERTEX_3D[cIndexNum];
		_VertexNum = cIndexNum;

		int nCount = 0;
		for (int y = 0; y < cHeightGrid; y++) {
			for (int x = 0; x < cWidthGrid; x++) {
				VERTEX_3D& vertex = _VertexArray[nCount];
				vertex.Position =
					XMFLOAT3(
						sX + cWidth * x,
						sY + cHeight * y,
						0.0f
					);
				vertex.Normal = XMFLOAT3(0.0f,1.0f,0.0f);
				vertex.Diffuse = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
				vertex.TexCoord = XMFLOAT2(cTexWidth * x,cTexHeight * y);
				nCount++;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(VERTEX_3D) * cIndexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem = _VertexArray;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&this->m_VertexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"頂点インデックスの生成に失敗しました。","MeshWall",MB_OK);
	}

	//インデックス設定
	{
		this->m_IndexNum = (cWidthGrid * 2)*cHeightSurface + (cHeightSurface -1)*2;
		WORD* pIndex = new WORD[this->m_IndexNum];

		int nCount = 0;
		int x,y;
		for(y = 0; y < cHeightSurface; y++){
			x = 0;
			if(y != 0){
				pIndex[nCount] = pIndex[nCount - 1];
				pIndex[nCount + 1] = cWidthGrid * (y+1) + x;
				nCount += 2;
			}

			for(; x < cWidthGrid; x++){
				pIndex[nCount] = cWidthGrid * (y + 1) + x;
				pIndex[nCount + 1] = cWidthGrid * y + x;
				nCount += 2;
			}
		}
		nCount = nCount;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth = sizeof(WORD) * this->m_IndexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem = pIndex;

		D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&this->m_IndexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"インデックスバッファの生成に失敗しました。","MeshWall",MB_OK);

		delete[] pIndex;
	}
}

DirectX::MeshWall::~MeshWall()
{
	if (this->m_VertexBuffer)
		this->m_VertexBuffer->Release();
	if (this->m_IndexBuffer)
		this->m_IndexBuffer->Release();
}

void DirectX::MeshWall::Render(XMMATRIX& worldMatrix)
{
	D3DApp::Renderer::SetVertexBuffer(this->m_VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(this->m_IndexBuffer);

	this->m_Texture->SetResource();
	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	XMMATRIX world;
	XMMATRIX local;
	local = XMMatrixTranslation(0.0f, this->m_Scale*0.5f, -this->m_Scale*0.5f);
	for (int i = 0; i < 4; i++) {
		world = local * worldMatrix;
		D3DApp::GetConstBuffer()->UpdateSubresource(CONSTANT_BUFFER_WORLD, &worldMatrix);
		D3DApp::GetDeviceContext()->DrawIndexed(this->m_IndexNum, 0, 0);
		local *= XMMatrixRotationY(Mathf::PI_2);
	}
}

#pragma endregion

//--- MeshCircle ----------------------------------------------------
#pragma region MeshCircle

DirectX::MeshCircle::MeshCircle()
{
	HRESULT hr;

	const unsigned int cSurface = 10;
	const unsigned int cGrid = cSurface + 1;
	const float cRadian = 1.0f;

	{
		const unsigned int cIndexNum = cGrid * 2;
		const float cAngle = Mathf::PI2 / (cGrid-1);
		const float cTexWidth = 1.0f / cGrid;

		_VertexArray = new VERTEX_3D[cIndexNum];
		_VertexNum = cIndexNum;

		int nCount = 0;
		for(int y = 0; y < 2; y++){
			for(int x = 0; x < cGrid; x++){
				VERTEX_3D& vertex = _VertexArray[nCount];
				vertex.Position =
					XMFLOAT3(
						cRadian * sinf(cAngle * x),
						(float)y,
						cRadian * cosf(cAngle * x)
					);
				vertex.Normal = XMFLOAT3(0.0f,1.0f,0.0f);
				vertex.Diffuse = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
				vertex.TexCoord = XMFLOAT2(cTexWidth*x,1.0f*y);
				nCount++;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd,sizeof(bd));
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.ByteWidth = sizeof(VERTEX_3D) * cIndexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd,sizeof(sd));
		sd.pSysMem = _VertexArray;

		hr = D3DApp::GetDevice()->CreateBuffer(&bd,&sd,&this->m_VertexBuffer);
		if (FAILED(hr))
			MessageBox(NULL,"頂点バッファの生成に失敗しました。","MeshCircle",MB_OK);
	}

	{
		this->m_IndexNum = cGrid * 2;
		WORD* pIndex = new WORD[this->m_IndexNum];

		int nCount = 0;
		for(int x = 0; x < cGrid; x++){
			pIndex[nCount]	= x;
			pIndex[nCount+1]		= cGrid + x;
			nCount += 2;
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
			MessageBox(NULL,"インデックスバッファの生成に失敗しました。","MeshCircle",MB_OK);

		delete[] pIndex;
	}
}

DirectX::MeshCircle::~MeshCircle()
{
	if (this->m_IndexBuffer)
		this->m_IndexBuffer->Release();
	if (this->m_VertexBuffer)
		this->m_VertexBuffer->Release();
}

void DirectX::MeshCircle::Render(XMMATRIX& worldMatrix)
{
	D3DApp::Renderer::SetVertexBuffer(this->m_VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(this->m_IndexBuffer);
	D3DApp::GetConstBuffer()->UpdateSubresource(CONSTANT_BUFFER_WORLD, &worldMatrix);

	this->m_Texture->SetResource();
	D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	D3DApp::GetDeviceContext()->DrawIndexed(this->m_IndexNum, 0, 0);
}

#pragma endregion




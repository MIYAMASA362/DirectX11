#pragma once

using namespace DirectX;
struct ID3D11Buffer;

class CField:public Mesh
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	CTexture* m_Texture = NULL;
public:
	CField();
	virtual ~CField();
	void Render(XMMATRIX worldMatrix) override;
};

class CWallField:public Mesh
{
private:
	ID3D11Buffer* m_FieldVertexBuffer = NULL;
	CTexture* m_FieldTexture = NULL;

	ID3D11Buffer* m_WallVertexBuffer = NULL;
	CTexture* m_WallTexture = NULL;
public:
	CWallField();
	~CWallField();
	void Render(XMMATRIX worldMatrix)override;
};

class CMeshField
{
private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	DX11_SUBSET* m_SubsetArray = NULL;
	unsigned short m_SubsetNum;
public:
	void Init(XMFLOAT2 GridScale, unsigned int WidthNum, unsigned int HeightNum);
	void UnInit();
	void Update();
	void Draw(XMFLOAT3 Position, XMFLOAT3 Rotation, XMFLOAT3 Scale);
};


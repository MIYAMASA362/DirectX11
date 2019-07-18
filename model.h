#pragma once

using namespace DirectX;

// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned short	VertexNum;
	unsigned short	*IndexArray;
	unsigned short	IndexNum;
	SUBSET			*SubsetArray;
	unsigned short	SubsetNum;
};

class CModel:public Mesh
{
private:
	ID3D11Buffer*	m_VertexBuffer = nullptr;
	ID3D11Buffer*	m_IndexBuffer = nullptr;

	DX11_SUBSET*	m_SubsetArray = nullptr;
	unsigned short	m_SubsetNum;
public:
	virtual ~CModel();
public:
	void Render(XMMATRIX worldMatrix) override;
public:
	void LoadObj( const char *FileName, MODEL *Model );
	void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum );
public:
	void Load( const char *FileName );
	void UnLoad();
};
#pragma once



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
	VERTEX_3D *VertexArray;
	unsigned short	VertexNum;
	unsigned short	*IndexArray;
	unsigned short	IndexNum;
	SUBSET			*SubsetArray;
	unsigned short	SubsetNum;
};



class CModel : public CGameObject
{
private:
	ID3D11Buffer*	m_VertexBuffer = NULL;
	ID3D11Buffer*	m_IndexBuffer = NULL;

	CShader* m_Shader;
	CTexture* m_Texture;
	CCamera* m_Camera;

	DX11_SUBSET*	m_SubsetArray = NULL;
	unsigned short	m_SubsetNum;

	void LoadObj( const char *FileName, MODEL *Model );
	void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned short *MaterialNum );

public:
	CModel();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Load( const char *FileName );
	void Unload();

	void SetCamera(CCamera* camera) { m_Camera = camera; };

	CShader* GetShader() { return m_Shader; }
	void SetTexture(CTexture* texture) { m_Texture = texture; }
};
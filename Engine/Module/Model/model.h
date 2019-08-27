#pragma once
using namespace DirectX;

// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
};

// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned short	StartIndex;
	unsigned short	IndexNum;
	MODEL_MATERIAL	Material;
};

// ���f���\����
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned short	VertexNum;
	unsigned short	*IndexArray;
	unsigned short	IndexNum;
	SUBSET			*SubsetArray;
	unsigned short	SubsetNum;
};

namespace DirectX
{
	//Model
	class Model:public Mesh
	{
		friend class ModelManager;
	private:
		XMMATRIX offsetMatrix;
		ID3D11Buffer* VertexBuffer = nullptr;
		ID3D11Buffer* IndexBuffer = nullptr;

		DX11_SUBSET* SubsetArray = nullptr;
		unsigned short SubsetNum;
		bool IsCulling = false;
		bool IsDepth = true;
	public:
		Model();
		void Render(XMMATRIX worldMatrix) override;
		void GetAsset(std::string name);
		void SetCulling(bool enable);
		void SetDepth(bool enable);
		void SetoffsetMatrix(XMMATRIX matrix);
	};
}
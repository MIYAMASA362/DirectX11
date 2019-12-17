#pragma once
using namespace DirectX;

//サブセット マテリアル区分け
struct ModelSubset
{
	unsigned short _StartIndex;
	unsigned short _IndexNum;
	TextureMaterial _Material;
};

//ノードメッシュ　ノード区分け
struct NodeMesh final
{
	using MeshType = Mesh;

	//オフセット行列
	XMMATRIX _OffsetMatrix;

	//メッシュ
	MeshType* _Mesh;

	//サブセット
	ModelSubset* _SubsetArray;
	unsigned short _SubsetNum;

	~NodeMesh();
};

//モデル
class Model
{
private:

public:
	std::vector<NodeMesh*> _NodeMeshArray;
	virtual~Model();
};
#pragma once
using namespace DirectX;

//*********************************************************************************************************************
//
//	ModelSubset �T�u�Z�b�g
//
//*********************************************************************************************************************
struct ModelSubset
{
	unsigned short _StartIndex;
	unsigned short _IndexNum;
	Material _Material;
};


//*********************************************************************************************************************
//
//	NodeMesh
//
//*********************************************************************************************************************
struct NodeMesh final
{
	using MeshType = Mesh;

	std::string _NodeName;

	//�I�t�Z�b�g�s��
	XMMATRIX _OffsetMatrix;

	//���b�V��
	std::weak_ptr<Mesh> _Mesh;

	//�T�u�Z�b�g
	ModelSubset* _SubsetArray;
	unsigned short _SubsetNum;

	~NodeMesh();
};

//*********************************************************************************************************************
//
//	Model
//
//*********************************************************************************************************************
class Model
{
private:

public:
	std::vector<NodeMesh*> _NodeMeshArray;
	virtual~Model();
};
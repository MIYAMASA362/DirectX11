#pragma once
using namespace DirectX;

//�T�u�Z�b�g �}�e���A���敪��
struct ModelSubset
{
	unsigned short _StartIndex;
	unsigned short _IndexNum;
	TextureMaterial _Material;
};

//�m�[�h���b�V���@�m�[�h�敪��
struct NodeMesh final
{
	using MeshType = Mesh;

	//�I�t�Z�b�g�s��
	XMMATRIX _OffsetMatrix;

	//���b�V��
	MeshType* _Mesh;

	//�T�u�Z�b�g
	ModelSubset* _SubsetArray;
	unsigned short _SubsetNum;

	~NodeMesh();
};

//���f��
class Model
{
private:

public:
	std::vector<NodeMesh*> _NodeMeshArray;
	virtual~Model();
};
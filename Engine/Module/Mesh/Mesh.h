#pragma once


//*********************************************************************************************************************
//
//	MeshLayer
//
//*********************************************************************************************************************
enum MeshLayer
{
	Default
};


//*********************************************************************************************************************
//
//	Surface
//
//*********************************************************************************************************************
struct Surface
{
	Vector3 _p1;
	Vector3 _p2;
	Vector3 _p3;

	Vector3 _normal;

	Vector3 Normal() { return Vector3::Cross(_p2 - _p1, _p3 - _p1); };


};


//*********************************************************************************************************************
//
//	Mesh
//
//*********************************************************************************************************************
class Mesh
{
	friend class MeshCollider;
	friend class Physics;
	friend class Collider;
public:
	using VertexType = VERTEX_3D;

	//�o�b�t�@
	ID3D11Buffer* _VertexBuffer;
	ID3D11Buffer* _IndexBuffer;

	//���_
	VertexType* _VertexArray;
	unsigned int _VertexNum;

	//�C���f�b�N�X
	unsigned short* _IndexArray;
	unsigned int _IndexNum;

	//��
	Surface* _Surface;
	unsigned int _SurfaceNum;

	//�R���X�g���N�^
	Mesh();
	//�f�X�g���N�^
	virtual ~Mesh();


public:
	//���_�o�b�t�@�ݒ�
	void SetVertexBuffer();
	//�C���f�b�N�X�ݒ�
	void SetIndexBuffer();

};


//*********************************************************************************************************************
//
//	MeshFilter
//
//*********************************************************************************************************************
class MeshFilter
{
	using MeshArray = std::vector<std::shared_ptr<Mesh>>;
private:
	//���b�V���s�� �������b�V���ɑΉ�
	MeshArray _MeshArray;


public:
	//���b�V���ݒ�
	void AddMesh(std::weak_ptr<Mesh> mesh) { _MeshArray.emplace_back(mesh.lock()); };
	//���b�V���擾
	std::weak_ptr<Mesh> GetMesh(unsigned int index) { return _MeshArray.at(index); };
	//���b�V�����̎擾
	size_t GetNumMesh() { return _MeshArray.size(); };
};
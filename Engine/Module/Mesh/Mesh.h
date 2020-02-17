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

	//�g�|���W�ݒ�
	D3D11_PRIMITIVE_TOPOLOGY _PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//�o�b�t�@
	ID3D11Buffer* _VertexBuffer;
	ID3D11Buffer* _IndexBuffer;

	//���_
	VertexType* _VertexArray;
	unsigned int _VertexNum;

	//���_�C���f�b�N�X
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
	//�R���e�L�X�g�ɒ��_�o�b�t�@�ݒ�
	void SetVertexBuffer();
	//�R���e�L�X�g�ɒ��_�C���f�b�N�X�ݒ�
	void SetIndexBuffer();

	//���_�o�b�t�@����
	void CreateVertexBuffer();
	//���_�C���f�b�N�X����
	void CreateIndexBuffer();


};


//*********************************************************************************************************************
//
//	MeshFilter
//
//*********************************************************************************************************************
class MeshFilter : public Component<MeshFilter>
{
private:
	std::weak_ptr<Mesh> _mesh;
	unsigned int _IndexNum;
	unsigned int _IndexStartNum;

public:
	MeshFilter();

	void SetMesh(std::weak_ptr<Mesh> mesh);
	void SetIndexNum(unsigned int IndexNum,unsigned int StartNum);

	std::weak_ptr<Mesh> GetMesh() { return _mesh; };
	unsigned int GetIndexNum() { return _IndexNum; }
	unsigned int GetIndesStartNum() { return _IndexStartNum; }
};

//*********************************************************************************************************************
//
//	MeshManager
//
//*********************************************************************************************************************
class MeshManager
{
	using MeshIndex = std::map<std::string, std::shared_ptr<Mesh>>;
private:
	static MeshIndex g_MeshIndex;

public:
	static std::weak_ptr<Mesh> RegisterIndex(std::string name, Mesh* mesh);
	static void Release();
	static std::weak_ptr<Mesh> GetMesh(std::string name);


};


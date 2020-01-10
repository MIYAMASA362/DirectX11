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

	//バッファ
	ID3D11Buffer* _VertexBuffer;
	ID3D11Buffer* _IndexBuffer;

	//頂点
	VertexType* _VertexArray;
	unsigned int _VertexNum;

	//インデックス
	unsigned short* _IndexArray;
	unsigned int _IndexNum;

	//面
	Surface* _Surface;
	unsigned int _SurfaceNum;

	//コンストラクタ
	Mesh();
	//デストラクタ
	virtual ~Mesh();


public:
	//頂点バッファ設定
	void SetVertexBuffer();
	//インデックス設定
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
	//メッシュ行列 複数メッシュに対応
	MeshArray _MeshArray;


public:
	//メッシュ設定
	void AddMesh(std::weak_ptr<Mesh> mesh) { _MeshArray.emplace_back(mesh.lock()); };
	//メッシュ取得
	std::weak_ptr<Mesh> GetMesh(unsigned int index) { return _MeshArray.at(index); };
	//メッシュ数の取得
	size_t GetNumMesh() { return _MeshArray.size(); };
};
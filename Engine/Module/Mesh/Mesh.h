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

	//頂点インデックス
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
	//コンテキストに頂点バッファ設定
	void SetVertexBuffer();
	//コンテキストに頂点インデックス設定
	void SetIndexBuffer();

	//頂点バッファ生成
	void CreateVertexBuffer();
	//頂点インデックス生成
	void CreateIndexBuffer();
};


//*********************************************************************************************************************
//
//	MeshFilter
//
//*********************************************************************************************************************
class MeshFilter : public Component<MeshFilter>
{
public:
	std::weak_ptr<Mesh> _mesh;
	unsigned int _IndexNum;
	unsigned int _IndexStartNum;

	MeshFilter(EntityID OwnerID);


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


};


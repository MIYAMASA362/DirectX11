#pragma once

//レイヤー
enum MeshLayer
{
	Default
};

//三角面
struct Surface
{
	Vector3 _p1;
	Vector3 _p2;
	Vector3 _p3;

	Vector3 Normal() { return Vector3::Cross(_p2 - _p1, _p3 - _p1); };
};

//メッシュ
template<typename Type>
class Mesh
{
	friend class MeshCollider;
	friend class Physics;
	friend class Collider;
public:
	using VertexType = Type;

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
	Surface* _Surface;			//三角面
	unsigned int _SurfaceNum;	//面数

	Mesh();
	virtual ~Mesh();

	void SetVertexBuffer();
	void SetIndexBuffer();
};
	
template<typename Type>
inline Mesh<Type>::Mesh()
{

}

template<typename Type>
inline Mesh<Type>::~Mesh()
{
	if (_Surface) delete[] _Surface;

	if (_IndexArray) delete[] _IndexArray;
	if (_VertexArray) delete[] _VertexArray;

	if (_IndexBuffer) _IndexBuffer->Release();
	if (_VertexBuffer) _VertexBuffer->Release();
}

template<typename Type>
inline void Mesh<Type>::SetVertexBuffer()
{
	D3DApp::Renderer::SetVertexBuffer(_VertexBuffer,sizeof(VertexType),0);
}

template<typename Type>
inline void Mesh<Type>::SetIndexBuffer()
{
	D3DApp::Renderer::SetIndexBuffer(_IndexBuffer);
}

#pragma once

namespace DirectX
{
	struct XMMATRIX;

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
	};

	//メッシュ
	class Mesh
	{
		friend class MeshCollider;
		friend class Physics;
		friend class Collider;
	protected:
		MeshLayer _Layer;
		VERTEX_3D* _VertexArray;	//頂点情報
		unsigned int _VertexNum;	//頂点数

		Surface* _Surface;			//三角面
		unsigned int _SurfaceNum;	//面数

		Mesh();
		virtual ~Mesh();
	public:
		virtual void Render(XMMATRIX& worldMatrix) = 0;
		void SetLayer(MeshLayer layer);
	};

	inline Mesh::Mesh()
	{
		_Layer = MeshLayer::Default;
	}

	inline Mesh::~Mesh()
	{
		delete[] this->_Surface;
		delete[] this->_VertexArray;
	}

	inline void Mesh::SetLayer(MeshLayer layer)
	{
		_Layer = layer;
	}
}
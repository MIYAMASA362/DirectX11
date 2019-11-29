#pragma once

namespace DirectX
{
	struct XMMATRIX;

	//���C���[
	enum MeshLayer
	{
		Default
	};

	//�O�p��
	struct Surface
	{
		Vector3 _p1;
		Vector3 _p2;
		Vector3 _p3;
	};

	//���b�V��
	class Mesh
	{
		friend class MeshCollider;
		friend class Physics;
		friend class Collider;
	protected:
		MeshLayer _Layer;
		VERTEX_3D* _VertexArray;	//���_���
		unsigned int _VertexNum;	//���_��

		Surface* _Surface;			//�O�p��
		unsigned int _SurfaceNum;	//�ʐ�

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
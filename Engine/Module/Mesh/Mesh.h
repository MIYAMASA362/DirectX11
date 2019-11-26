#pragma once

namespace DirectX
{
	struct XMMATRIX;


	enum MeshLayer
	{
		Default
	};

	//=== Mesh ================================================================
	class Mesh
	{
		friend class MeshCollider;
	protected:
		MeshLayer _Layer = MeshLayer::Default;
		VERTEX_3D* _VertexArray;	//í∏ì_èÓïÒ
		Mesh() = default;
		virtual ~Mesh() = default;
	public:
		virtual void Render(XMMATRIX& worldMatrix) = 0;
		void SetLayer(MeshLayer layer);
	};

	void Mesh::SetLayer(MeshLayer layer)
	{
		_Layer = layer;
	}
}
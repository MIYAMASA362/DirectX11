#pragma once

namespace DirectX
{
	class Mesh;
	//=== MeshRender ==========================================================
	class MeshRender :public Renderer
	{
	private:
		std::shared_ptr<Mesh> mesh;
	public:
		MeshRender(EntityID OwnerID);
		virtual ~MeshRender();
	public:
		void Render(XMMATRIX& worldMatrix) override;
		template<typename Type> Type* SetMesh();
		Mesh* GetMesh();
	public:
		void DebugImGui() override;
	};

	//-------------------------------------------------------------------------
	inline Mesh* MeshRender::GetMesh() {
		return mesh.get();
	}
	template<typename Type> Type* MeshRender::SetMesh() {
		mesh = std::shared_ptr<Mesh>(new Type());
		return static_cast<Type*>(mesh.get());
	};
}
#pragma once

namespace DirectX
{
	class Mesh;
	//=== MeshRender ==========================================================
	class MeshRender final :public Component<MeshRender>
	{
	private:
		std::shared_ptr<Mesh> mesh;
	public:
		MeshRender();
		virtual ~MeshRender();
	public:
		void OnComponent() override;
		void Render();
		template<typename Type> Type* SetMesh();
		Mesh* GetMesh();
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
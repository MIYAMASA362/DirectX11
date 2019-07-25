#pragma once

namespace DirectX
{
	class MeshRender;

	class Mesh
	{
		friend class MeshRender;
	protected:
		virtual void Render(XMMATRIX worldMatrix)=0;
	protected:
		Mesh() {};
		virtual ~Mesh() {};
	};

	class MeshRender final:public Component
	{
	private:
		std::shared_ptr<Mesh> mesh;
	public:
		MeshRender();
		virtual ~MeshRender();
	public:
		void OnComponent() override;
		void SendBehaviourMessage(Message message) override;
		//Meshを設定
		template<typename Type> Type* SetMesh()
		{
			mesh = std::shared_ptr<Mesh>(new Type());

			return static_cast<Type*>(mesh.get());
		}
		//Meshデータの取得
		Mesh* GetMesh()
		{
			return mesh.get();
		}
	};
}

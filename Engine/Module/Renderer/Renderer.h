#pragma once

namespace DirectX
{
	struct XMMATRIX;

	//=== Renderer ============================================================
	class Renderer:public Component<Renderer>
	{
	public:
		static void BeginRender();
	private:
		bool m_IsEnable = true;
	public:
		Renderer(EntityID OwnerID,std::string name);
		virtual ~Renderer() = default;
	public:
		void SetEnable(bool enable);
		bool GetEnable();
		virtual void Render(XMMATRIX& worldMatrix)=0;
		virtual void OnComponent() override;
		void AddComponentIndex(std::weak_ptr<Renderer> instance) override;
	};
}

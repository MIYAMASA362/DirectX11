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
		Renderer(EntityID OwnerID);
		virtual ~Renderer() = default;
	public:
		void SetEnable(bool enable);
		bool GetEnable();
		virtual void Render(XMMATRIX& worldMatrix)=0;
	};
}

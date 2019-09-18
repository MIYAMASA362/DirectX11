#pragma once

namespace DirectX
{
	struct XMMATRIX;

	//=== Renderer ============================================================
	class Renderer:public Component<Renderer>
	{
	public:
		Renderer();
		virtual ~Renderer() = default;
	public:
		void Run();
		virtual void Render(XMMATRIX& worldMatrix)=0;
		virtual void OnComponent() override;
	};
}

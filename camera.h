#pragma once

namespace DirectX
{
	class CCamera :public Behaviour
	{
	private:
		static std::shared_ptr<GameObject> MainCamera;
	private:
		RECT m_Viewport;
	public:
		CCamera();
	public:
		void Initialize()override;
		void Update()override;
		void Render()override;
		void Finalize()override;
	};
}
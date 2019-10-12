#pragma once

namespace DirectX
{
	class Camera final :public Behaviour<Camera>
	{
	private:
		static Camera* pActiveCamera;
		static std::list<std::weak_ptr<Camera>> CameraIndex;
	public:
		static void Render(void(*Draw)(void),void(*Begin)(void));
		static Camera* GetActiveCamera();
	private:
		static void IndexSort(Camera* camera);
	private:
		RECT viewport;
		XMMATRIX m_ViewMatrix;
		int priority;
	public:
		Camera(EntityID OwnerID);
		virtual ~Camera();
		void SetViewPort(float x, float y,float w, float h);
		void SetPriority(int priority);
		XMMATRIX GetViewMatrix();
	//--- Inherited Members -----------------------------------------
	public:
		void Run();
		void OnComponent() override;
		void OnDestroy() override;
		void DebugImGui() override;
	};
}
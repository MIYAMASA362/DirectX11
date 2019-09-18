#pragma once

namespace DirectX
{
	class Camera final :public Behaviour<Camera>
	{
		friend class CameraManager;
	private:
		static Camera* pActiveCamera;
		static std::list<std::weak_ptr<Camera>> CameraIndex;
	public:
		static void SetRender(void(*Draw)(void),void(*Begin)(void));
		static Camera* GetActiveCamera();
		static void IndexSort(Camera* camera);
		static void RemoveCamera(Camera* camera);
		static void Release();
	private:
		RECT viewport;
		XMMATRIX m_ViewMatrix;
		int priority;
	public:
		Camera();
		virtual ~Camera();
		void SetViewPort(float x, float y,float w, float h);
		void SetPriority(int priority);
		void Finalize();
		XMMATRIX GetViewMatrix();
	//--- Inherited Members -----------------------------------------
	public:
		void Run() override;	//•`‰æ
		void OnComponent() override;
		void OnDestroy() override;
	};
}
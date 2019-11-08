#pragma once

namespace DirectX
{
	class Camera final :public Behaviour<Camera>
	{
	private:
		static Camera* pActiveCamera;
		static std::list<std::weak_ptr<Camera>> CameraIndex;

		RECT viewport;
		XMMATRIX m_ViewMatrix;
		XMMATRIX m_ProjectionMatrix;
		int priority;
	private:
		static void IndexSort(Camera* camera);

	public:
		static void Render(void(*Draw)(void),void(*Begin)(void));
		static Camera* GetActiveCamera();

		Camera(EntityID OwnerID);
		virtual ~Camera();
		void SetViewPort(float x, float y,float w, float h);
		void SetPriority(int priority);
		XMMATRIX GetViewMatrix();

		void Run();
		void OnDestroy() override;

		bool GetVisibility(Vector3 position);
	};
}
#pragma once

namespace DirectX
{
	class Camera;

	class CameraManager 
	{
	private:
		static Camera* pActiveCamera;
		static std::list<std::weak_ptr<Camera>> CameraIndex;
	private:
		CameraManager() = default;
		~CameraManager();
	public:
		static void IndexSort(Camera* target);
		static void RemoveCamera(Camera* camera);
		static void SetRender(void (*Draw)(void),void(*Begin)(void));
		static void Release();
		static Camera* GetActiveCamera();
	};

	class Camera final :public Behaviour
	{
		friend class CameraManager;
	private:
		RECT viewport;
		int priority;	//ï`âÊèá
		XMMATRIX m_ViewMatrix;
	public:
		Camera();
		virtual ~Camera();
	private:
		void Run();	//ï`âÊ
	public:
		virtual const std::type_info& GetType() override;
		void OnComponent() override;
		void OnDestroy() override;
		void SetViewPort(float x, float y,float w, float h);
		void SetPriority(int priority);
		void Finalize();
		XMMATRIX GetViewMatrix();
	};

	inline const std::type_info& Camera::GetType() {
		return typeid(*this);
	};
	inline void Camera::OnComponent(){
		CameraManager::IndexSort(this);
	};
}
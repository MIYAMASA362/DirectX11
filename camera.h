#pragma once

namespace DirectX
{
	class Camera;

	class CameraManager 
	{
	private:
		static std::list<std::weak_ptr<Camera>> CameraIndex;
		static CameraManager* pInstance;
	private:
		CameraManager() {};
		~CameraManager();
	public:
		static void Create();
		static void Destroy();
		static void IndexSort(Camera* target);
		static void RemoveCamera(Camera* camera);
		static void SetRender(void (*Draw)(void),void(*Begin)(void));
		static void Release();
	};

	class Camera final :public Behaviour
	{
		friend class CameraManager;
	private:
		RECT viewport;
		int priority;	//ï`âÊèá
	public:
		Camera();
		virtual ~Camera();
	private:
		void Run();	//ï`âÊ
	public:
		void OnComponent() override { CameraManager::IndexSort(this); };
		void OnDestroy() override;
		void SetViewPort(float x, float y,float w, float h);
		void SetPriority(int priority);
		void Finalize();
	};
}
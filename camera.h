#pragma once

namespace DirectX
{
	class Camera :public Behaviour
	{
	private:
		static std::list<std::weak_ptr<Camera>> CameraIndex;

		RECT viewport;
		int priority;	//ï`âÊèá

	public:
		Camera();
		virtual ~Camera();
	private:
		static void IndexSort(Camera* target);
	public:
		static void BeginRun(void (*Draw)(void), void(*Begin)(void));
		static void Release();
	private:
		void Run();	//ï`âÊ
	public:
		//Component
		void Initialize()override;
		void Update()override;
		void OnDestroy() override;
	
		//Method
		void SetViewPort(float x, float y,float w, float h);
		void SetPriority(int priority);
	};
}
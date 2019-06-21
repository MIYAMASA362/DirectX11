#pragma once

namespace DirectX
{
	class Camera :public Behaviour
	{
	private:
		static std::list<std::shared_ptr<Camera>> CameraIndex;

		RECT viewport;
		Color BGColor;	//”wŒiF
		int priority;	//•`‰æ‡

	public:
		Camera();
		virtual ~Camera();

	public:
		static void BeginRun(void (*Draw)(void),void (*Begin)(Color));
		static void Release();
	private:
		void Run();	//•`‰æ
	public:
		//Component
		void Initialize()override;
	
		//Method
		void SetViewPort(long x, long y, long w, long h);
		void SetPriority(int priority) { this->priority = priority; };
		void SetBGColor(Color color) { this->BGColor = color; };
	};
}
#pragma once

namespace DirectX
{
	class Camera :public Behaviour
	{
	private:
		static std::shared_ptr<Camera> MainCamera;
		static std::list<std::shared_ptr<Camera>> CameraIndex;
	private:
		RECT viewport;
		Color BGColor;	//”wŒiF
		int priority;
	public:
		Camera();
		virtual ~Camera() {};
	public:
		//Component
		void Initialize()override;
		void Update()override;
		void Render()override;
		void Finalize()override;
	public:
		//Method
		virtual void Run();	//•`‰æ
		void SetViewPort(long x, long y, long w, long h);
		void SetPriority(int priority) { this->priority = priority; };
		void SetBGColor(Color color) { this->BGColor = color; };
		void Destroy() override;
	};
}
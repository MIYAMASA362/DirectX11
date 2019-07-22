#pragma once

namespace DirectX
{
	
	class Time
	{
		friend class TimeManager;
	public:
		Time() {};
		~Time() {};
	public:
		static float Get_DeltaTime();
		static float Get_FixedTime();
		static float Get_Time();
		static float Get_TimeScale();
		static float Get_FixedDeltaTime();
		static int Get_SecGameTime();
	public:
		static void Set_TimeScale(float TimeScale);
		static void Set_FixedScale(DWORD fixedtime);
	};

	class TimeManager
	{
		friend class Time;
	private:
		static TimeManager* pInstance;
	private:
		float TimeScale;		//���Ԕ{��
		DWORD dwStartTime;		//�J�n����
		const DWORD fps;		//�ڕWFPS
		DWORD dwCurrentTime;	//����

		DWORD dwFpsLastTime;	//�OFPS�J�E���^�X�V����
		int FpsCounter;			//FPS�J�E���^
		int Fps;				//��FPS

		//Main
		DWORD dwExecLastTime;	//�ŏI�X�V����
		DWORD dwDeltaTime;		//Update�Ԋu����

		//Fixed
		DWORD dwFixedTime;		//��莞��
		DWORD dwFixedLastTime;	//FixedUpdate�ŏI�X�V����
		DWORD dwFixedDeltaTime;	//FixedUpdate�Ԋu����
	private:
		TimeManager(unsigned long fps);
		~TimeManager() {};
	public:
		static void Create(unsigned long fps);
		static void Destroy();
	public:
		static bool IsUpdate();
		static bool IsFixedUpdate();
		static void Update();
		static void DebugGUI_Time();
	};
}
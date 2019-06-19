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
		const DWORD fps;
		DWORD dwCurrentTime;	//����

		//Main
		DWORD dwExecLastTime;
		DWORD dwDeltaTime;

		//Fixed
		DWORD dwFixedTime;
		DWORD dwFixedLastTime;
		DWORD dwFixedDeltaTime;
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
	};
}
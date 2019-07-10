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
		float TimeScale;		//時間倍率
		DWORD dwStartTime;		//開始時間
		const DWORD fps;		//目標FPS
		DWORD dwCurrentTime;	//現在

		DWORD dwFpsLastTime;	//前FPSカウンタ更新時間
		int FpsCounter;			//FPSカウンタ
		int Fps;				//実FPS

		//Main
		DWORD dwExecLastTime;	//最終更新時間
		DWORD dwDeltaTime;		//Update間隔時間

		//Fixed
		DWORD dwFixedTime;		//一定時間
		DWORD dwFixedLastTime;	//FixedUpdate最終更新時間
		DWORD dwFixedDeltaTime;	//FixedUpdate間隔時間
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
#include"main.h"
#include "Time.h"

#define SET_SECOND_TIME (0.001f)

using namespace DirectX;

TimeManager* TimeManager::pInstance = NULL;

//--- TimeManager -------------------------------------------------------------

TimeManager::TimeManager(unsigned long fps)
	:
	TimeScale(1.0f),
	dwStartTime(0),
	fps(1000/fps),
	dwCurrentTime(0),
	dwExecLastTime(0),
	dwDeltaTime(0),
	dwFixedTime(1000/(fps/2)),
	dwFixedLastTime(0)
{

}

void TimeManager::Create(unsigned long fps)
{
	if (pInstance) return;
	pInstance = new TimeManager(fps);
	timeBeginPeriod(TRUE);

	DWORD time = timeGetTime();

	pInstance->dwExecLastTime = time;
	pInstance->dwCurrentTime = 0;

	pInstance->dwStartTime = time;
	pInstance->dwFixedLastTime = time;
}

void TimeManager::Destroy()
{
	if (!pInstance) return;
	timeEndPeriod(TRUE);
	delete pInstance;
}

bool TimeManager::IsUpdate()
{
	if(!pInstance) return false;

	DWORD frameTime = pInstance->dwCurrentTime - pInstance->dwExecLastTime;
	if (frameTime < pInstance->fps) return false;
	pInstance->dwDeltaTime = frameTime;
	pInstance->dwExecLastTime = pInstance->dwCurrentTime;

	return true;
}

bool TimeManager::IsFixedUpdate()
{
	if (!pInstance) return false;
	if (pInstance->TimeScale <= 0.0f) return false;

	DWORD frameTime = pInstance->dwCurrentTime - pInstance->dwFixedLastTime;
	if (frameTime < pInstance->dwFixedTime * pInstance->TimeScale) return false;
	pInstance->dwFixedDeltaTime = frameTime;
	pInstance->dwFixedLastTime = pInstance->dwCurrentTime;

	return true;
}

void TimeManager::Update()
{
	if (!pInstance) return;
	pInstance->dwCurrentTime = timeGetTime();
}

//--- Time --------------------------------------------------------------------

float Time::Get_DeltaTime()
{
	return TimeManager::pInstance->dwDeltaTime * TimeManager::pInstance->TimeScale * SET_SECOND_TIME;	
}

float Time::Get_FixedTime()
{
	return TimeManager::pInstance->dwFixedTime * SET_SECOND_TIME;
}

float Time::Get_Time()
{
	return (TimeManager::pInstance->dwStartTime - timeGetTime()) * SET_SECOND_TIME;
}

float Time::Get_TimeScale()
{
	return TimeManager::pInstance->TimeScale;
}

float Time::Get_FixedDeltaTime()
{
	return TimeManager::pInstance->dwFixedDeltaTime * SET_SECOND_TIME;
}

void Time::Set_TimeScale(float TimeScale)
{
	TimeManager::pInstance->TimeScale = TimeScale;
}

void Time::Set_FixedScale(DWORD fixedtime)
{
	TimeManager::pInstance->dwFixedTime = fixedtime;
}


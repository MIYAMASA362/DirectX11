#include<Windows.h>
#include "Time.h"

#include"Module\IMGUI\GUI_ImGui.h"

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
	FpsCounter(0),
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
	pInstance->FpsCounter++;

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
	
	if ((pInstance->dwCurrentTime - pInstance->dwFpsLastTime) >= 1000){
		pInstance->dwFpsLastTime = pInstance->dwCurrentTime;
		pInstance->Fps = pInstance->FpsCounter;
		pInstance->FpsCounter = 0;
	}

	pInstance->dwCurrentTime = timeGetTime();
}

void DirectX::TimeManager::DebugGUI_Time()
{
	ImGui::Begin("Time");
	ImGui::Text("FPS:%d",pInstance->Fps);
	ImGui::Text("deltaTime %f", Time::Get_DeltaTime());
	ImGui::Text("fixedTime %f", Time::Get_FixedDeltaTime());
	ImGui::End();
}

//--- Time --------------------------------------------------------------------

float Time::Get_DeltaTime()
{
	if (TimeManager::pInstance == nullptr) return 0.0f;
	return TimeManager::pInstance->dwDeltaTime * TimeManager::pInstance->TimeScale * SET_SECOND_TIME;	
}

float Time::Get_FixedTime()
{
	if (TimeManager::pInstance == nullptr) return 0.0f;
	return TimeManager::pInstance->dwFixedTime * SET_SECOND_TIME;
}

float Time::Get_Time()
{
	if (TimeManager::pInstance == nullptr) return 0.0f;
	return (TimeManager::pInstance->dwStartTime - timeGetTime()) * SET_SECOND_TIME;
}

float Time::Get_TimeScale()
{
	if (TimeManager::pInstance == nullptr) return 0.0f;
	return TimeManager::pInstance->TimeScale;
}

float Time::Get_FixedDeltaTime()
{
	if (TimeManager::pInstance == nullptr) return 0.0f;
	return TimeManager::pInstance->dwFixedDeltaTime * SET_SECOND_TIME;
}

int DirectX::Time::Get_SecGameTime()
{
	if (TimeManager::pInstance == nullptr) return 0;
	return (TimeManager::pInstance->dwCurrentTime - TimeManager::pInstance->dwStartTime) / 1000;
}

void Time::Set_TimeScale(float TimeScale)
{
	if (TimeManager::pInstance == nullptr) return;
	TimeManager::pInstance->TimeScale = TimeScale;
}

void Time::Set_FixedScale(DWORD fixedtime)
{
	if (TimeManager::pInstance == nullptr) return;
	TimeManager::pInstance->dwFixedTime = fixedtime;
}
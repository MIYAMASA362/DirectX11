#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

class Input
{
private:
	static HWND _hWnd;

	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
	static POINT m_MousePos;
	static bool IsCursorLoop;

	static LPDIRECTINPUT8 m_pInput;
	static LPDIRECTINPUTDEVICE8 m_pDevMouse;
	static DIMOUSESTATE2 m_MouseState2;
	static BYTE m_OldMouseButtons[8];
public:
	//引数:VK_@@@ '@'
	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyCodePress(BYTE keyCode);
	static bool GetKeyTrigger(BYTE KeyCode);
	static bool GetKeyUp(BYTE KeyCode);

	static void DebugGUI();
	static void Initialize(HWND hWnd);
	static void Update();
	static void Finalize();
public:
	//マウス
	class Mouse
	{
	public:
		static void SetMouseShow();
		static void SetMouseHide();
		static void SetScreenLoop(bool IsLoop);
	public:
		static float GetMouseX();
		static float GetMouseY();
	public:
		static bool IsLeftDown();
		static bool IsRightDown();
		static bool IsWheelDown();
		static bool IsLeftTrigger();
		static bool IsRightTrigger();
		static bool IsWheelTrigger();
	public:
		static float GetAccelerationX();
		static float GetAccelerationY();
		static float GetAccelerationZ();
	};
};
#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

namespace DirectX
{
	class Input
	{
	private:
		static BYTE m_OldKeyState[256];
		static BYTE m_KeyState[256];
		static POINT m_MousePos;
		static bool IsCursorLoop;

		static LPDIRECTINPUT8 m_pInput;
		static LPDIRECTINPUTDEVICE8 m_pDevMouse;
		static DIMOUSESTATE2 m_MouseState2;
	public:
		static void Init();
		static void Uninit();
		static void Update();

		//à¯êî:VK_@@@ '@'
		static bool GetKeyPress(BYTE KeyCode);
		static bool GetKeyCodePress(BYTE keyCode);
		static bool GetKeyTrigger(BYTE KeyCode);
		static bool GetKeyUp(BYTE KeyCode);

	public:
		static void DebugGUI();
	public:
		//É}ÉEÉX
		class Mouse
		{
		public:
			static void SetMouseShow();
			static void SetMouseHide();
			static void SetScreenLoop(bool IsLoop);
		public:
			static float GetMouseX();
			static float GetMouseY();
			static bool IsLeftDown();
			static bool IsRightDown();
			static bool IsWheelDown();
			static float GetAccelerationX();
			static float GetAccelerationY();
			static float GetAccelerationZ();
		};
	};
}
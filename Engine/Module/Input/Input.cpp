#include<stdio.h>
#include<string>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include "input.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

using namespace DirectX;

//入力値に対しては特殊なVK_@@@を利用

BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];
POINT Input::m_MousePos;
bool Input::IsCursorLoop = false;

LPDIRECTINPUT8		 Input::m_pInput = nullptr;
LPDIRECTINPUTDEVICE8 Input::m_pDevMouse = nullptr;
DIMOUSESTATE2		 Input::m_MouseState2 = { NULL };

void Input::Init()
{
	//キーボード設定
	memset(m_OldKeyState, 0, 256);
	memset(m_KeyState, 0, 256);

	HRESULT hr;

	hr = DirectInput8Create(
		D3DApp::GethInstance(),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInput,
		NULL
	);

	if(FAILED(hr))
	{
		MessageBox(NULL,"Inputデバイスの生成に失敗しました。","エラー",MB_OK);
		return;
	}

	hr = m_pInput->CreateDevice(GUID_SysMouse, &m_pDevMouse, NULL);

	hr = m_pDevMouse->SetDataFormat(&c_dfDIMouse2);

	hr = m_pDevMouse->SetCooperativeLevel(D3DApp::GetWindow(),(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

	m_pDevMouse->Acquire();
}

void Input::Uninit()
{
	if(m_pDevMouse != nullptr)
	{
		m_pDevMouse->Unacquire();
		m_pDevMouse->Release();
		m_pDevMouse = nullptr;
	}

	if(m_pInput != nullptr){
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

void Input::Update()
{
	//キー処理
	{
		memcpy(m_OldKeyState, m_KeyState, 256);

		GetKeyboardState(m_KeyState);
	}

	//マウス処理
	{
		//マウス情報
		GetCursorPos(&m_MousePos);
		//カーソルループ
		if (!Input::GetKeyPress(VK_CONTROL) && IsCursorLoop) {
			WINDOWINFO info;
			GetWindowInfo(D3DApp::GetWindow(),&info);
			//--- 左右 -------------------------------------------------------------------
			if ((UINT)m_MousePos.x <= info.rcWindow.left + info.cxWindowBorders)
				SetCursorPos(info.rcWindow.right - info.cxWindowBorders, m_MousePos.y);
			else if ((UINT)m_MousePos.x >= info.rcWindow.right - info.cxWindowBorders)
				SetCursorPos(info.rcClient.left + info.cxWindowBorders,m_MousePos.y);
			//--- 上下 -------------------------------------------------------------------
			if ((UINT)m_MousePos.y <= info.rcWindow.top + info.cyWindowBorders)
				SetCursorPos(m_MousePos.x, info.rcWindow.bottom - info.cyWindowBorders);
			else if ((UINT)m_MousePos.y >= info.rcWindow.bottom - info.cyWindowBorders)
				SetCursorPos(m_MousePos.x,info.rcWindow.top + info.cyWindowBorders);
			
			GetCursorPos(&m_MousePos);
		}
		ScreenToClient(D3DApp::GetWindow(), &m_MousePos);

		//DirectInput
		if (FAILED(m_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState2)))
			m_pDevMouse->Acquire();
	}
}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80) != 0 ? true:false;
}

bool Input::GetKeyCodePress(BYTE keyCode){
	return (m_KeyState[keyCode] & 0x80) != 0 ? true:false;
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::GetKeyUp(BYTE KeyCode)
{
	return (!(m_KeyState[KeyCode] & 0x80) && (m_OldKeyState[KeyCode]&0x80));
}

void DirectX::Input::DebugGUI()
{

}

float DirectX::Input::Mouse::GetMouseX()
{
	return (float)m_MousePos.x;
}

float DirectX::Input::Mouse::GetMouseY()
{
	return (float)m_MousePos.y;
}

bool DirectX::Input::Mouse::IsLeftDown()
{
	return (m_MouseState2.rgbButtons[0] & 0x80) ? true : false;
}

bool DirectX::Input::Mouse::IsRightDown()
{
	return (m_MouseState2.rgbButtons[1] & 0x80) ? true : false;
}

bool DirectX::Input::Mouse::IsWheelDown()
{
	return (m_MouseState2.rgbButtons[2] & 0x80) ? true : false;
}

void DirectX::Input::Mouse::SetMouseShow()
{
	ShowCursor(true);
}

void DirectX::Input::Mouse::SetMouseHide()
{
	ShowCursor(false);
}

void DirectX::Input::Mouse::SetScreenLoop(bool IsLoop)
{
	IsCursorLoop = IsLoop;
}

float DirectX::Input::Mouse::GetAccelerationX()
{
	return (float)m_MouseState2.lX;
}

float DirectX::Input::Mouse::GetAccelerationY()
{
	return (float)m_MouseState2.lY;
}

float DirectX::Input::Mouse::GetAccelerationZ()
{
	return (float)m_MouseState2.lZ;
}

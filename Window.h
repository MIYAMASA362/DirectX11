#pragma once

namespace System
{
	class Window
	{
	private:
		HWND hWnd;
		WNDCLASSEX WndClass;
	public:
		Window();
		~Window();
	public:
		HRESULT Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style);
		HRESULT Destroy();
	public:
		HWND Get_Window();
	};
}
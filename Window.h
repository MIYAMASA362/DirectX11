#pragma once

namespace System
{
	class Window
	{
	protected:
		HWND hWnd;
		WNDCLASSEX WndClass;
	public:
		Window();
		~Window();
	public:
		virtual HRESULT Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style);
		HRESULT Destroy();
	public:
		HWND Get_Window();
	};
	
#ifdef GUI_ImGui_H
	//IMGUIウィンドウのマウス移動に対応したウィンドウ
	class ImGui_Window:public Window
	{
	public:
		HRESULT Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style)override;
	};
#endif
}
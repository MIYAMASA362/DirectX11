#pragma once

namespace System
{
	class Window
	{
	protected:
		HWND hWnd;
		WNDCLASSEX WndClass;
		HINSTANCE hInstance;
	public:
		Window();
		~Window();
	public:
		virtual HRESULT Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style);
		HRESULT Destroy();
	public:
		HWND Get_Window() { return hWnd; }
		HINSTANCE Get_hInstance() { return hInstance; }
	};
	
#ifdef GUI_ImGui_H
	//IMGUI�E�B���h�E�̃}�E�X�ړ��ɑΉ������E�B���h�E
	class ImGui_Window:public Window
	{
	public:
		HRESULT Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style)override;
	};
#endif
}
#include<Windows.h>

#include"Module\IMGUI\GUI_ImGui.h"
#include"Window.h"

using namespace System;

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
#ifdef GUI_ImGui_H
LRESULT CALLBACK exImGui_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

Window::Window()
{

}

Window::~Window()
{
	
}

HRESULT Window::Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style)
{
	this->WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		lpClassName,
		NULL
	};

	if (!RegisterClassEx(&this->WndClass)) {
		MessageBox(NULL, "WndClassの設定に失敗しました。", "失敗", MB_OK);
		return E_FAIL;
	}

	this->hWnd = CreateWindowEx(
		0,
		this->WndClass.lpszClassName,
		lpCaption,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		this->WndClass.hInstance,
		NULL
	);

	if(!this->hWnd){
		MessageBox(NULL,"hWndの設定に失敗しました。","失敗",MB_OK);
		return E_FAIL;
	}

	ShowWindow(this->hWnd,SW_SHOW);
	UpdateWindow(this->hWnd);
	return S_OK;
}

HRESULT Window::Destroy()
{
	UnregisterClass(this->WndClass.lpszClassName,this->WndClass.hInstance);
	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_KEYDOWN:{
		switch (wParam)
		{
		case VK_ESCAPE:{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}break;
		default:
			break;
		}
	}break;
	case WM_CLOSE:{
		if(MessageBox(NULL,"本当に終了してもよろしいですか？","確認",MB_OKCANCEL || MB_DEFBUTTON2) == IDOK){
			DestroyWindow(hWnd);
		}
	}break;
	case WM_DESTROY:{
		PostQuitMessage(0);
	}break;
	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	return 0;
}

#ifdef GUI_ImGui_H
HRESULT System::ImGui_Window::Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style)
{
	this->WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		exImGui_WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		lpClassName,
		NULL
	};

	if (!RegisterClassEx(&this->WndClass)) {
		MessageBox(NULL, "WndClassの設定に失敗しました。", "失敗", MB_OK);
		return E_FAIL;
	}

	this->hWnd = CreateWindowEx(
		0,
		this->WndClass.lpszClassName,
		lpCaption,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		this->WndClass.hInstance,
		NULL
	);

	if (!this->hWnd) {
		MessageBox(NULL, "hWndの設定に失敗しました。", "失敗", MB_OK);
		return E_FAIL;
	}

	ShowWindow(this->hWnd, SW_SHOWDEFAULT);
	UpdateWindow(this->hWnd);

	this->hInstance = hInstance;

	return S_OK;
}

//IMGUIの入力のやつを追加
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK exImGui_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_CREATE: 
	{

	}
	break;
	case WM_KEYDOWN: {
		switch (wParam)
		{
		case VK_ESCAPE: {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}break;
		default:
			break;
		}
	}break;
	case WM_CLOSE: {
		if (MessageBox(NULL, "本当に終了してもよろしいですか？", "確認", MB_OKCANCEL || MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd);
		}
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}
#endif

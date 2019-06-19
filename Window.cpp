#include<Windows.h>

#include"Window.h"

using namespace System;

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

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
		MessageBox(NULL, "WndClass�̐ݒ�Ɏ��s���܂����B", "���s", MB_OK);
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
		MessageBox(NULL,"hWnd�̐ݒ�Ɏ��s���܂����B","���s",MB_OK);
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


HWND Window::Get_Window()
{
	return this->hWnd;
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
		if(MessageBox(NULL,"�{���ɏI�����Ă���낵���ł����H","�m�F",MB_OKCANCEL || MB_DEFBUTTON2) == IDOK){
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
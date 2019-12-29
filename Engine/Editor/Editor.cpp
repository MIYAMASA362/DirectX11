#include<Windows.h>
#include<string>
#include"Module\DirectX\DirectX.h"


#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"
#include "Editor.h"

using namespace DirectX;
using namespace System;

using namespace Editor;

//EditorSubWindow
//	コンストラクタ
//
Editor::EditorSubWindow::EditorSubWindow()
{

}

//~EditorSubWindow
//	デストラクタ
//
Editor::EditorSubWindow::~EditorSubWindow()
{

}

//Create
//	サブウィンドウ生成
//
HRESULT EditorSubWindow::Create(EditorWindow * parent, LPSTR lpClassName, LPSTR lpCaption,int x,int y, long width, long height,DWORD style)
{
	//WndClass設定
	this->_WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		parent->Get_hInstance(),
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		lpClassName,
		NULL
	};

	if(!RegisterClassEx(&this->_WndClass)){
		MessageBox(NULL,"WndClassの設定に失敗しました。","EditorSubWindow",MB_OK);
		return E_FAIL;
	}

	//ウィンドウ生成
	this->_hWnd = CreateWindowEx(
		0,
		this->_WndClass.lpszClassName,
		lpCaption,
		style,
		x,
		y,
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		parent->Get_Window(),
		NULL,
		this->_WndClass.hInstance,
		this	//自身のポインタ設定 プロシージャに渡す
	);

	if (!this->_hWnd) {
		MessageBox(NULL, "hWndの設定に失敗しました。", "失敗", MB_OK);
		return E_FAIL;
	}

	//表示
	ShowWindow(this->_hWnd, SW_SHOW);
	UpdateWindow(this->_hWnd);

	return S_OK;
}

//localWndProc
//	サブウィンドウ用ウィンドウプロシージャ
//
//	SendMessage()	別ウィンドウへメッセージの送信
//
LRESULT EditorSubWindow::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//メッセージ
	switch (uMsg)
	{
		//キー入力
		case WM_KEYDOWN:
			switch (wParam)
			{
			case VK_ESCAPE:
				SendMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			default:
				break;
			}
			break;

		//ウィンドウ閉じる
		case WM_CLOSE:
			if ((this->_WindowFlag & WindowFlags_CloseCheck) == WindowFlags_CloseCheck)
				if (MessageBox(NULL, "本当に終了してもよろしいですか？", "確認", MB_OKCANCEL || MB_DEFBUTTON2) != IDOK)
					break;
			DestroyWindow(hWnd);
			break;
		//デフォルト
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}




//EditorWindow
//	コンストラクタ
//
EditorWindow::EditorWindow()
{
	_SubWindow = new EditorSubWindow();
}

//~EditorWindow
//	デストラクタ
//
EditorWindow::~EditorWindow()
{
	delete _SubWindow;
}

//Create
//	エディタウィンドウ生成
//
HRESULT EditorWindow::Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style)
{
	HRESULT hr;
	RECT rect;

	//親ウィンドウ
	hr = Window::Create(hInstance,lpClassName,lpCaption,width,height,style);
	if (FAILED(hr)) return hr;
	GetWindowRect(this->_hWnd,&rect);

	//サブウィンドウ
	hr = _SubWindow->Create(this,"SubWindow1","SubWindow",rect.right, rect.top,256,height,style);
	if (FAILED(hr)) return hr;

	return hr;
}

//MessageLoop
//	メッセージループ
//
WPARAM EditorWindow::MessageLoop()
{
	MSG msg;

	//Applicationの生成
	D3DApp::Create(
		this->Get_Window(),
		this->Get_hInstance(),
		60
	);

	CManager::Initialize();

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// メッセージの翻訳とディスパッチ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			CManager::Run();
		}
	}
	while (msg.message != WM_QUIT);

	CManager::Finalize();

	D3DApp::Destroy();

	return msg.wParam;
}

#include<Windows.h>

#include"Module\IMGUI\GUI_ImGui.h"
#include"Window.h"

using namespace System;

//Window
//	コンストラクタ
//
Window::Window()
{

}

//~Window
//	デストラクタ
//
Window::~Window()
{
	
}

//Create
//	ウィンドウ生成
//
HRESULT Window::Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style)
{
	//WndClass設定
	this->_WndClass = {
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

	if (!RegisterClassEx(&this->_WndClass)) {
		MessageBox(NULL, "WndClassの設定に失敗しました。", "失敗", MB_OK);
		return E_FAIL;
	}

	char text[256];
	sprintf_s(text, " PID:%u", GetCurrentProcessId());

	//ウィンドウ生成

	this->_hWnd = CreateWindowEx(
		0,
		this->_WndClass.lpszClassName,
		lpCaption,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		this->_WndClass.hInstance,
		this	//自身のポインタ設定 プロシージャに渡す
	);

	if(!this->_hWnd){
		MessageBox(NULL,"hWndの設定に失敗しました。","失敗",MB_OK);
		return E_FAIL;
	}

	this->_hInstance = hInstance;

	//表示
	ShowWindow(this->_hWnd, SW_SHOW);
	UpdateWindow(this->_hWnd);

	return S_OK;
}

//Destroy
//	ウィンドウ破棄
//
HRESULT Window::Destroy()
{
	UnregisterClass(this->_WndClass.lpszClassName,this->_WndClass.hInstance);
	return S_OK;
}

//localWndProc
//	ローカルなウィンドウプロシージャ
//
LRESULT System::Window::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//ドロップ
	HDROP hDrop;
	UINT FileNum = 0;

	char fileName[256];

	//メッセージ
	switch (uMsg)
	{
		//生成
	case WM_CREATE:
		//ファイルのドラッグ・ドロップ許可
		if((this->_WindowFlag & WindowFlags_DragDropFile) == WindowFlags_DragDropFile)
			DragAcceptFiles(this->_hWnd, TRUE);
		break;

		//ドロップファイル
	case WM_DROPFILES:
		if ((this->_WindowFlag & WindowFlags_DragDropFile) == WindowFlags_DragDropFile)
		{
			//ドロップ
			hDrop = (HDROP)wParam;

			//ドロップされたファイル数取得
			FileNum = DragQueryFile(
				hDrop,
				0xFFFFFFFF,
				NULL,
				0
			);

			//ファイル
			for (UINT i = 0; i < FileNum; i++)
			{
				DragQueryFile(hDrop, i, fileName, sizeof(fileName));
				if (ShellExecute(hWnd, NULL, fileName, NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32)
					MessageBox(hWnd, "ファイルを開けませんでした。", "失敗", MB_OK);
			}

			//ドロップ解放
			DragFinish(hDrop);
		}
		break;

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

		//ウィンドウ削除
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		//デフォルト
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

//MessageLoop
//	メッセージループ　イベントドリブン
//
WPARAM System::Window::MessageLoop()
{
	//MainLoop
	MSG msg;

	do
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			// メッセージの翻訳とディスパッチ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (msg.message != WM_QUIT);

	return msg.wParam;
}

//SetHWndPointer
//	HWNDを設定
//
void System::Window::SetHWndPointer(HWND hWnd)
{
	SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)this);
	this->_hWnd = hWnd;
}

#ifdef GUI_ImGui_H
//ImGuiのウィンドウ移動
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

//WndProc
//	グローバル ウィンドウプロシージャ
//
LRESULT CALLBACK Window::WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
#ifdef GUI_ImGui_H
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
#endif

	Window* window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if(uMsg == WM_CREATE)
	{
		//ユーザーが設定したParamを利用する
		window = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;

		if(window != nullptr)
		{
			window->SetHWndPointer(hWnd);
		}
	}

	if(window != nullptr)
	{
		return window->localWndProc(hWnd,uMsg,wParam,lParam);
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

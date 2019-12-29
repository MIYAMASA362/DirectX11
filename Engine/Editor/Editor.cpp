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
	//サブウィンドウ設定
	this->_WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		parent->Get_hInstance(),
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_APPWORKSPACE + 1),
		NULL,
		lpClassName,
		NULL
	};

	//サブウィンドウ登録
	if(!RegisterClassEx(&this->_WndClass)){
		MessageBox(NULL,"WndClassの設定に失敗しました。","EditorSubWindow",MB_OK);
		return E_FAIL;
	}

	//MDIウィンドウ設定
	this->_WndClass.lpfnWndProc = DefMDIChildProc;
	this->_WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	this->_WndClass.lpszClassName = "MDICHILD";

	//MDIウィンドウ登録
	if (!RegisterClassEx(&this->_WndClass)){
		MessageBox(NULL, "WndClassの設定に失敗しました。", "EditorSubWindow", MB_OK);
		return E_FAIL;
	}

	//ウィンドウ生成
	this->_hWnd = CreateWindowEx(
		0,
		lpClassName,
		lpCaption,
		style,
		x,
		y,
		width  + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		parent->Get_Window(),
		NULL,
		this->_WndClass.hInstance,
		this//this	//自身のポインタ設定 プロシージャに渡す
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
	//サブウィンドウ取得
	EditorSubWindow* subWindow = (EditorSubWindow*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	CLIENTCREATESTRUCT ccsClient;
	MDICREATESTRUCT mdic;

	//メッセージ
	switch (uMsg)
	{
		//ウィンドウ生成
		case WM_CREATE:
			//MDIを有効化
			if ((this->_WindowFlag & WindowFlags_MultipleDocumentInterface) == WindowFlags_MultipleDocumentInterface)
			{
				ccsClient.hWindowMenu = NULL;
				ccsClient.idFirstChild = 0x100;

				subWindow->_MDIClient = new HWND;
				*subWindow->_MDIClient = CreateWindow(
					"MDICLIENT",
					NULL,
					WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
					0,
					200,
					0,
					0,
					hWnd,
					(HMENU)1,
					this->_hInstance,
					&ccsClient
				);

				//MDIウィンドウ
				mdic.szClass = "MDICHILD";
				mdic.szTitle = "MDIWindow";
				mdic.style = NULL;
				mdic.cx = 200;
				mdic.cy = 200;
				mdic.x = 0;
				mdic.y = 0;
				mdic.hOwner = this->_hInstance;
				mdic.lParam = NULL;

				SendMessage(*subWindow->_MDIClient,WM_MDICREATE,NULL,(LPARAM)&mdic);
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

		//デフォルト
		default:
			if(subWindow->_MDIClient != nullptr)
				return DefFrameProc(hWnd, *subWindow->_MDIClient,uMsg, wParam, lParam);
			else
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
	RECT rect;			//メインウィンドウRECT

	//メインウィンドウ
	this->_WindowFlag |= WindowFlags_CloseCheck ^ WindowFlags_DragDropFile;
	hr = Window::Create(hInstance,lpClassName,lpCaption,width,height,style);
	if (FAILED(hr)) return hr;
	GetWindowRect(this->_hWnd,&rect);

	//サブウィンドウ
	_SubWindow->GetWindowFlags() |= WindowFlags_MultipleDocumentInterface;
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

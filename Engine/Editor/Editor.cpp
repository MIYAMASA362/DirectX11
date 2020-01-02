#include<Windows.h>
#include<string>
#include<thread>
#include"Module\DirectX\DirectX.h"

#include"../resource.h"

#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"

#include"Editor\MDIWindow.h"
#include"TreeView.h"

#include "Editor.h"

using namespace DirectX;
using namespace System;

using namespace Editor;

//*********************************************************************************************************************
//
//	EditorSubWindow
//
//*********************************************************************************************************************

//EditorSubWindow
//	コンストラクタ
//
EditorSubWindow::EditorSubWindow(EditorWindow* editor)
	:
	_MainEditor(editor)
{

}

//~EditorSubWindow
//	デストラクタ
//
EditorSubWindow::~EditorSubWindow()
{
	delete _FileTree;
	delete _MDIWindow;
}

//Create
//	サブウィンドウ生成
//
HRESULT EditorSubWindow::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	//サブウィンドウ設定
	this->_WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_APPWORKSPACE + 1),
		MAKEINTRESOURCE(IDR_MENU1),
		lpClassName,
		NULL
	};

	//サブウィンドウ登録
	if(!RegisterClassEx(&this->_WndClass)){
		MessageBox(NULL,"WndClassの設定に失敗しました。","EditorSubWindow",MB_OK);
		return E_FAIL;
	}

	//ウィンドウ生成
	this->_hWnd = CreateWindowEx(
		0,
		lpClassName,
		lpCaption,
		style | WS_CLIPCHILDREN,
		x,
		y,
		width  + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		hParent,
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
	
	//メッセージ
	switch (uMsg)
	{
		//ウィンドウ生成
		case WM_CREATE:
			//MDIを有効化
			if ((this->_WindowFlag & WindowFlags_MultipleDocumentInterface) == WindowFlags_MultipleDocumentInterface)
			{
				//MDIウィンドウ設定
				this->_MDIWindow = new MDIWindow();
				if (FAILED(this->_MDIWindow->Create(hWnd,this->_hInstance, "MDIWindow", "MDIWindow", 0, 0,300,300,WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_VISIBLE))) {
					MessageBox(NULL, "MDIWindowの設定に失敗しました", "EditorSubWindow", MB_OK);
					return E_FAIL;
				}
			}

			this->_FileTree = new FileTreeView();
			_FileTree->Create(hWnd,this->_hInstance,"FileTree","FileView",0,0,500,300, WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CHILD);
			break;

		//サイズ
		case WM_SIZE:
			RECT rect;
			GetWindowRect(this->_FileTree->Get_Window(),&rect);
			MoveWindow(this->_FileTree->Get_Window(),0,HIWORD(lParam) - 300,LOWORD(lParam),300,TRUE);
			break;

		//コマンド
		case WM_COMMAND:

			switch (LOWORD(wParam))
			{
				//ウィンドウ => 新規作成
				case ID_WINDOW_40003:
					//MDIウィンドウの生成
					if ((this->_WindowFlag & WindowFlags_MultipleDocumentInterface) == WindowFlags_MultipleDocumentInterface)
						this->_MDIWindow->CreateMDI("Window",0,0,200,200,NULL);
					break;
				
				//終了処理
				case ID_40004:
					SendMessage(hWnd, WM_DESTROY, 0, 0);
					break;

				//ゲーム実行
				case ID_EDITOR_RUN:
					this->_MainEditor->Start();
					break;

				//ゲーム停止
				case ID_EDITOR_STOP:
					this->_MainEditor->Stop();
					break;

				default:
					break;
			}
			break;

		//キー入力
		case WM_KEYDOWN:
			switch (wParam)
			{
				//スペース
				case VK_ESCAPE:
					SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				//F1
				case VK_F1:
					ShowWindow(hWnd, SW_SHOW);
					ShowWindow(hWnd, SW_RESTORE);
					SendMessage(this->_FileTree->Get_Window(),uMsg,wParam,lParam);
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
			ShowWindow(hWnd, SW_HIDE);
			break;

		//削除処理
		case WM_DESTROY:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;

		//デフォルト
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}





//*********************************************************************************************************************
//
//	EditorWindow
//
//*********************************************************************************************************************


//EditorWindow
//	コンストラクタ
//
EditorWindow::EditorWindow()
{
	_SubWindow = new EditorSubWindow(this);
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
HRESULT EditorWindow::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	HRESULT hr;
	RECT rect;			//メインウィンドウRECT

	//メインウィンドウ
	this->_WindowFlag |= WindowFlags_CloseCheck ^ WindowFlags_DragDropFile ^ WindowFlags_PreviewProcessID;
	hr = Window::Create(hParent,hInstance, lpClassName, lpCaption, CW_USEDEFAULT,CW_USEDEFAULT,width, height, style);
	if (FAILED(hr)) return hr;
	GetWindowRect(this->_hWnd, &rect);

	//サブウィンドウ
	//_SubWindow->GetWindowFlags() |= WindowFlags_MultipleDocumentInterface;
	hr = _SubWindow->Create(this->_hWnd,hInstance, "SubWindow1", "SubWindow", rect.right, rect.top, 400, height, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
	if (FAILED(hr)) return hr;

	return hr;
}

LRESULT Editor::EditorWindow::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		if ((this->_WindowFlag & WindowFlags_DragDropFile) == WindowFlags_DragDropFile)
			DragAcceptFiles(this->_hWnd, TRUE);
		break;

		//フォーカスされた
	case WM_SETFOCUS:
		//最前面に表示する
		//SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
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
			//Space
		case VK_ESCAPE:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
			//F1
		case VK_F1:
			SendMessage(this->_SubWindow->Get_Window(),uMsg,wParam,lParam);
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
//	メッセージループ
//
WPARAM EditorWindow::MessageLoop()
{
	MSG msg;

	std::thread thread([this]() { this->RunProcces(); });

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// メッセージの翻訳とディスパッチ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	while (msg.message != WM_QUIT);

	_threadEnd = true;

	thread.join();

	return msg.wParam;
}

//RunProcces
//	ゲームウィンドウ用のスレッド
//
void Editor::EditorWindow::RunProcces()
{
	//Applicationの生成
	D3DApp::Create(
		this->Get_Window(),
		this->Get_hInstance(),
		60
	);

	CManager::Initialize();

	_IsRunProcess = true;
	
	do
	{
		if(_IsRunProcess)
			CManager::Run();
	} 
	while (!_threadEnd);
	
	_IsRunProcess = false;

	CManager::Finalize();

	D3DApp::Destroy();
}

//Start
//
//
void Editor::EditorWindow::Start()
{
	_IsRunProcess = true;
}

//Stop
//
//
void Editor::EditorWindow::Stop()
{
	_IsRunProcess = false;
}

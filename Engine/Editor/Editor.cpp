#include<Windows.h>
#include<string>
#include<thread>
#include<vector>
#include<algorithm>
#include<map>

#include"Module\DirectX\DirectX.h"

#include"../resource.h"

#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"

#include"Editor\MDIWindow.h"
#include"TreeView.h"

#include "Editor.h"
#include"InspectorView.h"

#include"NodeView.h"

#include"Common.h"
#include"Module\ECSEngine.h"
#include"Module\Module.h"

using namespace DirectX;
using namespace System;

using namespace Editor;

//*********************************************************************************************************************
//
//	SceneWindow
//
//*********************************************************************************************************************

LRESULT Editor::SceneWindow::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	char caption[256];

	switch (uMsg)
	{
		//生成
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		sprintf_s(caption, "Scene:%d x %d", rect.right - rect.left, rect.bottom - rect.top);
		SetWindowText(hWnd, caption);

		break;

	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;

		//サイズ
	case WM_SIZE:
		if (_RenderStatus)
		{
			_RenderStatus->CleanupRenderTargetView();
			_RenderStatus->CleanupDepthStencilView();

			GetClientRect(hWnd, &rect);
			sprintf_s(caption, "Scene:%d x %d", rect.right - rect.left, rect.bottom - rect.top);
			SetWindowText(hWnd, caption);

			_RenderStatus->GetSwapChain()->ResizeBuffers(0, (UINT)rect.right - rect.left, (UINT)rect.bottom - rect.top, DXGI_FORMAT_UNKNOWN, 0);
			_RenderStatus->SetClientViewport(hWnd);

			_RenderStatus->CreateRenderTargetView();
			_RenderStatus->CreateDepthStencilView();
		}
		break;

		//左ボタンが押された
	case WM_NCLBUTTONDOWN:
		//ウィンドウのサイズ変更を無視
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

HRESULT Editor::SceneWindow::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	//WndClass設定
	WNDCLASSEX WndClass = {
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

	if (!RegisterClassEx(&WndClass)) {
		MessageBox(NULL, "WndClassの設定に失敗しました。", lpCaption, MB_OK);
		return E_FAIL;
	}

	//ウィンドウ生成
	this->_hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		lpClassName,
		lpCaption,
		style,
		x,
		y,
		width + WINDOW_XBORDERFRAME,
		height + WINDOW_YBORDERFRAME,
		hParent,
		NULL,
		hInstance,
		this	//自身のポインタ設定 プロシージャに渡す
	);

	if (!this->_hWnd) {
		MessageBox(NULL, "hWndの設定に失敗しました。", lpCaption, MB_OK);
		return E_FAIL;
	}

	if (FAILED(D3DRenderer::GetInstance()->CreateRenderStatus(this->_hWnd, &_RenderStatus, 60))) {
		MessageBox(NULL, "RenderStatusの生成に失敗しました。", "GameWindow", MB_OK);
		return 0;
	}

	return S_OK;
}

//*********************************************************************************************************************
//
//	EditorWindow
//
//*********************************************************************************************************************

void Editor::EditorWindow::CheckInspectors()
{
	auto rend = std::remove_if(this->_Inspectors.begin(), this->_Inspectors.end(), [](InspectorView* obj) 
	{
		bool enable = obj->GetIsDelete();
		if(enable) delete obj;
		return enable;
	});
	this->_Inspectors.erase(rend,this->_Inspectors.end());
}

//EditorWindow
//	コンストラクタ
//
EditorWindow::EditorWindow()
{
	_FileTree = new FileTreeView();
	_SceneWindow = new SceneWindow();
}

//~EditorWindow
//	デストラクタ
//
EditorWindow::~EditorWindow()
{
	int i = 0;
	for(auto obj : _Inspectors)
	{
		delete obj;
	}
	for(auto obj : _NodeViews)
	{
		delete obj;
	}
	delete _SceneWindow;
	delete _FileTree;
}

//Create
//	エディタウィンドウ生成
//
HRESULT EditorWindow::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	//
	//	メインウィンドウ
	//
	this->_WindowFlag |= WindowFlags_CloseCheck ^ WindowFlags_DragDropFile ^ WindowFlags_PreviewProcessID;

	WNDCLASSEX WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		MAKEINTRESOURCE(IDR_MENU1),
		EDITOR_WINDOW,
		NULL
	};
	if (!RegisterClassEx(&WndClass)) {
		MessageBox(NULL, "WndClassの設定に失敗しました。", EDITOR_WINDOW, MB_OK);
		return E_FAIL;
	}

	//
	//	サブウィンドウ
	//
	WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		DefWindowProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		EDITOR_BASICWINDOW,
		NULL
	};

	if(!RegisterClassExA(&WndClass)){
		MessageBox(NULL,"WndClassの設定に失敗しました。",EDITOR_WINDOW,MB_OK);
		return E_FAIL;
	}

	//
	//	インスペクター
	//
	WndClass = {
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
		EDITOR_INSPECTOR,
		NULL
	};

	if (!RegisterClassExA(&WndClass)) {
		MessageBox(NULL, "WndClassの設定に失敗しました。", EDITOR_WINDOW, MB_OK);
		return E_FAIL;
	}

	//
	//	ウィンドウ生成
	//
	this->_hWnd = CreateWindowEx(
		NULL,
		EDITOR_WINDOW,
		"EditorWindow",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		hParent,
		NULL,
		hInstance,
		this
	);

	if(!this->_hWnd){
		MessageBox(NULL,"hWndの生成に失敗しました。",EDITOR_WINDOW,MB_OK);
	}

	

	return S_OK;
}

//Toolber
//	ref : http://wisdom.sakura.ne.jp/system/winapi/common/common8.html
//

#define TOOLBER_ID (400)

//ボタン識別子 WM_COMMANDメッセージ受け取り時に利用
#define TOOLBER_ID_01 (1) 
#define TOOLBER_ID_02 (2)

TBBUTTON tbButton[] =
{
	{ 0 , 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0, 0},
	{ STD_FILENEW, TOOLBER_ID_01,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0},
	{ VIEW_DETAILS, TOOLBER_ID_02,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0}
};

//localWndProc
//	プロシージャー処理
//
LRESULT Editor::EditorWindow::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//メッセージ
	switch (uMsg)
	{
		//生成
	case WM_CREATE:
		//ファイルのドラッグ・ドロップ許可
		if ((this->_WindowFlag & WindowFlags_DragDropFile) == WindowFlags_DragDropFile)
			DragAcceptFiles(this->_hWnd, TRUE);

		this->CaptionClipProcessID();

		//DirectX11デバイス生成
		D3DRenderer::Create();

		InitCommonControls();

		//ツールバー
		//
		//
		_hToolBer = CreateToolbarEx(
			hWnd,
			WS_VISIBLE | WS_CLIPCHILDREN | WS_CHILD,
			TOOLBER_ID,
			1,
			(HINSTANCE)HINST_COMMCTRL,
			IDB_STD_SMALL_COLOR,
			tbButton,
			ARRAYSIZE(tbButton),
			0,
			0,
			0,
			0,
			sizeof(TBBUTTON)
		);

		//カラムスペース
		//	SceneWindow や Inspectorのウィンドウはこの範囲内
		//
		_hColumnSpace = CreateWindowEx(
			NULL,
			EDITOR_BASICWINDOW,
			"SubWindow",
			WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE,
			0,
			30,
			0,
			0,
			hWnd,
			NULL,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL
		);



		//エディタ ゲームシーン
		//
		//
		this->_SceneWindow->Create(
			_hColumnSpace,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			"SceneWindow",
			"Scene",
			300,
			0,
			1024,
			576,
			WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_CAPTION
		);

		//ゲームウィンドウの生成
		//_GameWindow->Create(hWnd, "Game", CW_USEDEFAULT, CW_USEDEFAULT, 1024, 576, WS_OVERLAPPEDWINDOW);

		//ツリービュー
		//
		//
		this->_FileTree->Create(
			hWnd,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			"FileTree",
			"Project",
			0,
			0,
			500,
			300,
			WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD
		);

		//ウィンドウ最大で生成
		//
		//
		ShowWindow(hWnd, SW_MAXIMIZE);

		break;

		//フォーカスされた
	case WM_SETFOCUS:
		//最前面に表示する
		//SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		DefWindowProc(hWnd,uMsg,wParam,lParam);
		break;
		//サイズ
	case WM_SIZE:
		RECT rect;
		GetWindowRect(this->_hColumnSpace, &rect);
		MoveWindow(this->_hColumnSpace, 0, 30, LOWORD(lParam), HIWORD(lParam) - 330, TRUE);

		MoveWindow(this->_hToolBer, 0, 0, HIWORD(lParam), 100, TRUE);
		//ファイルツリー
		MoveWindow(this->_FileTree->Get_Window(), 0, HIWORD(lParam) - 300, LOWORD(lParam), 300, TRUE);
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
			/*SendMessage(this->_GameWindow->Get_Window(), uMsg, wParam, lParam);*/
			break;
		default:
			DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}
		break;

		//コマンド
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//Window -> 新規作成
		case ID_WINDOW_40003:
			CreateInspector(std::shared_ptr<GameObject>());

			break;
			//Window -> ノードウィンドウ
		case ID_WINDOW_40007:
			CreateNodeView();
			break;
			//エディタ終了
		case ID_40004:
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;

		case TOOLBER_ID_01:
			SendMessage(hWnd,WM_COMMAND,ID_WINDOW_40003,NULL);
			break;
			//ツールボタン押し 02
		case TOOLBER_ID_02:
			_EnableGUI = !_EnableGUI;
			break;
		default:
			DefWindowProc(hWnd,uMsg,wParam,lParam);
			break;
		}
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
//	TODO : エディタの設定をする
//
WPARAM EditorWindow::MessageLoop()
{

	RenderStatus* RS_SceneWindow = *this->_SceneWindow->GetLPRenderStatus();

	MSG msg;

	//GUI
	GUI::guiImGui::Create(
		this->_SceneWindow->Get_Window(),
		D3DRenderer::GetInstance()->GetDevice(),
		D3DRenderer::GetInstance()->GetDeviceContext()
	);

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	CManager::Editor(this);
	//Manager
	CManager::Initialize(this->_SceneWindow->Get_Window(), 60);

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// メッセージの翻訳とディスパッチ
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			this->CheckInspectors();

			continue;
		}

		//エディタの処理
		CManager::SetFrame();

		//エディタ更新
		Update();

		if (CManager::IsProcess())
		{
			CManager::Update();
			CManager::FixedUpdate();

			if (CManager::IsUpdate())
			{
				if (RS_SceneWindow->IsProcess())
				{
					RS_SceneWindow->ClearRenderTargetView(Color::gray());

					CManager::Render(RS_SceneWindow);

					if(_EnableGUI)
						CManager::DebugRender();

					RS_SceneWindow->End();
				}

				for(auto obj : this->_Inspectors)
				{
					auto rs = *obj->GetLPRenderStatus();
					
					if(rs->IsProcess())
					{
						rs->ClearRenderTargetView(Color::cyan());
						CManager::Render(rs);
						rs->End();
					}
				}
			}
		}

		CManager::EndFrame();

	} while (msg.message != WM_QUIT);

	CManager::Finalize();

	GUI::guiImGui::Destroy();

	D3DRenderer::Destroy();

	return msg.wParam;
}

void Editor::EditorWindow::CreateInspector(std::shared_ptr<GameObject> gameObject)
{
	RECT rect;
	GetClientRect(this->_hColumnSpace, &rect);

	//インスペクタ
	//
	//
	InspectorView* inspector;
	inspector = new InspectorView();

	inspector->Create(
		_hColumnSpace,
		(HINSTANCE)GetWindowLong(this->_hColumnSpace, GWLP_HINSTANCE),
		EDITOR_INSPECTOR,
		"Inspector",
		0,
		0,
		300 - WINDOW_XBORDERFRAME,
		(rect.bottom - rect.top) - WINDOW_YBORDERFRAME,
		WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW,
		this,
		gameObject
	);

	_Inspectors.push_back(inspector);
}

void Editor::EditorWindow::CreateNodeView()
{
	NodeView* nodeView = new NodeView();

	nodeView->Create(
		this->_hWnd,
		(HINSTANCE)GetWindowLong(this->_hColumnSpace, GWLP_HINSTANCE),
		"NodeView",
		"NodeView",
		0,
		0,
		600,
		600,
		WS_VISIBLE | WS_OVERLAPPEDWINDOW
	);

	this->_NodeViews.push_back(nodeView);
}

void Editor::EditorWindow::Update()
{

}
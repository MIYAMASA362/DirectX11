#include<Windows.h>
#include<vector>
#include<memory>

#include"Window\Window.h"
#include"Editor\Editor.h"

#include "MDIWindow.h"

#define MDI_CLIENT ("mdiClient")
#define MDI_CHILD  ("mdiChild")

#define MDI_CHILD_FIRST_ID (0x100)

using namespace Editor;

//*********************************************************************************************************************
//
//	MDIWindow
//
//*********************************************************************************************************************


//MDIWindow
//	コンストラクタ
//
MDIWindow::MDIWindow()
{

}


//~MDIWindow
//	デストラクタ
//
MDIWindow::~MDIWindow()
{

}

//Create
//	MDIClientの生成
//	MDIウィンドウの登録
//
HRESULT MDIWindow::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	//MDI親ウィンドウ
	//サブウィンドウ設定
	WNDCLASSEX WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_APPWORKSPACE + 1),
		NULL,
		lpClassName,
		NULL
	};

	//サブウィンドウ登録
	if (!RegisterClassEx(&WndClass)) {
		MessageBox(NULL, "WndClassの設定に失敗しました。", "EditorSubWindow", MB_OK);
		return E_FAIL;
	}

	//ウィンドウ生成
	this->_hWnd = CreateWindowEx(
		NULL,
		lpClassName,
		lpCaption,
		style,
		x,
		y,
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		hParent,
		NULL,
		hInstance,
		this//this	//自身のポインタ設定 プロシージャに渡す
	);

	if (!this->_hWnd) {
		MessageBox(NULL, "hWndの設定に失敗しました。", "失敗", MB_OK);
		return E_FAIL;
	}

	CLIENTCREATESTRUCT ccsClient;
	ccsClient.hWindowMenu = GetSubMenu(GetMenu(hParent),1);
	ccsClient.idFirstChild = MDI_CHILD_FIRST_ID;

	//MDIウィンドウ
	WNDCLASSEX hWndMDIChild = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		DefMDIChildProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)GetStockObject(WHITE_BRUSH),
		NULL,
		MDI_CHILD,
		NULL
	};

	//MDIウィンドウ登録
	if(!RegisterClassEx(&hWndMDIChild)){
		MessageBox(NULL,"MDIウィンドウの登録に失敗しました。","MDIWindow",MB_OK);
		return E_FAIL;
	}

	//MDI Client生成
	this->_Client = CreateWindow(
		MDI_CLIENT,
		NULL,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		x,
		y,
		width,
		height,
		this->_hWnd,
		(HMENU)1,
		hInstance,
		&ccsClient
	);

	return S_OK;
}

//CreateMDI
//	MDIClient領域にMDIウィンドウを生成する
//
HRESULT Editor::MDIWindow::CreateMDI(LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{	
	HWND hWnd;

	hWnd = CreateMDIWindow(
		MDI_CHILD,
		lpCaption,
		style,
		x,
		y,
		width,
		height,
		this->_Client,
		(HINSTANCE)GetWindowLongPtr(this->_hWnd,GWLP_HINSTANCE),
		NULL
	);

	if(!hWnd)
	{
		MessageBox(NULL,"MDIウィンドウの生成に失敗しました。","MDIWindow",MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

LRESULT Editor::MDIWindow::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefFrameProc(hWnd, this->_Client, uMsg, wParam, lParam);
}

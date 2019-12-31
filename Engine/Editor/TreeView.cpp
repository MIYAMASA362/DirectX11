#include<Windows.h>
#include<string>

#include"Window\Window.h"

#include<CommCtrl.h>
#pragma comment(lib,"comctl32.lib")

#include "TreeView.h"

using namespace System;
using namespace Editor;

//*********************************************************************************************************************
//
//	TreeView
//
//*********************************************************************************************************************

//TreeView
//	コンストラクタ
//
FileTreeView::FileTreeView()
{

}

//~TreeView
//	デストラクタ
//
FileTreeView::~FileTreeView()
{

}

//Create
//	ウィンドウ生成
//
HRESULT FileTreeView::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	this->_WndClass ={
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		lpClassName,
		NULL
	};

	RegisterClassEx(&this->_WndClass);

	this->_hInstance = hInstance;

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
		this
	);

	//表示
	ShowWindow(this->_hWnd,SW_SHOW);
	UpdateWindow(this->_hWnd);


	return S_OK;
}

//localWndProc
//
//
LRESULT Editor::FileTreeView::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
			//生成時
		case WM_CREATE:
			InitCommonControls();

			this->_TreeView = CreateWindowEx(
				NULL,
				WC_TREEVIEW,
				NULL,
				WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
				0,
				0,
				0,
				0,
				this->_hWnd,
				(HMENU)3,
				this->_hInstance,
				NULL
			);

			if (!this->_TreeView) {
				MessageBox(NULL, "ツリービューの設定に失敗しました。", "TreeView", MB_OK);
				return E_FAIL;
			}

			SendMessage(this->_TreeView, SB_SIMPLE, TRUE, 0);

			//ディレクトリ取得 現在のディレクトリ
			TCHAR currentDir[MAX_PATH + 1];
			GetCurrentDirectory(sizeof(currentDir),currentDir);

			
			TVINSERTSTRUCT tv;
			ZeroMemory(&tv, sizeof(tv));

			//追加項目
			tv.hParent = TVI_ROOT;
			tv.hInsertAfter = TVI_LAST;
			tv.item.mask = TVIF_TEXT;

			FileViewNode(currentDir,tv);
			break;

			//サイズ
		case WM_SIZE:
			MoveWindow(this->_TreeView,0,0,LOWORD(lParam),HIWORD(lParam),TRUE);
			break;

			//デフォルト
		default:
			return DefWindowProc(hWnd,uMsg,wParam,lParam);
			break;
	}
	return 0;
}

//ViewNode
//	ディレクトリ内の項目をTreeViewに表示する
//
void FileTreeView::FileViewNode(LPSTR filePath,TVINSERTSTRUCT tv)
{
	HTREEITEM group;
	HANDLE hFind;
	WIN32_FIND_DATA file;

	//項目名
	std::string name(filePath);
	size_t pos = name.find_last_of("\\");
	if(pos != std::string::npos)
		name = name.substr(pos+1);
	tv.item.pszText = (LPSTR)name.c_str();

	//項目グループ
	group = TreeView_InsertItem(this->_TreeView,&tv);
	tv.hParent = group;

	//ファイルのパス
	TCHAR path[MAX_PATH +1];
	strcpy_s(path,filePath);
	strcat_s(path,"\\*");

	//ファイルを見つける
	hFind = FindFirstFile(path,&file);

	//ファイルが見当たらない
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	//ディレクトリ内探査
	do
	{
		//ディレクトリ発見
		if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((strcmp(file.cFileName, ".") == 0) || (strcmp(file.cFileName, "..") == 0))
				continue;
			TCHAR path[MAX_PATH + 1];
			strcpy_s(path,filePath);
			strcat_s(path,"\\");
			strcat_s(path,file.cFileName);

			FileViewNode(path,tv);
		}
		//ファイル名表示
		else
		{
			tv.item.pszText = file.cFileName;
			TreeView_InsertItem(this->_TreeView,&tv);
		}
	}while (FindNextFile(hFind, &file));

	//破棄
	FindClose(hFind);
}

#include<Windows.h>
#include<string>
#include<memory>
#include<map>

#include"Window\Window.h"

#include"Module\DirectX\DirectX.h"
#include"Module\IMGUI\GUI_ImGui.h"

#include"Module\Texture\texture.h"
#include"Module\Texture\TextureManager.h"

#include "TreeView.h"
#pragma comment(lib,"comctl32.lib")

using namespace System;
using namespace Editor;

//CreateSolidBrush(RGB(0xff, 0xff, 0xff))

LPTSTR strItem[] = { TEXT("RENA") , TEXT("YUKI") , TEXT("MIMI") };

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

	RegisterClassEx(&WndClass);

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
	{
		//ディレクトリ取得 現在のディレクトリ
		TCHAR currentDir[MAX_PATH + 1];
		GetCurrentDirectory(sizeof(currentDir), currentDir);
		_TreeDirectory = currentDir;

		//TreeViewの作成
		{
			InitCommonControls();

			//Iconのイメージ作成
			this->_hImage = ImageList_Create(16, 16, ILC_COLOR | ILC_MASK, 2, 1);
			ImageList_AddIcon(this->_hImage, LoadIcon(NULL, IDI_QUESTION));
			ImageList_AddIcon(this->_hImage, LoadIcon(NULL, IDI_ASTERISK));

			this->_TreeView = CreateWindowEx(
				NULL,
				WC_TREEVIEW,
				NULL,
				WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_INFOTIP,
				0,
				0,
				300,
				HIWORD(lParam),
				hWnd,
				NULL,
				(HINSTANCE)GetWindowLongPtr(hWnd,GWLP_HINSTANCE),
				NULL
			);

			if (!this->_TreeView) {
				MessageBox(NULL, "ツリービューの設定に失敗しました。", "FileTreeView", MB_OK);
				return E_FAIL;
			}
			////ImageをTreeViewに適応する
			//TreeView_SetImageList(this->_TreeView,this->_hImage,TVSIL_NORMAL);

			SendMessage(this->_TreeView, SB_SIMPLE, TRUE, 0);

			//FileTree作成
			{

				TVINSERTSTRUCT tv;
				ZeroMemory(&tv, sizeof(tv));

				//追加項目
				tv.hParent = TVI_ROOT;
				tv.hInsertAfter = TVI_LAST;
				tv.item.mask = TVIF_TEXT;

				tv.item.iImage = 0;
				tv.item.iSelectedImage = 1;

				FileViewNode(currentDir, tv);

				TreeView_Expand(this->_TreeView, TreeView_GetFirstVisible(this->_TreeView), TVE_EXPAND);
			}
		}

		//リストビュー
		{
			this->_ListView = CreateWindowEx(
				NULL,
				WC_LISTVIEW,
				NULL,
				WS_CHILD | WS_VISIBLE | LVS_REPORT,
				300,
				0,
				LOWORD(lParam) - 300,
				HIWORD(lParam),
				hWnd,
				NULL,
				(HINSTANCE)GetWindowLongPtr(hWnd,GWLP_HINSTANCE),
				NULL
			);

			//LVCLOUMN
			_Column.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;

			_Column.fmt = LVCFMT_LEFT;
			_Column.pszText = "FileView";
			_Column.cx = 200;	//列の幅

			ListView_InsertColumn(this->_ListView, 0, &this->_Column);
			//現在のディレクトリファイル表示
			ListView_FileView(currentDir);
		}
	}
	break;

	//サイズ変更
	case WM_SIZE:
	{
		MoveWindow(this->_TreeView, 0, 0, 300, HIWORD(lParam), TRUE);
		MoveWindow(this->_ListView, 300, 0, LOWORD(lParam) - 300, HIWORD(lParam), TRUE);
	}
	break;

	//キー入力
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			//F1キー
		case VK_F1:
			ShowWindow(hWnd, SW_SHOW);
			ShowWindow(hWnd, SW_RESTORE);
			break;
		default:
			break;
		}
	}
	break;

	//コモンコントロールからのメッセージ
	case WM_NOTIFY:
	{
		switch (((LPNMHDR)lParam)->code)
		{
#pragma region TREEVIEW_NOTIFY

			//ドラッグ開始
		case TVN_BEGINDRAG:
		{
			//TreeView_OnBeginDrag(this->_TreeView,(LPNMTREEVIEW)lParam);
		}
		break;

		//ツリービューの選択が変更された
		case TVN_SELCHANGED:
		{
			LPNMTREEVIEW lptv;
			lptv = (LPNMTREEVIEW)lParam;
			HTREEITEM item;
			item = lptv->itemNew.hItem;
			_TreeDirectory = TreeView_GetFilePath(item);
			ListView_FileView(_TreeDirectory);

		}
		break;

		//TreeView情報の取得
		//
		//
		case TVN_GETINFOTIP:
		{
			////Itemのテキスト取得
			//TVITEM tvitem;
			//tvitem.mask = TVIF_TEXT;
			//tvitem.hItem = item;
			//
			//tvitem.pszText = infoTipBuf;
			//tvitem.cchTextMax = sizeof(temp) / sizeof(TCHAR);
			//TreeView_GetItem(this->_TreeView, &tvitem);

			//// Copy the text to the infotip.
			////strcpy_s(pTip->pszText,pTip->cchTextMax,tvitem.pszText);
			//tvitem.pszText = tvitem.pszText;
		}
		break;

#pragma endregion

#pragma region LISTVIEW_NOTIFY

		//ListViewがクリックされた
		//	ref : https://docs.microsoft.com/en-us/windows/win32/controls/lvn-columnclick
		//
		case NM_CLICK:
		{
			if (((LPNMHDR)lParam)->hwndFrom == this->_ListView)
			{
				LPNMITEMACTIVATE lpnmitem;
				lpnmitem = (LPNMITEMACTIVATE)lParam;
				ListView_FileOpen(lpnmitem);
			}
		}
		break;

		case LVN_COLUMNCLICK:
		{

		}
		break;

#pragma endregion


		default:
			DefWindowProc(this->_TreeView, uMsg, wParam, lParam);
			break;

		}
	}
	break;	//WM_NOTIFY

	case WM_MOUSEMOVE:
		//TreeView_OnDragging(hWnd, this->_TreeView, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_MBUTTONUP:
		//TreeView_OnEndDrag(this->_TreeView);
		break;

		//デフォルト
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

//ViewNode
//	ディレクトリ内の項目をTreeViewに表示する
//
void FileTreeView::FileViewNode(LPSTR filePath, TVINSERTSTRUCT tv)
{
	HTREEITEM group;
	HANDLE hFind;
	WIN32_FIND_DATA file;

	//項目名
	std::string name(filePath);
	size_t pos = name.find_last_of("\\");
	if (pos != std::string::npos)
		name = name.substr(pos + 1);
	tv.item.pszText = (LPSTR)name.c_str();

	//項目グループ
	group = TreeView_InsertItem(this->_TreeView, &tv);
	tv.hParent = group;

	//ファイルのパス
	TCHAR path[MAX_PATH + 1];
	strcpy_s(path, filePath);
	strcat_s(path, "\\*");

	//ファイルを見つける
	hFind = FindFirstFile(path, &file);

	//ファイルが見当たらない
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	//ディレクトリ内探査
	do
	{
		//ディレクトリ発見
		if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((strcmp(file.cFileName, ".") == 0) || (strcmp(file.cFileName, "..") == 0))
				continue;
			TCHAR path[MAX_PATH + 1];
			strcpy_s(path, filePath);
			strcat_s(path, "\\");
			strcat_s(path, file.cFileName);

			FileViewNode(path, tv);
		}
	} while (FindNextFile(hFind, &file));

	//破棄
	FindClose(hFind);
}

//TreeView_OnBeginDrag
//	ツリーがドラッグ移動された
//	ref : https://docs.microsoft.com/ja-jp/windows/win32/controls/drag-a-tree-view-item
//
void Editor::FileTreeView::TreeView_OnBeginDrag(HWND hwndTV, LPNMTREEVIEW lpnmtv)
{
	HIMAGELIST himl;
	RECT rcItem;

	himl = TreeView_CreateDragImage(hwndTV, lpnmtv->itemNew.hItem);

	TreeView_GetItemRect(hwndTV, lpnmtv->itemNew.hItem, &rcItem, TRUE);

	ImageList_BeginDrag(himl, 0, 0, 0);
	ImageList_DragEnter(hwndTV, lpnmtv->ptDrag.x, lpnmtv->ptDrag.x);

	SetCapture(GetParent(hwndTV));

	_IsDragging = TRUE;

	return;
}

//TreeView_OnDragging
//	ツリーがドラッグ移動中
//
void Editor::FileTreeView::TreeView_OnDragging(HWND hwndParent, HWND hwndTV, LONG xCur, LONG yCur)
{
	//ドラッグしてる
	if (_IsDragging)
	{

		HTREEITEM TargetItem;	//選択Item
		TVHITTESTINFO tvInfo;

		POINT point; //Item位置
		point.x = xCur;
		point.y = yCur;

		//位置変換
		ClientToScreen(hwndParent, &point);
		ScreenToClient(hwndTV, &point);

		//ドラック中の見た目
		ImageList_DragMove(point.x, point.y);

		//バックグランドで削除する様に
		ImageList_DragShowNolock(FALSE);

		tvInfo.pt.x = point.x;
		tvInfo.pt.y = point.y;

		//Itemがドラッグ可能領域にある
		if ((TargetItem = TreeView_HitTest(hwndTV, &tvInfo)) != NULL)
		{
			TreeView_SelectDropTarget(hwndTV, TargetItem);
		}

		ImageList_DragShowNolock(TRUE);
	}
	return;
}

//TreeView_OnEndDrag
//	ツリーのドラッグ移動終了
//
void Editor::FileTreeView::TreeView_OnEndDrag(HWND hwndTV)
{
	if (_IsDragging)
	{
		HTREEITEM hitDest;
		hitDest = TreeView_GetDropHilight(hwndTV);
		if (hitDest != NULL)
		{
			MessageBox(NULL, "Hello", "Hello", MB_OK);
		}

		ImageList_EndDrag();
		TreeView_SelectDropTarget(hwndTV, NULL);
		ReleaseCapture();
		_IsDragging = FALSE;
	}
	return;
}

//TreeView_GetFilePath
//	Itemのファイルパスを取得する　TreeView　Itemの親を参照して取得
//	WARNING : 最上階層はCurrentDirectory
//
std::string Editor::FileTreeView::TreeView_GetFilePath(HTREEITEM item)
{
	std::string path;

	//ノードの親取得
	HTREEITEM parent;
	parent = TreeView_GetParent(this->_TreeView, item);

	//最上階層
	if (parent == NULL)
	{
		TCHAR currentDir[MAX_PATH + 1];
		GetCurrentDirectory(sizeof(currentDir), currentDir);
		return currentDir;
	}

	//テキストの取得（item名）
	TCHAR name[MAX_PATH + 1];

	TVITEM tv;
	tv.mask = TVIF_TEXT;
	tv.hItem = item;
	tv.pszText = name;
	tv.cchTextMax = sizeof(name) / sizeof(TCHAR);

	TreeView_GetItem(this->_TreeView, &tv);

	path = TreeView_GetFilePath(parent) + "\\" + tv.pszText;
	return path;
}

//ListView_FileView
//	DirectoryPathの内容をListViewで表示する
//
void Editor::FileTreeView::ListView_FileView(std::string DirectoryPath)
{
	ListView_DeleteAllItems(this->_ListView);

	HANDLE hFind;
	WIN32_FIND_DATA file;

	TCHAR path[MAX_PATH + 1];
	strcpy_s(path, DirectoryPath.c_str());
	strcat_s(path, "\\*");

	//ファイルを見つける
	hFind = FindFirstFile(path, &file);

	//ファイルが見当たらない
	if (hFind == INVALID_HANDLE_VALUE) return;

	int num = 0;

	LVITEM item;
	ZeroMemory(&item, sizeof(item));
	item.mask = TVIF_TEXT;

	do
	{
		if ((strcmp(file.cFileName, ".") == 0) || (strcmp(file.cFileName, "..") == 0))
			continue;
		item.iItem = num;
		item.pszText = file.cFileName;
		ListView_InsertItem(this->_ListView, &item);
		num++;
	} while (FindNextFile(hFind, &file));

	FindClose(hFind);
}


//ListView_FileOpen
//	ListViewで選択された項目を取得する
//	現状ファイルをオープンする形になっている
//
//	ファイル名 : item.pszText (拡張子込み)
//
void Editor::FileTreeView::ListView_FileOpen(LPNMITEMACTIVATE lpnmitem)
{
	TCHAR name[MAX_PATH + 1];

	LVITEM item;
	ZeroMemory(&item, sizeof(item));

	item.mask = TVIF_HANDLE | TVIF_TEXT;
	item.iItem = lpnmitem->iItem;
	item.iSubItem = 0;
	item.pszText = name;
	item.cchTextMax = sizeof(name) / sizeof(TCHAR);

	ListView_GetItem(this->_ListView, &item);

	std::string path;
	path = _TreeDirectory + "\\" + item.pszText;
	path = path;

	//ファイルの探査
	HANDLE hFind;
	WIN32_FIND_DATA file;

	hFind = FindFirstFile(path.c_str(), &file);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	//ディクショナリの場合ツリーの項目を開く
	if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		HTREEITEM htItem;
		TCHAR text[MAX_PATH + 1];

		htItem = TreeView_GetSelection(this->_TreeView);					//現在選択中の項目

		//ツリービュー　選択アイテム情報
		TVITEM tv;
		ZeroMemory(&tv, sizeof(tv));
		tv.mask = TVIF_TEXT | TVIF_CHILDREN;
		tv.pszText = text;
		tv.cchTextMax = sizeof(text) / sizeof(TCHAR);
		tv.hItem = htItem;

		//項目情報取得
		TreeView_GetItem(this->_TreeView, &tv);

		//子の取得
		htItem = TreeView_GetChild(this->_TreeView, tv.hItem);

		if (strcmp(tv.pszText, item.pszText) != 0)
		{
			//ノード探索
			do
			{
				tv.hItem = htItem;
				TreeView_GetItem(this->_TreeView, &tv);
				//発見
				if (strcmp(tv.pszText, item.pszText) == 0)break;

				//兄弟
				htItem = TreeView_GetNextSibling(this->_TreeView, tv.hItem);
				if (htItem == NULL) break;

			} while (TRUE);
		}
		TreeView_Expand(this->_TreeView, tv.hItem, TVE_EXPAND);				//項目の子ノードを展開表示
		TreeView_Select(this->_TreeView,tv.hItem,TVGN_CARET);

		FindClose(hFind);
		return;
	}

	// TODO : リストビュー操作/選択時の機能を作成する

	std::string extension;
	size_t pos = path.find_last_of(".");
	extension = path.substr(pos + 1);

	if(extension == "tga")
	{
		size_t fpos = path.find_last_of("\\");
		std::string name = path.substr(fpos + 1,pos - (fpos +1));
		auto texture = TextureManager::GetTexture(name);
		TextureManager::SetEditorView(texture.lock()->GetShaderResourceView());
	}
	else
	{
		//ファイルオープン
		ShellExecute(this->_hWnd, NULL, path.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
	FindClose(hFind);
}

//ListView_GetFilePath
//
//
std::string Editor::FileTreeView::ListView_GetFilePath(HTREEITEM item)
{



	return std::string();
}

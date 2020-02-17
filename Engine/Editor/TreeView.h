#pragma once

#include<CommCtrl.h>

namespace System
{
	class Window;
}

namespace Editor
{
	//*********************************************************************************************************************
	//
	//	FileTreeView
	//		ファイルのツリービュー
	//
	//*********************************************************************************************************************
	class FileTreeView : public System::Window
	{
	private:
		std::string _TreeDirectory;	//ツリービューでのディレクトリ

		HWND _TreeView;		//ツリービュー
		BOOL _IsDragging;	//ツリーが移動中
		HIMAGELIST _hImage;	//イメージリスト

		HWND _ListView;		//リストビュー
		LVCOLUMN _Column;	//リストビュー表示形式

	public:
		//コンストラクタ
		FileTreeView();
		//デストラクタ
		virtual ~FileTreeView();

		//ウィンドウ生成
		virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		//ローカルなウィンドウプロシージャ
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




		//*********************************************************************************************************************
		//
		//	TreeView
		//
		//*********************************************************************************************************************

		//ファイルノード表示
		void FileViewNode(LPSTR filePath, TVINSERTSTRUCT tv);

		//ツリーがドラッグ移動開始
		void TreeView_OnBeginDrag(HWND hwndTV, LPNMTREEVIEW lpnmtv);
		//ツリーがドラッグ移動中
		void TreeView_OnDragging(HWND hwndParent, HWND hwndTV, LONG xCur, LONG yCur);
		//ツリーのドラッグ移動終了
		void TreeView_OnEndDrag(HWND hwndTV);

		//選択中のツリーノードを遡って、選択されたファイルのパスを得る
		std::string TreeView_GetFilePath(HTREEITEM item);




		//*********************************************************************************************************************
		//
		//	ListView
		//
		//*********************************************************************************************************************

		//ディレクトリ内のファイルをリストビューで表示
		void ListView_FileView(std::string DirectoryPath);
		//選択ファイルを開く
		void ListView_FileOpen(LPNMITEMACTIVATE lpnmitem);

		//リストビューからファイルパスを得る
		std::string ListView_GetFilePath(HTREEITEM item);
	};
}
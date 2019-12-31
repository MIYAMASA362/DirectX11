#pragma once

namespace System
{
	class Window;
}

namespace Editor
{
	//FileTreeView
	//	ファイルのツリービュー
	//
	class FileTreeView : public System::Window
	{
	private:
		HWND _TreeView;

	public:
		//コンストラクタ
		FileTreeView();
		//デストラクタ
		virtual ~FileTreeView();

		//ウィンドウ生成
		virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		//ローカルなウィンドウプロシージャ
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//ファイルノード表示
		void FileViewNode(LPSTR filePath, TVINSERTSTRUCT tv);
	};
}
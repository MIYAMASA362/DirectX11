#pragma once

namespace System
{
	class Window;
}

namespace Editor
{
	//MDIWindow
	//	MDI
	//
	class MDIWindow:public System::Window
	{
	private:
		HWND _Client;
	public:
		//コンストラクタ
		MDIWindow();
		//デストラクタ
		virtual ~MDIWindow();

		//ウィンドウ生成 MDIクライアント領域の設定
		virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		//MDIウィンドウ生成
		virtual HRESULT CreateMDI(LPSTR lpCaption,int x,int y,long width,long height,DWORD style);

		//ローカルなウィンドウプロシージャ
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}
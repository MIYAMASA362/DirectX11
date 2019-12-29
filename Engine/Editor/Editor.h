#pragma once

namespace System
{
	class Window;
}

namespace Editor
{
	class EditorWindow;

	//Editor用のサブウィンドウ
	//	
	//
	class EditorSubWindow : public System::Window
	{
	private:
		HWND* _MDIClient = nullptr;	//MDIウィンドウ クライアント
		MDICREATESTRUCT _MDIC;

	public:
		//コンストラクタ
		EditorSubWindow();
		//デストラクタ
		virtual ~EditorSubWindow();

		//ウィンドウ生成
		virtual HRESULT Create(EditorWindow* parent,LPSTR lpClassName,LPSTR lpCaption,int x,int y, long width,long height,DWORD style);
		//ウィンドウプロシージャ
		virtual LRESULT localWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) override;
	};


	//EditorWindow
	//	エディタ用のウィンドウ
	//
	class EditorWindow final : public System::Window
	{
	private:
		//Window
		System::Window* _Window;

		EditorSubWindow* _SubWindow;
	public:
		//コンストラクタ
		EditorWindow();
		//デストラクタ
		~EditorWindow();

		//ウィンドウ生成
		HRESULT Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style) override;

		//メッセージループ
		WPARAM MessageLoop() override;
	};
}
#pragma once

namespace System
{
	class Window;
}

namespace Editor
{
	class EditorWindow;
	class MDIWindow;
	class CommonControlWindow;
	class FileTreeView;

	//Editor用のサブウィンドウ
	//	メニュー操作　可能ウィンドウ
	//
	class EditorSubWindow final : public System::Window
	{
	private:
		EditorWindow* _MainEditor = nullptr;

		MDIWindow* _MDIWindow = nullptr;
		FileTreeView* _FileTree = nullptr;

	public:
		//コンストラクタ
		EditorSubWindow(EditorWindow* editor);
		//デストラクタ
		~EditorSubWindow();

		//ウィンドウ生成
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName,LPSTR lpCaption, int x,int y, long width,long height,DWORD style) override;
		//ウィンドウプロシージャ
		LRESULT localWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) override;
	};


	//EditorWindow
	//	エディタ用のウィンドウ　メインウィンドウ
	//	
	//	WARNING : スレッドを操作しているので、リソースの読み込みなどに影響を与える可能性
	//
	class EditorWindow final : public System::Window
	{
	private:
		//Window
		System::Window* _Window;

		//エディタサブウィンドウ
		EditorSubWindow* _SubWindow;
		
		//スレッドの終了
		bool _threadEnd = false;
		//実行状態か
		bool _IsRunProcess= false;

	public:
		//コンストラクタ
		EditorWindow();
		//デストラクタ
		~EditorWindow();

		//ウィンドウ生成
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption,int x,int y, long width, long height, DWORD style) override;

		//ウィンドウプロシージャ
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		//メッセージループ
		WPARAM MessageLoop() override;


		//ゲーム用のスレッド
		void RunProcces();

		//ゲームの更新を停止
		void Start();

		//ゲームの更新を開始
		void Stop();

	};
}
#pragma once

namespace System
{
	//ウィンドウのフラグ
	typedef int WindowFlags;

	//ウィンドウフラグ
	enum WindowFlags_
	{
		WindowFlags_None = 0,
		WindowFlags_CloseCheck = 1 << 0,				//Windowを閉じた時にメッセージウィンドウを表示する
		WindowFlags_DragDropFile = 1 << 1,				//Windowへファイルのドラッグ&ドロップを許可する
		WindowFlags_MultipleDocumentInterface = 1 << 2	//WindowのMDIを有効化
	};

	//Window
	//	メインウィンドウ生成用のウィンドウクラス
	//
	class Window
	{
	protected:
		//ウィンドウハンドル
		HWND _hWnd;
		//ウィンドウクラス
		WNDCLASSEX _WndClass;
		//インスタンスハンドル
		HINSTANCE _hInstance;

		//ウィンドウフラグ
		WindowFlags _WindowFlag = WindowFlags_None;

	public:
		//コンストラクタ
		Window();
		//デストラクタ
		~Window();

		//生成
		virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption,int x,int y, long width, long height, DWORD style);
		//破棄
		HRESULT Destroy();

		//HWND取得
		HWND Get_Window() { return _hWnd; }
		//インスタンス
		HINSTANCE Get_hInstance() { return _hInstance; }

		//ローカルなウィンドウプロシージャ
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//メッセージループ イベントドリブン
		virtual WPARAM MessageLoop();

		//ウィンドウフラグの取得
		WindowFlags& GetWindowFlags() { return this->_WindowFlag; };

		//ウィンドウハンドル設定
		void SetHWndPointer(HWND hWnd);

		//ウィンドウプロシージャ
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

}
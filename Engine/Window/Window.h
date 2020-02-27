#pragma once

namespace System
{
	//*********************************************************************************************************************
	//
	//	IWindow
	//
	//*********************************************************************************************************************
	class IWindow
	{
	protected:
		HWND _hWnd;

	public:
		IWindow();
		virtual ~IWindow();

		//ウィンドウプロシージャ
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//ローカルなウィンドウプロシージャ
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//ウィンドウハンドル設定
		void SetHWndPointer(HWND hWnd);

		//HWND取得
		HWND Get_Window() { return _hWnd; }
	};



	//ウィンドウのフラグ
	typedef int WindowFlags;

	//ウィンドウフラグ
	enum WindowFlags_
	{
		WindowFlags_None = 0,
		WindowFlags_CloseCheck = 1 << 0,				//Windowを閉じた時にメッセージウィンドウを表示する
		WindowFlags_DragDropFile = 1 << 1,				//Windowへファイルのドラッグ&ドロップを許可する
		WindowFlags_MultipleDocumentInterface = 1 << 2,	//WindowのMDIを有効化
		WindowFlags_PreviewProcessID = 1 << 3			//ProcessIDの表示
	};

	//*********************************************************************************************************************
	//
	//	Window
	//		メインウィンドウ生成用のウィンドウクラス
	//
	//*********************************************************************************************************************
	class Window : public IWindow
	{
	protected:
		//ウィンドウフラグ
		WindowFlags _WindowFlag = WindowFlags_None;

	public:
		//コンストラクタ
		Window();
		//デストラクタ
		virtual ~Window();

		//生成
		virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption,int x,int y, long width, long height, DWORD style);

		//ローカルなウィンドウプロシージャ
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		//メッセージループ イベントドリブン
		virtual WPARAM MessageLoop();

		//ウィンドウフラグの取得
		WindowFlags& GetWindowFlags() { return this->_WindowFlag; };

		void CaptionClipProcessID();


	};

}
#pragma once

class RenderStatus;

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

	//*********************************************************************************************************************
	//
	//	InspectorView
	//
	//*********************************************************************************************************************
	class InspectorView final : public System::Window
	{
	private:
		EditorWindow* _EditorWindow = nullptr;

		RenderStatus* _RenderStatus = nullptr;

		HWND _ModelView;

		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;
	
		bool _IsDelete = false;
	

	public:
		~InspectorView();

		//ウィンドウプロシージャ
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
		//ウィンドウ生成
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style,EditorWindow* editor);

		RenderStatus** GetLPRenderStatus() { return &_RenderStatus; };

		bool GetIsDelete() { return _IsDelete; }


	};

	//*********************************************************************************************************************
	//
	//	SceneWindow
	//
	//*********************************************************************************************************************
	class SceneWindow final : public System::Window
	{
	private:
		RenderStatus* _RenderStatus = nullptr;

	public:
		//ウィンドウプロシージャ
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
		//ウィンドウ生成
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		RenderStatus** GetLPRenderStatus() { return &_RenderStatus; };


	};


	//*********************************************************************************************************************
	//
	//	EditorWindow
	//	　エディタ用のウィンドウ メインウィンドウ
	//
	//	TODO : SubWindow 廃止 
	//
	//	WARNING : スレッドを操作しているので、リソースの読み込みなどに影響を与える可能性
	//
	//*********************************************************************************************************************
	class EditorWindow final : public System::Window
	{
	private:
		//ファイルビュー
		FileTreeView* _FileTree = nullptr;

		//シーンウィンドウ
		SceneWindow* _SceneWindow = nullptr;

		std::vector<InspectorView*> _Inspectors;

		HWND _hToolBer;		//ツールバー
		HWND _hColumnSpace; //エディタ領域

		bool _EnableGUI = true;


		void CheckInspectors();

	public:
		//コンストラクタ
		EditorWindow();
		//デストラクタ
		~EditorWindow();

		//ウィンドウ生成
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		//ウィンドウプロシージャ
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		//メッセージループ
		WPARAM MessageLoop() override;

		void Update();
	};
}
#pragma once

#ifndef EDITOR_HEADER
#define EDITOR_HEADER

#define WINDOW_XBORDERFRAME ((GetSystemMetrics(SM_CXBORDER) + GetSystemMetrics(SM_CXFIXEDFRAME) + GetSystemMetrics(SM_CXFRAME)) * 2)
#define WINDOW_YBORDERFRAME ((GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYFRAME)) * 2 + GetSystemMetrics(SM_CYCAPTION))

//ユーザー定義メッセージ
#define WM_INSPECTOR_DELETE (WM_APP + 1)

#define EDITOR_WINDOW		("EditorWindow")
#define EDITOR_BASICWINDOW	("EditorBasicWindow")
#define EDITOR_INSPECTOR	("EditorInspector")
#define EDITOR_INSPECTOR_VIEW ("EditorInspectorView")
#define EDITOR_GUIOVERRAP ("EditorGuiOverRap")

class RenderStatus;
class GameObject;

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
	class InspectorView;
	class NodeView;

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
	//*********************************************************************************************************************
	class EditorWindow final : public System::Window
	{
	private:
		//ファイルビュー
		FileTreeView* _FileTree = nullptr;

		//シーンウィンドウ
		SceneWindow* _SceneWindow = nullptr;

		std::vector<InspectorView*> _Inspectors;

		std::vector<NodeView*> _NodeViews;

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

		void CreateInspector(std::shared_ptr<GameObject> gameObject);

		void CreateNodeView();

		void Update();
	};
}

#endif // !EDITOR_HEADER
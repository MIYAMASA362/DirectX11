#pragma once

namespace System
{
	class Window;
}

namespace Editor
{
	class NodeColumn;

	//接続位置
	class ConectPos
	{
	private:
		float _x;
		float _y;

		ConectPos* _Pair;		//接続先
		NodeColumn* _Node;		//所属ノード
	public:
		//コンストラクタ
		ConectPos(float x, float y, NodeColumn* node)
			:
			_x(x),
			_y(y),
			_Node(node)
		{}
		//接続
		void Conect(ConectPos* conect);
		//描画
		void Draw(HDC hdc);
	};

	//NodeColum
	//
	//
	class NodeColumn : public System::Window
	{
	private:
		//接続可能位置
		std::vector<ConectPos*> _ConectPos;

		bool _IsSelect;
		COLORREF _DefaultBkColor;

		float _offsetX;
		float _offsetY;
	public:
		NodeColumn();
		virtual ~NodeColumn();

		//接続先の追加
		void AddConectPos(float x, float y);

		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	};

	//NodeView
	//
	//
	class NodeView final : public System::Window
	{
	private:
		std::vector<NodeColumn*> _NodeColumn;
	public:
		NodeView();
		~NodeView();

		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		void AddNodeColumn();
	};
}

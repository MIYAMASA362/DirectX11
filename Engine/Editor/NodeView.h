#pragma once

namespace System
{
	class Window;
}

namespace Editor
{
	class NodeColumn;

	//�ڑ��ʒu
	class ConectPos
	{
	private:
		float _x;
		float _y;

		ConectPos* _Pair;		//�ڑ���
		NodeColumn* _Node;		//�����m�[�h
	public:
		//�R���X�g���N�^
		ConectPos(float x, float y, NodeColumn* node)
			:
			_x(x),
			_y(y),
			_Node(node)
		{}
		//�ڑ�
		void Conect(ConectPos* conect);
		//�`��
		void Draw(HDC hdc);
	};

	//NodeColum
	//
	//
	class NodeColumn : public System::Window
	{
	private:
		//�ڑ��\�ʒu
		std::vector<ConectPos*> _ConectPos;

		bool _IsSelect;
		COLORREF _DefaultBkColor;

		float _offsetX;
		float _offsetY;
	public:
		NodeColumn();
		virtual ~NodeColumn();

		//�ڑ���̒ǉ�
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

#pragma once

namespace System
{
	class Window;
}

namespace Editor
{
	class EditorWindow;

	//Editor�p�̃T�u�E�B���h�E
	//	
	//
	class EditorSubWindow : public System::Window
	{
	private:
		HWND* _MDIClient = nullptr;	//MDI�E�B���h�E �N���C�A���g
		MDICREATESTRUCT _MDIC;

	public:
		//�R���X�g���N�^
		EditorSubWindow();
		//�f�X�g���N�^
		virtual ~EditorSubWindow();

		//�E�B���h�E����
		virtual HRESULT Create(EditorWindow* parent,LPSTR lpClassName,LPSTR lpCaption,int x,int y, long width,long height,DWORD style);
		//�E�B���h�E�v���V�[�W��
		virtual LRESULT localWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) override;
	};


	//EditorWindow
	//	�G�f�B�^�p�̃E�B���h�E
	//
	class EditorWindow final : public System::Window
	{
	private:
		//Window
		System::Window* _Window;

		EditorSubWindow* _SubWindow;
	public:
		//�R���X�g���N�^
		EditorWindow();
		//�f�X�g���N�^
		~EditorWindow();

		//�E�B���h�E����
		HRESULT Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style) override;

		//���b�Z�[�W���[�v
		WPARAM MessageLoop() override;
	};
}
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
		//�R���X�g���N�^
		MDIWindow();
		//�f�X�g���N�^
		virtual ~MDIWindow();

		//�E�B���h�E���� MDI�N���C�A���g�̈�̐ݒ�
		virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		//MDI�E�B���h�E����
		virtual HRESULT CreateMDI(LPSTR lpCaption,int x,int y,long width,long height,DWORD style);

		//���[�J���ȃE�B���h�E�v���V�[�W��
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
}
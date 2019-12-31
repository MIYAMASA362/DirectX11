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

	//Editor�p�̃T�u�E�B���h�E
	//	���j���[����@�\�E�B���h�E
	//
	class EditorSubWindow final : public System::Window
	{
	private:
		EditorWindow* _MainEditor = nullptr;

		MDIWindow* _MDIWindow = nullptr;
		FileTreeView* _FileTree = nullptr;

	public:
		//�R���X�g���N�^
		EditorSubWindow(EditorWindow* editor);
		//�f�X�g���N�^
		~EditorSubWindow();

		//�E�B���h�E����
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName,LPSTR lpCaption, int x,int y, long width,long height,DWORD style) override;
		//�E�B���h�E�v���V�[�W��
		LRESULT localWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam) override;
	};


	//EditorWindow
	//	�G�f�B�^�p�̃E�B���h�E�@���C���E�B���h�E
	//	
	//	WARNING : �X���b�h�𑀍삵�Ă���̂ŁA���\�[�X�̓ǂݍ��݂Ȃǂɉe����^����\��
	//
	class EditorWindow final : public System::Window
	{
	private:
		//Window
		System::Window* _Window;

		//�G�f�B�^�T�u�E�B���h�E
		EditorSubWindow* _SubWindow;
		
		//�X���b�h�̏I��
		bool _threadEnd = false;
		//���s��Ԃ�
		bool _IsRunProcess= false;

	public:
		//�R���X�g���N�^
		EditorWindow();
		//�f�X�g���N�^
		~EditorWindow();

		//�E�B���h�E����
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption,int x,int y, long width, long height, DWORD style) override;

		//�E�B���h�E�v���V�[�W��
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		//���b�Z�[�W���[�v
		WPARAM MessageLoop() override;


		//�Q�[���p�̃X���b�h
		void RunProcces();

		//�Q�[���̍X�V���~
		void Start();

		//�Q�[���̍X�V���J�n
		void Stop();

	};
}
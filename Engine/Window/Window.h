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

		//�E�B���h�E�v���V�[�W��
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//���[�J���ȃE�B���h�E�v���V�[�W��
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//�E�B���h�E�n���h���ݒ�
		void SetHWndPointer(HWND hWnd);

		//HWND�擾
		HWND Get_Window() { return _hWnd; }
	};



	//�E�B���h�E�̃t���O
	typedef int WindowFlags;

	//�E�B���h�E�t���O
	enum WindowFlags_
	{
		WindowFlags_None = 0,
		WindowFlags_CloseCheck = 1 << 0,				//Window��������Ƀ��b�Z�[�W�E�B���h�E��\������
		WindowFlags_DragDropFile = 1 << 1,				//Window�փt�@�C���̃h���b�O&�h���b�v��������
		WindowFlags_MultipleDocumentInterface = 1 << 2,	//Window��MDI��L����
		WindowFlags_PreviewProcessID = 1 << 3			//ProcessID�̕\��
	};

	//*********************************************************************************************************************
	//
	//	Window
	//		���C���E�B���h�E�����p�̃E�B���h�E�N���X
	//
	//*********************************************************************************************************************
	class Window : public IWindow
	{
	protected:
		//�E�B���h�E�t���O
		WindowFlags _WindowFlag = WindowFlags_None;

	public:
		//�R���X�g���N�^
		Window();
		//�f�X�g���N�^
		virtual ~Window();

		//����
		virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption,int x,int y, long width, long height, DWORD style);

		//���[�J���ȃE�B���h�E�v���V�[�W��
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		//���b�Z�[�W���[�v �C�x���g�h���u��
		virtual WPARAM MessageLoop();

		//�E�B���h�E�t���O�̎擾
		WindowFlags& GetWindowFlags() { return this->_WindowFlag; };

		void CaptionClipProcessID();


	};

}
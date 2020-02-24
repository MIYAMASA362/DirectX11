#include<Windows.h>

#include"Module\IMGUI\GUI_ImGui.h"
#include"Window.h"

using namespace System;


//*********************************************************************************************************************
//
//	IWindow
//
//*********************************************************************************************************************

System::IWindow::IWindow()
{

}

System::IWindow::~IWindow()
{
	DestroyWindow(_hWnd);
}


#ifdef GUI_ImGui_H
//ImGui�̃E�B���h�E�ړ�
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

LRESULT System::IWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef GUI_ImGui_H
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
#endif

	IWindow* window = (IWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (uMsg == WM_CREATE)
	{
		//���[�U�[���ݒ肵��Param�𗘗p����
		window = (IWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;

		if (window != nullptr)
		{
			window->SetHWndPointer(hWnd);
		}
	}

	//�E�B���h�E���ʎ���
	if (window != nullptr)
	{
		return window->localWndProc(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT System::IWindow::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//SetHWndPointer
//	HWND��ݒ�
//
void System::IWindow::SetHWndPointer(HWND hWnd)
{
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	this->_hWnd = hWnd;
}

//*********************************************************************************************************************
//
//	Window
//
//*********************************************************************************************************************


//Window
//	�R���X�g���N�^
//
Window::Window()
{

}

//~Window
//	�f�X�g���N�^
//
Window::~Window()
{
	
}

//Create
//	�E�B���h�E����
//
HRESULT Window::Create(HWND hParent,HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption,int x, int y, long width, long height, DWORD style)
{
	//WndClass�ݒ�
	WNDCLASSEX WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		IWindow::WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		lpClassName,
		NULL
	};

	if (!RegisterClassEx(&WndClass)) {
		MessageBox(NULL, "WndClass�̐ݒ�Ɏ��s���܂����B", "���s", MB_OK);
		return E_FAIL;
	}

	//�E�B���h�E����
	this->_hWnd = CreateWindowEx(
		0,
		lpClassName,
		lpCaption,
		style,
		x,
		y,
		width  + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		hParent,
		NULL,
		hInstance,
		this	//���g�̃|�C���^�ݒ� �v���V�[�W���ɓn��
	);

	if(!this->_hWnd){
		MessageBox(NULL,"hWnd�̐ݒ�Ɏ��s���܂����B","���s",MB_OK);
		return E_FAIL;
	}

	//�\��
	ShowWindow(this->_hWnd, SW_SHOW);
	UpdateWindow(this->_hWnd);

	return S_OK;
}

//localWndProc
//	���[�J���ȃE�B���h�E�v���V�[�W��
//
LRESULT System::Window::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�h���b�v
	HDROP hDrop;
	UINT FileNum = 0;

	char fileName[256];

	//���b�Z�[�W
	switch (uMsg)
	{
		//����
	case WM_CREATE:
		this->CaptionClipProcessID();

		//�t�@�C���̃h���b�O�E�h���b�v����
		if((this->_WindowFlag & WindowFlags_DragDropFile) == WindowFlags_DragDropFile)
			DragAcceptFiles(this->_hWnd, TRUE);
		break;

		//�h���b�v�t�@�C��
	case WM_DROPFILES:
		if ((this->_WindowFlag & WindowFlags_DragDropFile) == WindowFlags_DragDropFile)
		{
			//�h���b�v
			hDrop = (HDROP)wParam;

			//�h���b�v���ꂽ�t�@�C�����擾
			FileNum = DragQueryFile(
				hDrop,
				0xFFFFFFFF,
				NULL,
				0
			);

			//�t�@�C��
			for (UINT i = 0; i < FileNum; i++)
			{
				DragQueryFile(hDrop, i, fileName, sizeof(fileName));
				if (ShellExecute(hWnd, NULL, fileName, NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32)
					MessageBox(hWnd, "�t�@�C�����J���܂���ł����B", "���s", MB_OK);
			}

			//�h���b�v���
			DragFinish(hDrop);
		}
		break;

		//�L�[����
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		default:
			break;
		}
		break;

		//�E�B���h�E����
	case WM_CLOSE:
		if ((this->_WindowFlag & WindowFlags_CloseCheck) == WindowFlags_CloseCheck)
			if (MessageBox(NULL, "�{���ɏI�����Ă���낵���ł����H", "�m�F", MB_OKCANCEL || MB_DEFBUTTON2) != IDOK)
				break;
		DestroyWindow(hWnd);
		break;

		//�E�B���h�E�폜
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		//�f�t�H���g
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

//MessageLoop
//	���b�Z�[�W���[�v�@�C�x���g�h���u��
//
WPARAM System::Window::MessageLoop()
{
	//MainLoop
	MSG msg;

	do
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while (msg.message != WM_QUIT);

	return msg.wParam;
}

void System::Window::CaptionClipProcessID()
{
	if ((this->_WindowFlag & WindowFlags_PreviewProcessID) != WindowFlags_PreviewProcessID) return;
	char caption[256];
	char text[256];

	GetWindowText(_hWnd, caption, ARRAYSIZE(caption));
	sprintf_s(text, " PID:%u", GetCurrentProcessId());
	strcat_s(caption, text);
	SetWindowText(_hWnd, caption);
}
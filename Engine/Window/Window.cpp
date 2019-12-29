#include<Windows.h>

#include"Module\IMGUI\GUI_ImGui.h"
#include"Window.h"

using namespace System;

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
HRESULT Window::Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style)
{
	//WndClass�ݒ�
	this->_WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
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

	if (!RegisterClassEx(&this->_WndClass)) {
		MessageBox(NULL, "WndClass�̐ݒ�Ɏ��s���܂����B", "���s", MB_OK);
		return E_FAIL;
	}

	char text[256];
	sprintf_s(text, " PID:%u", GetCurrentProcessId());

	//�E�B���h�E����

	this->_hWnd = CreateWindowEx(
		0,
		this->_WndClass.lpszClassName,
		lpCaption,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		NULL,
		NULL,
		this->_WndClass.hInstance,
		this	//���g�̃|�C���^�ݒ� �v���V�[�W���ɓn��
	);

	if(!this->_hWnd){
		MessageBox(NULL,"hWnd�̐ݒ�Ɏ��s���܂����B","���s",MB_OK);
		return E_FAIL;
	}

	this->_hInstance = hInstance;

	//�\��
	ShowWindow(this->_hWnd, SW_SHOW);
	UpdateWindow(this->_hWnd);

	return S_OK;
}

//Destroy
//	�E�B���h�E�j��
//
HRESULT Window::Destroy()
{
	UnregisterClass(this->_WndClass.lpszClassName,this->_WndClass.hInstance);
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

//SetHWndPointer
//	HWND��ݒ�
//
void System::Window::SetHWndPointer(HWND hWnd)
{
	SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)this);
	this->_hWnd = hWnd;
}

#ifdef GUI_ImGui_H
//ImGui�̃E�B���h�E�ړ�
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

//WndProc
//	�O���[�o�� �E�B���h�E�v���V�[�W��
//
LRESULT CALLBACK Window::WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
#ifdef GUI_ImGui_H
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
#endif

	Window* window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if(uMsg == WM_CREATE)
	{
		//���[�U�[���ݒ肵��Param�𗘗p����
		window = (Window*)((LPCREATESTRUCT)lParam)->lpCreateParams;

		if(window != nullptr)
		{
			window->SetHWndPointer(hWnd);
		}
	}

	if(window != nullptr)
	{
		return window->localWndProc(hWnd,uMsg,wParam,lParam);
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

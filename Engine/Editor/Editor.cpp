#include<Windows.h>
#include<string>
#include"Module\DirectX\DirectX.h"


#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"
#include "Editor.h"

using namespace DirectX;
using namespace System;

using namespace Editor;

//EditorSubWindow
//	�R���X�g���N�^
//
Editor::EditorSubWindow::EditorSubWindow()
{

}

//~EditorSubWindow
//	�f�X�g���N�^
//
Editor::EditorSubWindow::~EditorSubWindow()
{
	
}

//Create
//	�T�u�E�B���h�E����
//
HRESULT EditorSubWindow::Create(EditorWindow * parent, LPSTR lpClassName, LPSTR lpCaption,int x,int y, long width, long height,DWORD style)
{
	//�T�u�E�B���h�E�ݒ�
	this->_WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		parent->Get_hInstance(),
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_APPWORKSPACE + 1),
		NULL,
		lpClassName,
		NULL
	};

	//�T�u�E�B���h�E�o�^
	if(!RegisterClassEx(&this->_WndClass)){
		MessageBox(NULL,"WndClass�̐ݒ�Ɏ��s���܂����B","EditorSubWindow",MB_OK);
		return E_FAIL;
	}

	//MDI�E�B���h�E�ݒ�
	this->_WndClass.lpfnWndProc = DefMDIChildProc;
	this->_WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	this->_WndClass.lpszClassName = "MDICHILD";

	//MDI�E�B���h�E�o�^
	if (!RegisterClassEx(&this->_WndClass)){
		MessageBox(NULL, "WndClass�̐ݒ�Ɏ��s���܂����B", "EditorSubWindow", MB_OK);
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
		parent->Get_Window(),
		NULL,
		this->_WndClass.hInstance,
		this//this	//���g�̃|�C���^�ݒ� �v���V�[�W���ɓn��
	);

	if (!this->_hWnd) {
		MessageBox(NULL, "hWnd�̐ݒ�Ɏ��s���܂����B", "���s", MB_OK);
		return E_FAIL;
	}

	//�\��
	ShowWindow(this->_hWnd, SW_SHOW);
	UpdateWindow(this->_hWnd);

	return S_OK;
}

//localWndProc
//	�T�u�E�B���h�E�p�E�B���h�E�v���V�[�W��
//
//	SendMessage()	�ʃE�B���h�E�փ��b�Z�[�W�̑��M
//
LRESULT EditorSubWindow::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�T�u�E�B���h�E�擾
	EditorSubWindow* subWindow = (EditorSubWindow*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
	CLIENTCREATESTRUCT ccsClient;
	MDICREATESTRUCT mdic;

	//���b�Z�[�W
	switch (uMsg)
	{
		//�E�B���h�E����
		case WM_CREATE:
			//MDI��L����
			if ((this->_WindowFlag & WindowFlags_MultipleDocumentInterface) == WindowFlags_MultipleDocumentInterface)
			{
				ccsClient.hWindowMenu = NULL;
				ccsClient.idFirstChild = 0x100;

				subWindow->_MDIClient = new HWND;
				*subWindow->_MDIClient = CreateWindow(
					"MDICLIENT",
					NULL,
					WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
					0,
					200,
					0,
					0,
					hWnd,
					(HMENU)1,
					this->_hInstance,
					&ccsClient
				);

				//MDI�E�B���h�E
				mdic.szClass = "MDICHILD";
				mdic.szTitle = "MDIWindow";
				mdic.style = NULL;
				mdic.cx = 200;
				mdic.cy = 200;
				mdic.x = 0;
				mdic.y = 0;
				mdic.hOwner = this->_hInstance;
				mdic.lParam = NULL;

				SendMessage(*subWindow->_MDIClient,WM_MDICREATE,NULL,(LPARAM)&mdic);
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

		//�f�t�H���g
		default:
			if(subWindow->_MDIClient != nullptr)
				return DefFrameProc(hWnd, *subWindow->_MDIClient,uMsg, wParam, lParam);
			else
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}




//EditorWindow
//	�R���X�g���N�^
//
EditorWindow::EditorWindow()
{
	_SubWindow = new EditorSubWindow();
}

//~EditorWindow
//	�f�X�g���N�^
//
EditorWindow::~EditorWindow()
{
	delete _SubWindow;
}

//Create
//	�G�f�B�^�E�B���h�E����
//
HRESULT EditorWindow::Create(HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, long width, long height, DWORD style)
{
	HRESULT hr;
	RECT rect;			//���C���E�B���h�ERECT

	//���C���E�B���h�E
	this->_WindowFlag |= WindowFlags_CloseCheck ^ WindowFlags_DragDropFile;
	hr = Window::Create(hInstance,lpClassName,lpCaption,width,height,style);
	if (FAILED(hr)) return hr;
	GetWindowRect(this->_hWnd,&rect);

	//�T�u�E�B���h�E
	_SubWindow->GetWindowFlags() |= WindowFlags_MultipleDocumentInterface;
	hr = _SubWindow->Create(this,"SubWindow1","SubWindow",rect.right, rect.top,256,height,style);
	if (FAILED(hr)) return hr;

	return hr;
}

//MessageLoop
//	���b�Z�[�W���[�v
//
WPARAM EditorWindow::MessageLoop()
{
	MSG msg;

	//Application�̐���
	D3DApp::Create(
		this->Get_Window(),
		this->Get_hInstance(),
		60
	);

	CManager::Initialize();

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			CManager::Run();
		}
	}
	while (msg.message != WM_QUIT);

	CManager::Finalize();

	D3DApp::Destroy();

	return msg.wParam;
}

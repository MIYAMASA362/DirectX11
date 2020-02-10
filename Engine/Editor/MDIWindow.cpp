#include<Windows.h>
#include<vector>
#include<memory>

#include"Window\Window.h"
#include"Editor\Editor.h"

#include "MDIWindow.h"

#define MDI_CLIENT ("mdiClient")
#define MDI_CHILD  ("mdiChild")

#define MDI_CHILD_FIRST_ID (0x100)

using namespace Editor;

//*********************************************************************************************************************
//
//	MDIWindow
//
//*********************************************************************************************************************


//MDIWindow
//	�R���X�g���N�^
//
MDIWindow::MDIWindow()
{

}


//~MDIWindow
//	�f�X�g���N�^
//
MDIWindow::~MDIWindow()
{

}

//Create
//	MDIClient�̐���
//	MDI�E�B���h�E�̓o�^
//
HRESULT MDIWindow::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	//MDI�e�E�B���h�E
	//�T�u�E�B���h�E�ݒ�
	WNDCLASSEX WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_APPWORKSPACE + 1),
		NULL,
		lpClassName,
		NULL
	};

	//�T�u�E�B���h�E�o�^
	if (!RegisterClassEx(&WndClass)) {
		MessageBox(NULL, "WndClass�̐ݒ�Ɏ��s���܂����B", "EditorSubWindow", MB_OK);
		return E_FAIL;
	}

	//�E�B���h�E����
	this->_hWnd = CreateWindowEx(
		NULL,
		lpClassName,
		lpCaption,
		style,
		x,
		y,
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		hParent,
		NULL,
		hInstance,
		this//this	//���g�̃|�C���^�ݒ� �v���V�[�W���ɓn��
	);

	if (!this->_hWnd) {
		MessageBox(NULL, "hWnd�̐ݒ�Ɏ��s���܂����B", "���s", MB_OK);
		return E_FAIL;
	}

	CLIENTCREATESTRUCT ccsClient;
	ccsClient.hWindowMenu = GetSubMenu(GetMenu(hParent),1);
	ccsClient.idFirstChild = MDI_CHILD_FIRST_ID;

	//MDI�E�B���h�E
	WNDCLASSEX hWndMDIChild = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		DefMDIChildProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)GetStockObject(WHITE_BRUSH),
		NULL,
		MDI_CHILD,
		NULL
	};

	//MDI�E�B���h�E�o�^
	if(!RegisterClassEx(&hWndMDIChild)){
		MessageBox(NULL,"MDI�E�B���h�E�̓o�^�Ɏ��s���܂����B","MDIWindow",MB_OK);
		return E_FAIL;
	}

	//MDI Client����
	this->_Client = CreateWindow(
		MDI_CLIENT,
		NULL,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		x,
		y,
		width,
		height,
		this->_hWnd,
		(HMENU)1,
		hInstance,
		&ccsClient
	);

	return S_OK;
}

//CreateMDI
//	MDIClient�̈��MDI�E�B���h�E�𐶐�����
//
HRESULT Editor::MDIWindow::CreateMDI(LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{	
	HWND hWnd;

	hWnd = CreateMDIWindow(
		MDI_CHILD,
		lpCaption,
		style,
		x,
		y,
		width,
		height,
		this->_Client,
		(HINSTANCE)GetWindowLongPtr(this->_hWnd,GWLP_HINSTANCE),
		NULL
	);

	if(!hWnd)
	{
		MessageBox(NULL,"MDI�E�B���h�E�̐����Ɏ��s���܂����B","MDIWindow",MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

LRESULT Editor::MDIWindow::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefFrameProc(hWnd, this->_Client, uMsg, wParam, lParam);
}

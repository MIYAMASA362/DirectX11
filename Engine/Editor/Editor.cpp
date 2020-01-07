#include<Windows.h>
#include<string>
#include<thread>
#include<map>

#include"Module\DirectX\DirectX.h"

#include"../resource.h"

#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"

#include"Editor\MDIWindow.h"
#include"TreeView.h"

#include "Editor.h"

using namespace DirectX;
using namespace System;

using namespace Editor;

//*********************************************************************************************************************
//
//	EditorSubWindow
//
//*********************************************************************************************************************

//EditorSubWindow
//	�R���X�g���N�^
//
EditorSubWindow::EditorSubWindow(EditorWindow* editor)
	:
	_MainEditor(editor)
{

}

//~EditorSubWindow
//	�f�X�g���N�^
//
EditorSubWindow::~EditorSubWindow()
{
	delete _FileTree;
	delete _MDIWindow;
}

//Create
//	�T�u�E�B���h�E����
//
HRESULT EditorSubWindow::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	//�T�u�E�B���h�E�ݒ�
	this->_WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_APPWORKSPACE + 1),
		MAKEINTRESOURCE(IDR_MENU1),
		lpClassName,
		NULL
	};

	//�T�u�E�B���h�E�o�^
	if (!RegisterClassEx(&this->_WndClass)) {
		MessageBox(NULL, "WndClass�̐ݒ�Ɏ��s���܂����B", "EditorSubWindow", MB_OK);
		return E_FAIL;
	}

	//�E�B���h�E����
	this->_hWnd = CreateWindowEx(
		0,
		lpClassName,
		lpCaption,
		style | WS_CLIPCHILDREN,
		x,
		y,
		width + GetSystemMetrics(SM_CXDLGFRAME) * 2,
		height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
		hParent,
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
	EditorSubWindow* subWindow = (EditorSubWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	//���b�Z�[�W
	switch (uMsg)
	{
		//�E�B���h�E����
	case WM_CREATE:
		//MDI��L����
		if ((this->_WindowFlag & WindowFlags_MultipleDocumentInterface) == WindowFlags_MultipleDocumentInterface)
		{
			//MDI�E�B���h�E�ݒ�
			this->_MDIWindow = new MDIWindow();
			if (FAILED(this->_MDIWindow->Create(hWnd, this->_hInstance, "MDIWindow", "MDIWindow", 0, 0, 300, 300, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_VISIBLE))) {
				MessageBox(NULL, "MDIWindow�̐ݒ�Ɏ��s���܂���", "EditorSubWindow", MB_OK);
				return E_FAIL;
			}
		}

		this->_FileTree = new FileTreeView();
		_FileTree->Create(hWnd, this->_hInstance, "FileTree", "FileView", 0, 0, 500, 300, WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CHILD);
		break;

		//�T�C�Y
	case WM_SIZE:
		RECT rect;
		GetWindowRect(this->_FileTree->Get_Window(), &rect);
		MoveWindow(this->_FileTree->Get_Window(), 0, HIWORD(lParam) - 300, LOWORD(lParam), 300, TRUE);

		if(_RenderStatus)
		{
			_MainEditor->Stop();
			_RenderStatus->CleanupRenderTargetView();
			_RenderStatus->CleanupDepthStencilView();

			_RenderStatus->GetSwapChain()->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			_RenderStatus->SetClientViewport(hWnd);

			_RenderStatus->CreateRenderTargetView();
			_RenderStatus->CreateDepthStencilView();
			_MainEditor->Start();
		}
		break;

		//�R�}���h
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
			//�E�B���h�E => �V�K�쐬
		case ID_WINDOW_40003:
			//MDI�E�B���h�E�̐���
			if ((this->_WindowFlag & WindowFlags_MultipleDocumentInterface) == WindowFlags_MultipleDocumentInterface)
				this->_MDIWindow->CreateMDI("Window", 0, 0, 200, 200, NULL);
			break;

			//�I������
		case ID_40004:
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;

			//�Q�[�����s
		case ID_EDITOR_RUN:
			this->_MainEditor->Start();
			break;

			//�Q�[����~
		case ID_EDITOR_STOP:
			this->_MainEditor->Stop();
			break;

		default:
			break;
		}
		break;

		//�L�[����
	case WM_KEYDOWN:
		switch (wParam)
		{
			//�X�y�[�X
		case VK_ESCAPE:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
			//F1
		case VK_F1:
			WINDOWPLACEMENT placement;
			GetWindowPlacement(hWnd,&placement);
			if(placement.showCmd == SW_SHOWNORMAL)
			{
				ShowWindow(hWnd,SW_SHOWMINNOACTIVE);
			}
			else
			{
				ShowWindow(hWnd, SW_SHOWNORMAL);
				SendMessage(this->_FileTree->Get_Window(), uMsg, wParam, lParam);
			}
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
		ShowWindow(hWnd, SW_HIDE);
		break;

		//�폜����
	case WM_DESTROY:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;

		//�f�t�H���g
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}





//*********************************************************************************************************************
//
//	EditorWindow
//
//*********************************************************************************************************************


//EditorWindow
//	�R���X�g���N�^
//
EditorWindow::EditorWindow()
{
	_SubWindow = new EditorSubWindow(this);
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
HRESULT EditorWindow::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	HRESULT hr;
	RECT rect;			//���C���E�B���h�ERECT

	//���C���E�B���h�E
	this->_WindowFlag |= WindowFlags_CloseCheck ^ WindowFlags_DragDropFile ^ WindowFlags_PreviewProcessID;
	hr = Window::Create(hParent, hInstance, lpClassName, lpCaption, CW_USEDEFAULT, CW_USEDEFAULT, width, height, style);
	if (FAILED(hr)) return hr;
	GetWindowRect(this->_hWnd, &rect);

	//�T�u�E�B���h�E
	//_SubWindow->GetWindowFlags() |= WindowFlags_MultipleDocumentInterface;
	hr = _SubWindow->Create(this->_hWnd, hInstance, "SubWindow1", "SubWindow", rect.right, rect.top, 400, height, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
	if (FAILED(hr)) return hr;

	return hr;
}

LRESULT Editor::EditorWindow::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		if ((this->_WindowFlag & WindowFlags_DragDropFile) == WindowFlags_DragDropFile)
			DragAcceptFiles(this->_hWnd, TRUE);
		break;

		//�t�H�[�J�X���ꂽ
	case WM_SETFOCUS:
		//�őO�ʂɕ\������
		//SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		break;

		//�T�C�Y
	case WM_SIZE:
		
		//Thread���Ⴄ�̂�RenderTarget�ȂǂɃG���[�������
		//	ERROR : �T�C�Y�ς�������Ƌ�����~
		//
		if (_RenderStatus)
		{
			Stop();
			_RenderStatus->CleanupRenderTargetView();
			_RenderStatus->CleanupDepthStencilView();

			_RenderStatus->GetSwapChain()->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			_RenderStatus->SetClientViewport(hWnd);

			_RenderStatus->CreateRenderTargetView();
			_RenderStatus->CreateDepthStencilView();
			Start();
		}
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
			//Space
		case VK_ESCAPE:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
			//F1
		case VK_F1:
			SendMessage(this->_SubWindow->Get_Window(), uMsg, wParam, lParam);
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
//	���b�Z�[�W���[�v
//
WPARAM EditorWindow::MessageLoop()
{
	MSG msg;

	//Application�̐���
	D3DRenderer::Create();

	if (FAILED(D3DRenderer::GetInstance()->CreateRenderStatus(this->_hWnd, &_RenderStatus, 60)))
	{
		MessageBox(NULL, "RenderStatus�̐����Ɏ��s���܂����B", "EditorWindow", MB_OK);
		return 0;
	}

	auto SubWindowRenderStatus = _SubWindow->GetLPRenderStatus();
	if (FAILED(D3DRenderer::GetInstance()->CreateRenderStatus(this->_SubWindow->Get_Window(), &(*SubWindowRenderStatus), 60)))
	{
		MessageBox(NULL, "RenderStatus�̐����Ɏ��s���܂����B", "EditorWindow", MB_OK);
		return 0;
	}

	//GUI
	GUI::guiImGui::Create(this->_SubWindow->Get_Window(), D3DRenderer::GetInstance()->GetDevice(), D3DRenderer::GetInstance()->GetDeviceContext());
	//Manager
	CManager::Initialize(this->_hWnd, 60);


	//Game�r���[�X���b�h
	std::thread thread([this]() { this->RunProcces(); });

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		//����

		


	} while (msg.message != WM_QUIT);

	_threadEnd = true;

	thread.join();

	CManager::Finalize();

	return msg.wParam;
}

//RunProcces
//	�Q�[���E�B���h�E�p�̃X���b�h
//
void Editor::EditorWindow::RunProcces()
{
	auto SubWindowRenderStatus = _SubWindow->GetLPRenderStatus();
	

	_IsRunProcess = true;

	do
	{
		//�������Ȃ�
		if (!_IsRunProcess && !CManager::IsProcess()) continue;

		CManager::SetFrame();

		CManager::Update();
		CManager::FixedUpdate();

		if (!CManager::IsUpdate())
		{
			if (_RenderStatus->IsProcess())
			{
				_RenderStatus->ClearRenderTargetView(Color::gray());

				CManager::Render(_RenderStatus);

				_RenderStatus->End();
			}

			if ((*SubWindowRenderStatus)->IsProcess())
			{
				(*SubWindowRenderStatus)->ClearRenderTargetView(Color::blue());

				CManager::DebugRender();

				(*SubWindowRenderStatus)->End();
			}
		}

		CManager::EndFrame();
		
	}
	while (!_threadEnd);

	_IsRunProcess = false;

	GUI::guiImGui::Destroy();

	D3DRenderer::Destroy();
}

//Start
//
//
void Editor::EditorWindow::Start()
{
	_IsRunProcess = true;
}

//Stop
//
//
void Editor::EditorWindow::Stop()
{
	_IsRunProcess = false;
}

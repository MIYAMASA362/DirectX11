#include<Windows.h>
#include<string>

#include"Window\Window.h"

#include<CommCtrl.h>
#pragma comment(lib,"comctl32.lib")

#include "TreeView.h"

using namespace System;
using namespace Editor;

//*********************************************************************************************************************
//
//	TreeView
//
//*********************************************************************************************************************

//TreeView
//	�R���X�g���N�^
//
FileTreeView::FileTreeView()
{

}

//~TreeView
//	�f�X�g���N�^
//
FileTreeView::~FileTreeView()
{

}

//Create
//	�E�B���h�E����
//
HRESULT FileTreeView::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	this->_WndClass ={
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		NULL,
		lpClassName,
		NULL
	};

	RegisterClassEx(&this->_WndClass);

	this->_hInstance = hInstance;

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
		this
	);

	//�\��
	ShowWindow(this->_hWnd,SW_SHOW);
	UpdateWindow(this->_hWnd);


	return S_OK;
}

//localWndProc
//
//
LRESULT Editor::FileTreeView::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
			//������
		case WM_CREATE:
			InitCommonControls();

			this->_TreeView = CreateWindowEx(
				NULL,
				WC_TREEVIEW,
				NULL,
				WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
				0,
				0,
				0,
				0,
				this->_hWnd,
				(HMENU)3,
				this->_hInstance,
				NULL
			);

			if (!this->_TreeView) {
				MessageBox(NULL, "�c���[�r���[�̐ݒ�Ɏ��s���܂����B", "TreeView", MB_OK);
				return E_FAIL;
			}

			SendMessage(this->_TreeView, SB_SIMPLE, TRUE, 0);

			//�f�B���N�g���擾 ���݂̃f�B���N�g��
			TCHAR currentDir[MAX_PATH + 1];
			GetCurrentDirectory(sizeof(currentDir),currentDir);

			
			TVINSERTSTRUCT tv;
			ZeroMemory(&tv, sizeof(tv));

			//�ǉ�����
			tv.hParent = TVI_ROOT;
			tv.hInsertAfter = TVI_LAST;
			tv.item.mask = TVIF_TEXT;

			FileViewNode(currentDir,tv);
			break;

			//�T�C�Y
		case WM_SIZE:
			MoveWindow(this->_TreeView,0,0,LOWORD(lParam),HIWORD(lParam),TRUE);
			break;

			//�f�t�H���g
		default:
			return DefWindowProc(hWnd,uMsg,wParam,lParam);
			break;
	}
	return 0;
}

//ViewNode
//	�f�B���N�g�����̍��ڂ�TreeView�ɕ\������
//
void FileTreeView::FileViewNode(LPSTR filePath,TVINSERTSTRUCT tv)
{
	HTREEITEM group;
	HANDLE hFind;
	WIN32_FIND_DATA file;

	//���ږ�
	std::string name(filePath);
	size_t pos = name.find_last_of("\\");
	if(pos != std::string::npos)
		name = name.substr(pos+1);
	tv.item.pszText = (LPSTR)name.c_str();

	//���ڃO���[�v
	group = TreeView_InsertItem(this->_TreeView,&tv);
	tv.hParent = group;

	//�t�@�C���̃p�X
	TCHAR path[MAX_PATH +1];
	strcpy_s(path,filePath);
	strcat_s(path,"\\*");

	//�t�@�C����������
	hFind = FindFirstFile(path,&file);

	//�t�@�C������������Ȃ�
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	//�f�B���N�g�����T��
	do
	{
		//�f�B���N�g������
		if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if ((strcmp(file.cFileName, ".") == 0) || (strcmp(file.cFileName, "..") == 0))
				continue;
			TCHAR path[MAX_PATH + 1];
			strcpy_s(path,filePath);
			strcat_s(path,"\\");
			strcat_s(path,file.cFileName);

			FileViewNode(path,tv);
		}
		//�t�@�C�����\��
		else
		{
			tv.item.pszText = file.cFileName;
			TreeView_InsertItem(this->_TreeView,&tv);
		}
	}while (FindNextFile(hFind, &file));

	//�j��
	FindClose(hFind);
}
